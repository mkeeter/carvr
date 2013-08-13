#include "panel.h"
#include "bitmaps.h"

////////////////////////////////////////////////////////////////////////////////

const wxSize ImagePanel::min_size(40, 40);

////////////////////////////////////////////////////////////////////////////////

ImagePanel::ImagePanel(wxFrame* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(640, 480)),
      mode(BASE), max_size(GetSize()),
      arrow_h(HorizontalArrow(max_size)),
      arrow_v(VerticalArrow(max_size))
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
        gc->DrawBitmap(bitmap, 0, 0, size.GetWidth(), size.GetHeight());
        if (mode == RESIZING && progress.IsOk()) {
            gc->SetBrush(wxBrush(wxColour(255, 255, 255, 100)));
            gc->DrawBitmap(progress, 0, 0, size.GetWidth(), size.GetHeight());
        } else {
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

void ImagePanel::LoadImage(std::string filename)
{
    image = Image(filename);
    bitmap = image.GetBitmap();

    const int w = bitmap.GetWidth();
    const int h = bitmap.GetHeight();
    if (w > h && w > 640) {
        SetSize(wxDefaultCoord, wxDefaultCoord, 640, 640*h/w);
    } else if (h > 640) {
        SetSize(wxDefaultCoord, wxDefaultCoord, 640*w/h, 640);
    } else {
        SetSize(wxDefaultCoord, wxDefaultCoord, w, h);
    }
    max_size = GetSize();

    GetParent()->Fit();
    ((wxTopLevelWindow*)GetParent())->SetTitle(
        _("carvr  (" + filename + ")"));
    Refresh();
}

//////////////////////////////////////////////////////////////////////////////

void ImagePanel::SaveImage(std::string filename) const
{
    image.Save(filename);
}

//////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseLDown(wxMouseEvent& event)
{
    mouse_position = event.GetPosition();

    if (!bitmap.IsOk() || mode == RESIZING) return;

    if      (arrow_h.Hit(mouse_position))   mode = DRAG_HORIZONTAL;
    else if (arrow_v.Hit(mouse_position))   mode = DRAG_VERTICAL;
    else                                    mode = BASE;

    Refresh();
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseLUp(wxMouseEvent& event)
{
    if (mode == RESIZING)   return;
    progress = ProgressBar(GetSize(), 0.5);
    const wxSize size = GetSize();
    if (mode == DRAG_HORIZONTAL) {
        mode = RESIZING;
        const int new_width = bitmap.GetHeight() *
                              size.GetWidth() / size.GetHeight();
        const int tick = bitmap.GetHeight() / size.GetHeight();
        int dw = bitmap.GetWidth() - new_width;
        for (int w = 0; w < dw; ++w) {
            image.RemoveVerticalSeam();
            if (w % tick == 0) {
                bitmap = image.GetBitmap();
                progress = ProgressBar(size, w/float(dw));
                Refresh();
                Update();
                wxYield();
            }
        }
    } else if (mode == DRAG_VERTICAL) {
        mode = RESIZING;
        const int new_height = bitmap.GetWidth() *
                               size.GetHeight() / size.GetWidth();
        const int tick = bitmap.GetWidth() / size.GetWidth();
        const int dh = bitmap.GetHeight() - new_height;
        for (int h=0; h < dh; ++h) {
            image.RemoveHorizontalSeam();
            if (h % tick == 0) {
                bitmap = image.GetBitmap();
                progress = ProgressBar(size, h/float(dh));
                Refresh();
                Update();
                wxYield();
            }
        }
    }

    bitmap = image.GetBitmap();
    mode = BASE;
    max_size = size;
    Refresh();
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnMouseMove(wxMouseEvent& event)
{
    if (!event.LeftIsDown() && mode != RESIZING)    mode = BASE;

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
