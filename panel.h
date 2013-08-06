#ifndef PANEL_H
#define PANEL_H

#include <wx/wx.h>
#include <opencv2/opencv.hpp>

#include "overlay.h"

class ImagePanel : public wxPanel
{
public:
    ImagePanel(wxFrame* parent);
    void OnPaint(wxPaintEvent& event);
    void LoadImage(cv::Mat& image);
private:
    void OnMouseLDown(wxMouseEvent& event);
    void OnMouseLUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnResize(wxSizeEvent& event);

    const static wxSize min_size;

    enum {NONE, HORIZONTAL, VERTICAL} drag_mode;
    wxSize max_size;
    wxPoint mouse_position;
    wxBitmap bitmap;
    Overlay arrow_h, arrow_v;
};

#endif
