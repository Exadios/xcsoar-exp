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

#pragma once

#include "RasterLocation.hpp"
#include "thread/SharedMutex.hpp"

#include <cstdint>

struct zzip_dir;
struct GeoPoint;
class RasterTileCache;
class RasterProjection;
class OperationEnvironment;

class TerrainLoader {
  SharedMutex &mutex;

  RasterTileCache &raster_tile_cache;

  const bool scan_overview, scan_tiles;

  OperationEnvironment &env;

  /**
   * The number of remaining segments after the current one.
   */
  mutable unsigned remaining_segments = 0;

public:
  TerrainLoader(SharedMutex &_mutex, RasterTileCache &_rtc,
                bool _scan_overview, bool _scan_all,
                OperationEnvironment &_env)
    :mutex(_mutex), raster_tile_cache(_rtc),
     scan_overview(_scan_overview),
     scan_tiles(!_scan_overview || _scan_all),
     env(_env) {}

  /**
   * Throws on error.
   */
  void LoadOverview(struct zzip_dir *dir,
                    const char *path, const char *world_file);

  /**
   * Throws on error.
   */
  void UpdateTiles(struct zzip_dir *dir, const char *path,
                   SignedRasterLocation p, unsigned radius);

  /* callback methods for libjasper (via jas_rtc.cpp) */

  long SkipMarkerSegment(long file_offset) const;
  void MarkerSegment(long file_offset, unsigned id);

  void ProcessComment(const char *data, unsigned size);

  void StartTile(unsigned index);

  void SetSize(unsigned width, unsigned height,
               uint_least16_t tile_width, uint_least16_t tile_height,
               unsigned tile_columns, unsigned tile_rows);

  void PutTileData(unsigned index,
                   RasterLocation start, RasterLocation end,
                   const struct jas_matrix &m);

private:
  /**
   * Throws on error.
   */
  void LoadJPG2000(struct zzip_dir *dir, const char *path);

  void ParseBounds(const char *data);
};

/**
 * Throws on error.
 *
 * @param all load not only overview, but all tiles?  On large files,
 * this is a very expensive operation.  This option was designed for
 * small RASP files only.
 */
void
LoadTerrainOverview(struct zzip_dir *dir,
                    const char *path, const char *world_file,
                    RasterTileCache &raster_tile_cache,
                    bool all,
                    OperationEnvironment &env);

static inline void
LoadTerrainOverview(struct zzip_dir *dir,
                    RasterTileCache &tile_cache,
                    OperationEnvironment &env)
{
  LoadTerrainOverview(dir, "terrain.jp2", "terrain.j2w",
                      tile_cache, false, env);
}

/**
 * Throws on error.
 */
void
UpdateTerrainTiles(struct zzip_dir *dir, const char *path,
                   RasterTileCache &raster_tile_cache, SharedMutex &mutex,
                   SignedRasterLocation p, unsigned radius);

static inline void
UpdateTerrainTiles(struct zzip_dir *dir,
                   RasterTileCache &tile_cache, SharedMutex &mutex,
                   SignedRasterLocation p, unsigned radius)
{
  UpdateTerrainTiles(dir, "terrain.jp2", tile_cache, mutex, p, radius);
}

void
UpdateTerrainTiles(struct zzip_dir *dir, const char *path,
                   RasterTileCache &raster_tile_cache, SharedMutex &mutex,
                   const RasterProjection &projection,
                   const GeoPoint &location, double radius);

static inline void
UpdateTerrainTiles(struct zzip_dir *dir,
                   RasterTileCache &tile_cache, SharedMutex &mutex,
                   const RasterProjection &projection,
                   const GeoPoint &location, double radius)
{
  UpdateTerrainTiles(dir, "terrain.jp2", tile_cache, mutex,
                     projection, location, radius);
}
