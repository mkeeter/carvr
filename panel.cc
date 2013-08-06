#include "panel.h"

////////////////////////////////////////////////////////////////////////////////

ImagePanel::ImagePanel(wxFrame* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(640, 480)),
      drag_mode(NONE), max_size(GetSize())
{
    Bind(wxEVT_PAINT, &ImagePanel::OnPaint, this);
    Bind(wxEVT_MOTION, &ImagePanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_DOWN, &ImagePanel::OnMouseLDown, this);
    Bind(wxEVT_LEFT_UP, &ImagePanel::OnMouseLUp, this);
    Bind(wxEVT_SIZE, &ImagePanel::OnResize, this);

    MakeHorizontalArrow();
    MakeVerticalArrow();
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(this);
    const wxSize size = GetSize();

    if (bitmap.IsOk()) {
        const int w = size.GetWidth();
        const int h = size.GetHeight();
        gc->DrawBitmap(bitmap, 0, 0, w, h);
        if (drag_mode == HORIZONTAL)    arrow_h.Draw(*gc, 255);
        else                            arrow_h.Draw(*gc);

        if (drag_mode == VERTICAL)      arrow_v.Draw(*gc, 255);
        else                            arrow_v.Draw(*gc);

    } else {
        gc->SetBrush(wxBrush(wxColour(200, 200, 200)));
        gc->DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

        gc->SetBrush(wxBrush(wxColour(220, 220, 220)));
        for (int j=0; j <= size.GetHeight() - 20; j += 20) {
            for (int i=j % 40; i <= size.GetWidth() - 20; i += 40) {
                gc->DrawRectangle(i, j, 20, 20);
            }
        }
    }
    delete gc;
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnResize(wxSizeEvent& WXUNUSED(event))
{
    MakeHorizontalArrow();
    MakeVerticalArrow();
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::MakeHorizontalArrow(void)
{
    const wxSize size = GetSize();
    wxBitmap bitmap = wxBitmap(size, 1);

    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.Clear();

    wxPoint points[] = {
        wxPoint(0, ARROW_SIZE/4),
        wxPoint(-ARROW_SIZE/2, ARROW_SIZE/4),
        wxPoint(-ARROW_SIZE/2, ARROW_SIZE/2),
        wxPoint(-ARROW_SIZE, 0),
        wxPoint(-ARROW_SIZE/2, -ARROW_SIZE/2),
        wxPoint(-ARROW_SIZE/2, -ARROW_SIZE/4),
        wxPoint(0, -ARROW_SIZE/4)};
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen(wxNullPen);
    dc.DrawPolygon(7, points, size.GetWidth(), size.GetHeight()/2);

    arrow_h = Overlay(bitmap);
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::MakeVerticalArrow(void)
{
    const wxSize size = GetSize();
    wxBitmap bitmap = wxBitmap(size, 1);

    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.Clear();

    wxPoint points[] = {
        wxPoint(ARROW_SIZE/4, 0),
        wxPoint(ARROW_SIZE/4, -ARROW_SIZE/2),
        wxPoint(ARROW_SIZE/2, -ARROW_SIZE/2),
        wxPoint(0, -ARROW_SIZE),
        wxPoint(-ARROW_SIZE/2, -ARROW_SIZE/2),
        wxPoint(-ARROW_SIZE/4, -ARROW_SIZE/2),
        wxPoint(-ARROW_SIZE/4, 0)};
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen(wxNullPen);
    dc.DrawPolygon(7, points, size.GetWidth()/2, size.GetHeight());

    arrow_v = Overlay(bitmap);
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

    max_size = GetSize();
    GetParent()->Fit();
    Refresh();
}

//////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseLDown(wxMouseEvent& event)
{
    mouse_position = event.GetPosition();

    if (!bitmap.IsOk()) return;

    if      (arrow_h.Hit(mouse_position))    drag_mode = HORIZONTAL;
    else if (arrow_v.Hit(mouse_position))    drag_mode = VERTICAL;
    else                                     drag_mode = NONE;

    Refresh();
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseLUp(wxMouseEvent& event)
{
    drag_mode = NONE;
    max_size = GetSize();
    Refresh();
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseMove(wxMouseEvent& event)
{
    if (!event.LeftIsDown())    drag_mode = NONE;

    const wxPoint mp = event.GetPosition();
    bool changed = arrow_h.SendMousePosition(mp) ||
                   arrow_v.SendMousePosition(mp);

    wxSize size = GetSize();
    if (drag_mode == HORIZONTAL) {
        int new_width = size.GetWidth() + mp.x - mouse_position.x;
        if (new_width < DRAG_BORDER)
            new_width = DRAG_BORDER;
        if (new_width > max_size.GetWidth())
            new_width = max_size.GetWidth();
        size.SetWidth(new_width);
    } else if (drag_mode == VERTICAL) {
        int new_height = size.GetHeight() + mp.y - mouse_position.y;
        if (new_height < DRAG_BORDER)
            new_height = DRAG_BORDER;
        if (new_height > max_size.GetHeight())
            new_height = max_size.GetHeight();
        size.SetHeight(new_height);
    }

    if (size != GetSize())
    {
        changed = false;
        SetSize(size);
        GetParent()->Fit();
    }

    mouse_position = mp;
    if (changed)    Refresh();
}
