#ifndef PANEL_H
#define PANEL_H

#include <wx/wx.h>
#include <opencv2/opencv.hpp>

#include "overlay.h"

class Image;
class Worker;

class ImagePanel : public wxPanel
{
public:
    ImagePanel(wxFrame* parent);
    virtual ~ImagePanel();

    void LoadImage(std::string filename);
    void SaveImage(std::string filename) const;

private:
    const static wxSize min_size;

    void OnPaint(wxPaintEvent& event);
    void OnReloadBitmap(wxThreadEvent& event);
    void OnWorkerDone(wxThreadEvent& event);
    void OnMouseLDown(wxMouseEvent& event);
    void OnMouseLUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnResize(wxSizeEvent& event);

    enum {BASE, DRAG_HORIZONTAL, DRAG_VERTICAL, RESIZING} mode;
    Image* image;
    Worker* worker;
    wxBitmap bitmap;
    wxSize max_size;
    wxPoint mouse_position;
    Overlay arrow_h, arrow_v;
    wxBitmap progress;
};

#endif
