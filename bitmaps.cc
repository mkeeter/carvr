#include "bitmaps.h"

////////////////////////////////////////////////////////////////////////////////

const static int ARROW_SIZE  = 40;
const static int PROGRESS_BAR_SIZE = 40;

////////////////////////////////////////////////////////////////////////////////

wxBitmap HorizontalArrow(const wxSize& size)
{
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

    return bitmap;
}

////////////////////////////////////////////////////////////////////////////////

wxBitmap VerticalArrow(const wxSize& size)
{
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

    return bitmap;
}

////////////////////////////////////////////////////////////////////////////////

wxBitmap ProgressBar(const wxSize& size, const float progress)
{
    wxBitmap bitmap = wxBitmap(size, 1);

    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.Clear();

    const int w = size.GetWidth();
    const int h = size.GetHeight();
    const int b = PROGRESS_BAR_SIZE;

    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen(wxNullPen);
    dc.DrawCircle(w*0.2, h*0.7, b/2);
    dc.DrawCircle(w*0.8, h*0.7, b/2);
    dc.DrawRectangle(w*0.2, h*0.7 - b/2, w*0.6, b);

    dc.SetBrush(*wxWHITE_BRUSH);
    dc.DrawCircle(w*0.2, h*0.7, b*0.8/2);
    dc.DrawCircle(w*0.8, h*0.7, b*0.8/2);
    dc.DrawRectangle(w*0.2, h*0.7 - b*0.8/2, w*0.6, b*0.8);

    dc.SetBrush(*wxBLACK_BRUSH);
    if (progress > 0)
    {
        dc.DrawCircle(w*0.2, h*0.7, b*0.6/2);
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.DrawRectangle(w*0.2, h*0.7 - b*0.6/2, b*0.6/2, b*0.6);
        dc.SetBrush(*wxBLACK_BRUSH);
        dc.DrawRectangle(w*0.2, h*0.7 - b*0.6/2, w*0.6*progress, b*0.6);
    }
    if (progress >= 1)
    {
        dc.DrawCircle(w*0.8, h*0.7, b*0.6/2);
    }

    return bitmap;
}
