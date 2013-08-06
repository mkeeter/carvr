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
    const static wxSize min_size;
    enum Axis {AXIS_HORIZONTAL, AXIS_VERTICAL};

    void OnMouseLDown(wxMouseEvent& event);
    void OnMouseLUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnResize(wxSizeEvent& event);

    void ReloadImage(cv::Mat& cv_image, Axis a);

    enum {BASE, DRAG_HORIZONTAL, DRAG_VERTICAL, RESIZING} mode;
    cv::Mat image;
    wxBitmap bitmap;
    wxSize max_size;
    wxPoint mouse_position;
    Overlay arrow_h, arrow_v;
};

#endif
