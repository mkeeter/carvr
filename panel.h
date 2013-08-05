#ifndef PANEL_H
#define PANEL_H

#include <wx/wx.h>
#include <opencv2/opencv.hpp>

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

    void MakeHorizontalArrow();
    void MakeVerticalArrow();

    const static int DRAG_BORDER = 20;
    const static int ARROW_SIZE  = 40;
    enum {NONE, HORIZONTAL, VERTICAL} drag_mode;
    wxSize max_size;
    wxPoint mouse_position;
    wxBitmap bitmap;
    wxBitmap arrow_h, arrow_v;
};

#endif
