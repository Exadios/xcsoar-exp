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

#include "Form/ButtonPanel.hpp"
#include "Renderer/TextButtonRenderer.hpp"
#include "Renderer/SymbolButtonRenderer.hpp"
#include "ui/window/ContainerWindow.hpp"
#include "Screen/Layout.hpp"
#include "ui/event/KeyCode.hpp"
#include "Asset.hpp"

#include <algorithm>

ButtonPanel::ButtonPanel(ContainerWindow &_parent,
                         const ButtonLook &_look) noexcept
  :parent(_parent), look(_look), selected_index(-1) {
  style.TabStop();
}

ButtonPanel::~ButtonPanel() noexcept
{
  for (const auto i : buttons)
    delete i;
}

PixelRect
ButtonPanel::UpdateLayout(const PixelRect rc) noexcept
{
  if (buttons.empty())
    return rc;

  const bool landscape = rc.GetWidth() > rc.GetHeight();
  return landscape
    ? LeftLayout(rc)
    : BottomLayout(rc);
}

PixelRect
ButtonPanel::UpdateLayout() noexcept
{
  return UpdateLayout(parent.GetClientRect());
}

static constexpr PixelRect dummy_rc = { 0, 0, 100, 40 };

Button *
ButtonPanel::Add(std::unique_ptr<ButtonRenderer> &&renderer,
                 Button::Callback callback) noexcept
{
  auto *button = new Button(parent, dummy_rc, style,
                            std::move(renderer), std::move(callback));
  keys[buttons.size()] = 0;
  buttons.append(button);

  return button;
}

Button *
ButtonPanel::Add(const TCHAR *caption, Button::Callback callback) noexcept
{
  return Add(std::make_unique<TextButtonRenderer>(look, caption),
             std::move(callback));
}

Button *
ButtonPanel::AddSymbol(const TCHAR *caption,
                       Button::Callback callback) noexcept
{
  return Add(std::make_unique<SymbolButtonRenderer>(look, caption),
             std::move(callback));
}

void
ButtonPanel::AddKey(unsigned key_code) noexcept
{
  assert(!buttons.empty());
  assert(keys[buttons.size() - 1] == 0);

  keys[buttons.size() - 1] = key_code;
}

inline unsigned
ButtonPanel::Width(unsigned i) const noexcept
{
  return std::max(buttons[i]->GetMinimumWidth(),
                  Layout::GetMinimumControlHeight());
}

unsigned
ButtonPanel::RangeMaxWidth(unsigned start, unsigned end) const noexcept
{
  unsigned max_width = Layout::Scale(50);
  for (unsigned i = start; i < end; ++i) {
    unsigned width = Width(i);
    if (width > max_width)
      max_width = width;
  }

  return max_width;
}

PixelRect
ButtonPanel::VerticalRange(PixelRect rc, unsigned start, unsigned end) noexcept
{
  const unsigned n = end - start;
  assert(n > 0);

  const unsigned width = RangeMaxWidth(start, end);
  const unsigned total_height = rc.GetHeight();
  const unsigned max_height = n * Layout::GetMaximumControlHeight();
  const unsigned row_height = std::min(total_height, max_height) / n;

  auto button_rc = rc.CutLeftSafe(width).TopAligned(row_height);

  for (unsigned i = start; i < end; ++i) {
    buttons[i]->Move(button_rc);

    button_rc.top = button_rc.bottom;
    button_rc.bottom += row_height;
  }

  return rc;
}

PixelRect
ButtonPanel::HorizontalRange(PixelRect rc,
                             unsigned start, unsigned end) noexcept
{
  const unsigned n = end - start;
  assert(n > 0);

  const unsigned total_width = rc.GetWidth();
  const unsigned total_height = rc.GetHeight();
  const unsigned max_row_height = Layout::GetMaximumControlHeight();
  const unsigned row_height = max_row_height < total_height / 2
    ? max_row_height
    : std::max(Layout::GetMinimumControlHeight(),
               total_height / 2);
  const unsigned width = total_width / n;
  assert(width > 0);

  auto button_rc = rc.CutBottomSafe(row_height).LeftAligned(width);

  for (unsigned i = start; i < end; ++i) {
    buttons[i]->Move(button_rc);

    button_rc.left = button_rc.right;
    button_rc.right += width;
  }

  return rc;
}

PixelRect
ButtonPanel::LeftLayout(PixelRect rc) noexcept
{
  if (buttons.empty())
    return rc;

  return VerticalRange(rc, 0, buttons.size());
}

PixelRect
ButtonPanel::LeftLayout() noexcept
{
  return LeftLayout(parent.GetClientRect());
}

inline unsigned
ButtonPanel::FitButtonRow(unsigned start, unsigned total_width) const noexcept
{
  const unsigned n_buttons = buttons.size();
  unsigned max_width = Width(start);
  unsigned i = start + 1;
  for (; i < n_buttons; ++i) {
    /* determine max_width incrementally */
    unsigned width = Width(i);
    if (width > max_width)
      max_width = width;

    /* would adding that button fit into the row? */
    if ((i - start + 1) * max_width > total_width)
      /* no, doesn't fit - omit it and break this loop */
      break;
  }

  return i;
}

PixelRect
ButtonPanel::BottomLayout(PixelRect rc) noexcept
{
  if (buttons.empty())
    return rc;

  const unsigned n_buttons = buttons.size();
  const unsigned total_width = rc.GetWidth();

  /* naive button distribution algorithm: distribute as many buttons
     as possible into each row; weakness: the last row may have only
     one button */

  struct Row {
    unsigned start, end;

    constexpr unsigned size() const {
      return end - start;
    }
  };

  StaticArray<Row, 8u> rows;

  for (unsigned i = 0; i < n_buttons;) {
    unsigned end = FitButtonRow(i, total_width);
    assert(end > i);

    auto &row = rows.append();
    row.start = i;
    row.end = i = end;
  }

  assert(!rows.empty());

  /* optimise the naive result: try to move buttons down until we see
     no further chance for improvement */

  bool modified;
  do {
    modified = false;

    for (unsigned i = rows.size() - 1; i > 0; --i) {
      auto &dest_row = rows[i];
      auto &src_row = rows[i - 1];

      /* the upper row has many more buttons than the lower row */
      if (dest_row.size() + 2 <= src_row.size()) {
        unsigned max_width = RangeMaxWidth(dest_row.start - 1, dest_row.end);
        unsigned row_width = (dest_row.size() + 1) * max_width;

        /* yes, we can move one button down */
        if (row_width <= total_width) {
          --src_row.end;
          --dest_row.start;
          modified = true;
        }
      }
    }
  } while (modified);

  /* now do the actual layout based on row metadata */

  for (int i = rows.size() - 1; i >= 0; --i) {
    const auto &row = rows[i];

    rc = HorizontalRange(rc, row.start, row.end);
  }

  return rc;
}

PixelRect
ButtonPanel::BottomLayout() noexcept
{
  return BottomLayout(parent.GetClientRect());
}

void
ButtonPanel::ShowAll() noexcept
{
  for (auto i : buttons)
    i->Show();
}

void
ButtonPanel::HideAll() noexcept
{
  for (auto i : buttons)
    i->Hide();
}

bool
ButtonPanel::HasFocus() const noexcept
{
  return std::any_of(buttons.begin(), buttons.end(), [](const Button *b){
    return b->HasFocus();
  });
}

void
ButtonPanel::SetSelectedIndex(unsigned _index) noexcept
{
  assert(selected_index >= 0);
  assert(_index < buttons.size());

  if (_index == (unsigned)selected_index)
    return;

  buttons[selected_index]->SetSelected(false);
  selected_index = _index;
  buttons[selected_index]->SetSelected(true);
}

bool
ButtonPanel::SelectPrevious() noexcept
{
  for (int i = selected_index - 1; i >= 0; --i) {
    const auto &button = *buttons[i];
    if (button.IsVisible() && button.IsEnabled()) {
      SetSelectedIndex(i);
      return true;
    }
  }

  return false;
}

bool
ButtonPanel::SelectNext() noexcept
{
  for (unsigned i = selected_index + 1, n = buttons.size();
       i < n; ++i) {
    const auto &button = *buttons[i];
    if (button.IsVisible() && button.IsEnabled()) {
      SetSelectedIndex(i);
      return true;
    }
  }

  return false;
}

bool
ButtonPanel::KeyPress(unsigned key_code) noexcept
{
  assert(key_code != 0);

  const unsigned n = buttons.size();
  for (unsigned i = 0; i < n; ++i) {
    if (keys[i] == key_code) {
      buttons[i]->Click();
      return true;
    }
  }

  if (selected_index >= 0) {
    if (key_code == KEY_LEFT) {
      SelectPrevious();
      return true;
    } else if (key_code == KEY_RIGHT) {
      SelectNext();
      return true;
    } else if (key_code == KEY_RETURN) {
      auto &button = *buttons[selected_index];
      if (button.IsVisible() && button.IsEnabled()) {
        button.Click();
        return true;
      }
    }
  }

  return false;
}
