/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2025 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Topography/TopographyFileRenderer.hpp"
#include "Topography/TopographyFile.hpp"
#include "Topography/XShape.hpp"
#include "Look/TopographyLook.hpp"
#include "Renderer/LabelBlock.hpp"
#include "Projection/WindowProjection.hpp"
#include "ui/canvas/Canvas.hpp"
#include "ui/canvas/Features.hpp"
#include "Screen/Layout.hpp"
#include "shapelib/mapserver.h"
#include "util/AllocatedArray.hxx"
#include "util/tstring.hpp"
#include "Geo/GeoClip.hpp"
#include "Geo/FAISphere.hpp"

#ifdef ENABLE_OPENGL
#include "ui/canvas/opengl/VertexPointer.hpp"
#include "ui/canvas/opengl/Buffer.hpp"
#include "ui/canvas/opengl/Dynamic.hpp"
#include "ui/canvas/opengl/Geo.hpp"

#include "ui/canvas/opengl/Program.hpp"
#include "ui/canvas/opengl/Shaders.hpp"

#include <glm/gtc/type_ptr.hpp>
#endif

#include <algorithm>
#include <numeric>
#include <set>

TopographyFileRenderer::TopographyFileRenderer(const TopographyFile &_file,
                                               const TopographyLook &_look) noexcept
  :file(_file), look(_look),
   pen(Layout::ScaleFinePenWidth(file.GetPenWidth()), Color{file.GetColor()})
#ifndef ENABLE_OPENGL
  , brush(Color{file.GetColor()})
#endif
{
  ResourceId icon_ID = file.GetIcon();
  if (icon_ID.IsDefined())
    icon.LoadResource(icon_ID, file.GetBigIcon());
}

TopographyFileRenderer::~TopographyFileRenderer() noexcept = default;

void
TopographyFileRenderer::UpdateVisibleShapes(const WindowProjection &projection) noexcept
{
  if (file.GetSerial() == visible_serial &&
      visible_bounds.IsInside(projection.GetScreenBounds()) &&
      projection.GetScreenBounds().Scale(2).IsInside(visible_bounds))
    /* cache is clean */
    return;

  visible_serial = file.GetSerial();
  visible_bounds = projection.GetScreenBounds().Scale(1.2);
  visible_shapes.clear();
  visible_points.clear();
  visible_labels.clear();

  for (const XShape &shape : file) {
    if (!visible_bounds.Overlaps(shape.get_bounds()))
      continue;

    if (shape.get_type() != MS_SHAPE_NULL) {
      if (shape.get_type() == MS_SHAPE_POINT) {
        if (icon.IsDefined()) {
          const auto *points = shape.GetPoints();
          for (const unsigned line_size : shape.GetLines()) {
            const auto *end = points + line_size;
            for (; points < end; ++points) {
#ifdef ENABLE_OPENGL
              visible_points.push_back(file.ToGeoPoint(*points));
#else
              visible_points.push_back(*points);
#endif
            }
          }
        }
      } else
        visible_shapes.push_back(&shape);
    }

    if (shape.GetLabel() != nullptr)
      visible_labels.push_back(&shape);
  }
}

#ifdef ENABLE_OPENGL

inline void
TopographyFileRenderer::UpdateArrayBuffer() noexcept
{
  if (array_buffer == nullptr)
    array_buffer = std::make_unique<GLArrayBuffer>();
  else if (file.GetSerial() == array_buffer_serial)
    return;

  array_buffer_serial = file.GetSerial();

  unsigned n = 0;
  for (auto &shape : file) {
    shape.SetOffset(n);

    const auto lines = shape.GetLines();
    n = std::accumulate(lines.begin(), lines.end(), n);
  }

  ShapePoint *p = (ShapePoint *)
    array_buffer->BeginWrite(n * sizeof(*p));
  assert (p != nullptr);

  for (const auto &shape : file) {
    const auto lines = shape.GetLines();
    const ShapePoint *src = shape.GetPoints();
    for (const auto n_points : lines) {
      p = std::copy_n(src, n_points, p);
      src += n_points;
    }
  }

  array_buffer->CommitWrite(n * sizeof(*p), p - n);
}

#endif

inline void
TopographyFileRenderer::PaintPoints(Canvas &canvas,
                                    const WindowProjection &projection) noexcept
{
  for (const auto &point : visible_points) {
    if (auto p = projection.GeoToScreenIfVisible(point))
      icon.Draw(canvas, *p);
  }
}

void
TopographyFileRenderer::Paint(Canvas &canvas,
                              const WindowProjection &projection) noexcept
{
  const std::lock_guard lock{file.mutex};

  const auto map_scale = projection.GetMapScale();
  if (!file.IsVisible(map_scale))
    return;

  UpdateVisibleShapes(projection);
  PaintPoints(canvas, projection);

  if (visible_shapes.empty())
    return;

#ifdef ENABLE_OPENGL
  OpenGL::solid_shader->Use();

  UpdateArrayBuffer();
  array_buffer->Bind();
  const ShapePoint *const buffer = nullptr;

  pen.Bind();

  if (!pen.GetColor().IsOpaque()) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
#else
  shape_renderer.Configure(&pen, &brush);
#endif

  // get drawing info

#ifdef ENABLE_OPENGL
  const unsigned level = file.GetThinningLevel(map_scale);
  const ShapeScalar min_distance =
    ShapeScalar(file.GetMinimumPointDistance(level))
    / (Layout::Scale(1) * FAISphere::REARTH);

  glUniformMatrix4fv(OpenGL::solid_modelview, 1, GL_FALSE,
                     glm::value_ptr(ToGLM(projection, file.GetCenter())));
#else // !ENABLE_OPENGL
  const GeoClip clip(projection.GetScreenBounds().Scale(1.1));
  AllocatedArray<GeoPoint> geo_points;

  const unsigned iskip = file.GetSkipSteps(map_scale);
#endif

#ifdef ENABLE_OPENGL
  ScopeVertexPointer vp;

#ifdef GL_EXT_multi_draw_arrays
  std::vector<GLsizei> polygon_counts;
  std::vector<GLushort> polygon_indices;
#endif
#endif

  for (const XShape *shape_p : visible_shapes) {
    const XShape &shape = *shape_p;

    const auto lines = shape.GetLines();
#ifdef ENABLE_OPENGL
    const ShapePoint *points = buffer + shape.GetOffset();
#else // !ENABLE_OPENGL
    const GeoPoint *points = shape.GetPoints();
#endif

    switch (shape.get_type()) {
    case MS_SHAPE_NULL:
    case MS_SHAPE_POINT:
      break;

    case MS_SHAPE_LINE:
      {
#ifdef ENABLE_OPENGL
        vp.Update(GL_FLOAT, points);

        XShape::Indices indices;
        if (level == 0 ||
            (indices = shape.GetIndices(level, min_distance)).indices == nullptr) {
          unsigned offset = 0;
          for (unsigned n : lines) {
            glDrawArrays(GL_LINE_STRIP, offset, n);
            offset += n;
          }
        } else {
          for (unsigned n : std::span<const GLushort>{indices.count, lines.size()}) {
            glDrawElements(GL_LINE_STRIP, n, GL_UNSIGNED_SHORT,
                           indices.indices);
            indices.indices += n;
          }
        }
#else // !ENABLE_OPENGL
        for (unsigned msize : lines) {
        shape_renderer.Begin(msize);

        const GeoPoint *end = points + msize - 1;
        for (; points < end; ++points)
          shape_renderer.AddPointIfDistant(projection.GeoToScreen(*points));

        // make sure we always draw the last point
        shape_renderer.AddPoint(projection.GeoToScreen(*points));

        shape_renderer.FinishPolyline(canvas);
      }
#endif
      }
      break;

    case MS_SHAPE_POLYGON:
#ifdef ENABLE_OPENGL
      {
        const auto triangles = shape.GetIndices(level, min_distance);
        const unsigned n = *triangles.count;

#ifdef GL_EXT_multi_draw_arrays
        const unsigned offset = shape.GetOffset();
        if (GLExt::HaveMultiDrawElements() && offset + n < 0x10000) {
          /* postpone, draw many polygons with a single
             glMultiDrawElements() call */
          polygon_counts.push_back(n);
          const size_t size = polygon_indices.size();
          polygon_indices.resize(size + n, offset);
          for (unsigned i = 0; i < n; ++i)
            polygon_indices[size + i] += triangles.indices[i];
          break;
        }
#endif

        vp.Update(GL_FLOAT, points);
        glDrawElements(GL_TRIANGLE_STRIP, n, GL_UNSIGNED_SHORT,
                       triangles.indices);
      }
#else // !ENABLE_OPENGL
      {
        const GeoPoint *src = &points[0];
        for (const unsigned n : lines) {
          unsigned msize = n / iskip;

          /* copy all polygon points into the geo_points array and
             clip them, to avoid integer overflows (as PixelPoint may
             store only 16 bit integers on some platforms) */

          geo_points.GrowDiscard(msize * 3);
          for (unsigned i = 0; i < msize; ++i)
            geo_points[i] = src[i * iskip];

          msize = clip.ClipPolygon(geo_points.data(),
                                   geo_points.data(), msize);
          if (msize < 3)
            continue;

          shape_renderer.Begin(msize);

          for (unsigned i = 0; i < msize; ++i) {
            GeoPoint g = geo_points[i];
            shape_renderer.AddPointIfDistant(projection.GeoToScreen(g));
          }

          shape_renderer.FinishPolygon(canvas);

          src += n;
        }
      }
#endif
      break;
    }
  }
#ifdef ENABLE_OPENGL

#ifdef GL_EXT_multi_draw_arrays
  if (!polygon_indices.empty()) {
    assert(GLExt::HaveMultiDrawElements());

    std::vector<const GLushort *> polygon_pointers;
    unsigned i = 0;
    for (auto count : polygon_counts) {
      polygon_pointers.push_back(polygon_indices.data() + i);
      i += count;
    }

    vp.Update(GL_FLOAT, buffer);

    GLExt::MultiDrawElements(GL_TRIANGLE_STRIP, polygon_counts.data(),
                             GL_UNSIGNED_SHORT,
                             (const GLvoid **)polygon_pointers.data(),
                             polygon_counts.size());
  }
#endif

  glUniformMatrix4fv(OpenGL::solid_modelview, 1, GL_FALSE,
                     glm::value_ptr(glm::mat4(1)));
  if (!pen.GetColor().IsOpaque())
    glDisable(GL_BLEND);

  pen.Unbind();

  array_buffer->Unbind();
#else
  shape_renderer.Commit();
#endif
}

void
TopographyFileRenderer::PaintLabels(Canvas &canvas,
                                    const WindowProjection &projection,
                                    LabelBlock &label_block) noexcept
{
  const std::lock_guard lock{file.mutex};

  const auto map_scale = projection.GetMapScale();
  if (!file.IsVisible(map_scale) || !file.IsLabelVisible(map_scale))
    return;

  UpdateVisibleShapes(projection);

  if (visible_labels.empty())
    return;

  canvas.Select(file.IsLabelImportant(map_scale)
                ? look.important_label_font
                : look.regular_label_font);
  canvas.SetTextColor(file.IsLabelImportant(map_scale) ?
                COLOR_BLACK : COLOR_VERY_DARK_GRAY);
  canvas.SetBackgroundTransparent();

  // get drawing info

  int iskip = file.GetSkipSteps(map_scale);

  std::set<tstring> drawn_labels;

  // Iterate over all shapes in the file
  for (const XShape *shape_p : visible_labels) {
    const XShape &shape = *shape_p;

    // Skip shapes without a label
    const TCHAR *label = shape.GetLabel();
    assert(label != nullptr);

    const auto lines = shape.GetLines();
    const auto *points = shape.GetPoints();

    for (const unsigned n : lines) {
      int minx = canvas.GetWidth();
      int miny = canvas.GetHeight();

      const auto *end = points + n;
      for (; points < end; points += iskip) {
#ifdef ENABLE_OPENGL
        auto pt = projection.GeoToScreen(file.ToGeoPoint(*points));
#else
        auto pt = projection.GeoToScreen(*points);
#endif

        if (pt.x <= minx) {
          minx = pt.x;
          miny = pt.y;
        }
      }

      points = end;

      minx += 2;
      miny += 2;

      PixelSize tsize = canvas.CalcTextSize(label);
      PixelRect brect;
      brect.left = minx;
      brect.right = brect.left + tsize.width;
      brect.top = miny;
      brect.bottom = brect.top + tsize.height;

      if (!label_block.check(brect))
        continue;

      if (!drawn_labels.insert(label).second)
        continue;

      canvas.DrawText({minx, miny}, label);
    }
  }
}
