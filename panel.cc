#include "panel.h"

////////////////////////////////////////////////////////////////////////////////

ImagePanel::ImagePanel(wxFrame* parent)
    : wxPanel(parent), drag_mode(NONE)
{
    Bind(wxEVT_PAINT, &ImagePanel::OnPaint, this);
    Bind(wxEVT_MOTION, &ImagePanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_DOWN, &ImagePanel::OnMouseLDown, this);
    Bind(wxEVT_LEFT_UP, &ImagePanel::OnMouseLUp, this);
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    if (!bitmap.IsOk())  return;

    wxGraphicsContext* gc = wxGraphicsContext::Create(this);
    const wxSize size = GetSize();
    gc->DrawBitmap(bitmap, 0, 0, size.GetWidth(), size.GetHeight());
    delete gc;
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::LoadImage(cv::Mat& cv_image)
{
    assert(cv_image.type() == CV_8UC3);

    uint8_t* const img_data = (uint8_t*)malloc(cv_image.rows*cv_image.cols*3);
    int a=0;
    for (int j=0; j < cv_image.rows; ++j) {
        for (int i=0; i < cv_image.cols; ++i) {
            cv::Vec3b pixel = cv_image.at<cv::Vec3b>(j, i);
            img_data[a++] = pixel[2];
            img_data[a++] = pixel[1];
            img_data[a++] = pixel[0];
        }
    }

    wxImage wx_image(cv_image.cols, cv_image.rows, img_data);
    bitmap = wxBitmap(wx_image);

    if (cv_image.cols > cv_image.rows && cv_image.cols > 640) {
        SetSize(wxDefaultCoord, wxDefaultCoord,
                640, 640*cv_image.rows/cv_image.cols);
    } else if (cv_image.rows > 640) {
        SetSize(wxDefaultCoord, wxDefaultCoord,
                640*cv_image.cols/cv_image.rows, 640);
    } else {
        SetSize(wxDefaultCoord, wxDefaultCoord, cv_image.cols, cv_image.rows);
    }

    GetParent()->Fit();
    Refresh();
}

//////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseLDown(wxMouseEvent& event)
{
    mouse_position = event.GetPosition();
    if (mouse_position.x >= GetSize().GetWidth() - DRAG_BORDER) {
        drag_mode = HORIZONTAL;
    }
    else if (mouse_position.y >= GetSize().GetHeight() - DRAG_BORDER) {
        drag_mode = VERTICAL;
    } else {
        drag_mode = NONE;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseLUp(wxMouseEvent& event)
{
    drag_mode = NONE;
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseMove(wxMouseEvent& event)
{
    if (!event.LeftIsDown())    drag_mode = NONE;

    const wxPoint mp = event.GetPosition();

    wxSize size = GetSize();
    if (drag_mode == HORIZONTAL) {
        int new_width = size.GetWidth() + mp.x - mouse_position.x;
        if (new_width < DRAG_BORDER)    new_width = DRAG_BORDER;
        size.SetWidth(new_width);
    } else if (drag_mode == VERTICAL) {
        int new_height = size.GetHeight() + mp.y - mouse_position.y;
        if (new_height < DRAG_BORDER)    new_height = DRAG_BORDER;
        size.SetHeight(new_height);
    } else {
        if (mp.x >= size.GetWidth() - DRAG_BORDER ||
            mp.y >= size.GetHeight() - DRAG_BORDER)
        {
            SetCursor(wxCURSOR_HAND);
        } else {
            SetCursor(wxCURSOR_CROSS);
        }
    }

    if (size != GetSize())
    {
        SetSize(size);
        GetParent()->Fit();
    }

    mouse_position = mp;
}
