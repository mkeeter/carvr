#include "bitmaps.h"

////////////////////////////////////////////////////////////////////////////////

const static int ARROW_SIZE  = 40;

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
