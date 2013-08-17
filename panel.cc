#include "panel.h"
#include "bitmaps.h"

#include "image.h"
#include "worker.h"

////////////////////////////////////////////////////////////////////////////////

const wxSize ImagePanel::min_size(40, 40);

////////////////////////////////////////////////////////////////////////////////

ImagePanel::ImagePanel(wxFrame* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(640, 480)),
      mode(BASE), image(NULL), worker(NULL), max_size(GetSize()),
      arrow_h(HorizontalArrow(max_size)),
      arrow_v(VerticalArrow(max_size))
{
    Bind(wxEVT_PAINT, &ImagePanel::OnPaint, this);
    Bind(wxEVT_MOTION, &ImagePanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_DOWN, &ImagePanel::OnMouseLDown, this);
    Bind(wxEVT_LEFT_UP, &ImagePanel::OnMouseLUp, this);
    Bind(wxEVT_SIZE, &ImagePanel::OnResize, this);

    Bind(wxEVT_THREAD, &ImagePanel::OnReloadBitmap, this, RELOAD_BITMAP);
    Bind(wxEVT_THREAD, &ImagePanel::OnWorkerDone, this, WORKER_DONE);
}

////////////////////////////////////////////////////////////////////////////////

ImagePanel::~ImagePanel()
{
    // Shut down the worker thread
    if (worker) {
        worker->Delete();
        delete worker;
    }

    // Delete the image
    delete image;
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

void ImagePanel::OnReloadBitmap(wxThreadEvent& event)
{
    // Load an updated bitmap
    bitmap = image->GetBitmap();

    // Tell the worker thread that it can keep going
    worker->semaphore.Post();

    // Update the progress bar (percent done is passed in the event)
    progress = ProgressBar(GetSize(), event.GetInt()/100.0f);

    // Force a redraw
    Refresh();
}

////////////////////////////////////////////////////////////////////////////////

void ImagePanel::OnWorkerDone(wxThreadEvent& WXUNUSED(event))
{
    // Shuffle around image pointers to reclaim the image
    bitmap = image->GetBitmap();

    // Join the worker thread, then delete it.
    worker->Delete();
    delete worker;
    worker = NULL;

    // Update our local state
    mode = BASE;
    max_size = GetSize();
    Refresh();
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
    delete image;
    image = new Image(filename);
    bitmap = image->GetBitmap();

    const int w = image->Width();
    const int h = image->Height();
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
    image->Save(filename);
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
    const wxSize size = GetSize();
    const int w = size.GetWidth();
    const int h = size.GetHeight();

    if (mode == DRAG_HORIZONTAL) {
        const int new_width = image->Height() * w / h;

        if (new_width == image->Width())    return;
        else                                mode = RESIZING;

        worker = new Worker(image, cv::Size(new_width, image->Height()),
                            image->Height() / h, this);
    } else if (mode == DRAG_VERTICAL) {
        const int new_height = image->Width() * h / w;

        if (new_height == image->Height())  return;
        else                                mode = RESIZING;

        worker = new Worker(image, cv::Size(image->Width(), new_height),
                            image->Width() / w, this);
    } else {
        return;
    }

    // Load up an empty progress bar
    progress = ProgressBar(size, 0);

    // Start up the thread.
    worker->Create();   // Not necessary in wx 2.9.5
    worker->Run();
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
