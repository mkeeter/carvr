#include "overlay.h"

Overlay::Overlay(const wxBitmap& bitmap)
    : bitmap(bitmap), image(bitmap.ConvertToImage()),
      size(bitmap.GetSize()), mouse_over(false)
{
    // Nothing to do here
}

////////////////////////////////////////////////////////////////////////////////

bool Overlay::SendMousePosition(const wxPoint& pos)
{
    const bool filled = Hit(pos);
    const bool changed = (mouse_over != filled);
    mouse_over = filled;

    return changed;
}

////////////////////////////////////////////////////////////////////////////////

bool Overlay::Hit(const wxPoint& pos) const
{
    return pos.x >= 0 && pos.x < size.GetWidth() &&
           pos.y >= 0 && pos.y < size.GetHeight() &&
           !image.GetRed(pos.x, pos.y);
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::Draw(wxGraphicsContext& gc) const
{
    gc.SetBrush(wxBrush(wxColour(255, 255, 255, mouse_over ? 150 : 100)));
    gc.DrawBitmap(bitmap, 0, 0, size.GetWidth(), size.GetHeight());
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::Draw(wxGraphicsContext& gc, const uint8_t alpha) const
{
    gc.SetBrush(wxBrush(wxColour(255, 255, 255, alpha)));
    gc.DrawBitmap(bitmap, 0, 0, size.GetWidth(), size.GetHeight());
}
