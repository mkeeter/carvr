#include "panel.h"

////////////////////////////////////////////////////////////////////////////////

ImagePanel::ImagePanel(wxFrame* parent)
    : wxPanel(parent)
{
    Bind(wxEVT_PAINT, &ImagePanel::OnPaint, this);
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
