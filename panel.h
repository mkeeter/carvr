#ifndef PANEL_H
#define PANEL_H

#include <wx/wx.h>
#include <opencv2/opencv.hpp>

#include "overlay.h"
#include "image.h"

class ImagePanel : public wxPanel
{
public:
    ImagePanel(wxFrame* parent);
    void OnPaint(wxPaintEvent& event);
    void LoadImage(std::string filename);
private:
    const static wxSize min_size;

    void OnMouseLDown(wxMouseEvent& event);
    void OnMouseLUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnResize(wxSizeEvent& event);

    enum {BASE, DRAG_HORIZONTAL, DRAG_VERTICAL, RESIZING} mode;
    Image image;
    wxBitmap bitmap;
    wxSize max_size;
    wxPoint mouse_position;
    Overlay arrow_h, arrow_v;
};

#endif
