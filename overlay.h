#ifndef OVERLAY_H
#define OVERLAY_H

#include <wx/wx.h>

class Overlay
{
public:
    Overlay() {}
    Overlay(const wxBitmap& bitmap);
    bool SendMousePosition(const wxPoint& pos);
    bool Hit(const wxPoint& pos) const;
    void Draw(wxGraphicsContext& gc) const;
    void Draw(wxGraphicsContext& gc, const uint8_t alpha) const;
private:
    wxBitmap bitmap;
    wxImage  image;
    wxSize size;
    bool  mouse_over;
};

#endif
