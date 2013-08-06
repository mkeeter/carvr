#include "panel.h"
#include "bitmaps.h"

#include "seam.h"
#include "energy.h"

////////////////////////////////////////////////////////////////////////////////

const wxSize ImagePanel::min_size(40, 40);

////////////////////////////////////////////////////////////////////////////////

ImagePanel::ImagePanel(wxFrame* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(640, 480)),
      mode(BASE), max_size(GetSize()),
      arrow_h(HorizontalArrow(max_size)), arrow_v(VerticalArrow(max_size))
{
    Bind(wxEVT_PAINT, &ImagePanel::OnPaint, this);
    Bind(wxEVT_MOTION, &ImagePanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_DOWN, &ImagePanel::OnMouseLDown, this);
    Bind(wxEVT_LEFT_UP, &ImagePanel::OnMouseLUp, this);
    Bind(wxEVT_SIZE, &ImagePanel::OnResize, this);
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
        if (mode != RESIZING) {
            if (mode == DRAG_HORIZONTAL)    arrow_h.Draw(*gc, 255);
            else                            arrow_h.Draw(*gc);

            if (mode == DRAG_VERTICAL)      arrow_v.Draw(*gc, 255);
            else                            arrow_v.Draw(*gc);
        }

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
    const wxSize size = GetSize();
    arrow_h = HorizontalArrow(size);
    arrow_v = VerticalArrow(size);
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::LoadImage(cv::Mat& cv_image)
{
    assert(cv_image.type() == CV_8UC3);
    image = cv_image;

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

void ImagePanel::ReloadImage(cv::Mat& cv_image)
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

    Refresh();
}

//////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseLDown(wxMouseEvent& event)
{
    mouse_position = event.GetPosition();

    if (!bitmap.IsOk()) return;

    if      (arrow_h.Hit(mouse_position))    mode = DRAG_HORIZONTAL;
    else if (arrow_v.Hit(mouse_position))    mode = DRAG_VERTICAL;
    else                                     mode = BASE;

    Refresh();
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseLUp(wxMouseEvent& event)
{
    const wxSize size = GetSize();
    if (mode == DRAG_HORIZONTAL) {
        mode = RESIZING;
        int new_width = image.rows * size.GetWidth() / size.GetHeight();
        while (image.cols > new_width) {
            image = RemoveVerticalSeam(image);
            ReloadImage(image);
            Update();
            wxYield();
        }
    } else if (mode == DRAG_VERTICAL) {
        mode = RESIZING;
        int new_height = image.cols * size.GetHeight() / size.GetWidth();
        while (image.rows > new_height) {
            image = RemoveHorizontalSeam(image);
            ReloadImage(image);
            Update();
            wxYield();
        }
    }
    mode = BASE;
    max_size = size;
    Refresh();
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseMove(wxMouseEvent& event)
{
    if (!event.LeftIsDown())    mode = BASE;

    const wxPoint mp = event.GetPosition();
    bool changed = arrow_h.SendMousePosition(mp) ||
                   arrow_v.SendMousePosition(mp);

    wxSize size = GetSize();
    if (mode == DRAG_HORIZONTAL) {
        int new_width = size.GetWidth() + mp.x - mouse_position.x;
        if (new_width < min_size.GetWidth())
            new_width = min_size.GetWidth();
        if (new_width > max_size.GetWidth())
            new_width = max_size.GetWidth();
        size.SetWidth(new_width);
    } else if (mode == DRAG_VERTICAL) {
        int new_height = size.GetHeight() + mp.y - mouse_position.y;
        if (new_height < min_size.GetHeight())
            new_height = min_size.GetHeight();
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
