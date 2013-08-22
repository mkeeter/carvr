#include "worker.h"

////////////////////////////////////////////////////////////////////////////////

Worker::Worker(Image* image, cv::Size target,
               const int tock, wxEvtHandler* const parent)
    : wxThread(wxTHREAD_JOINABLE), image(image), target(target),
      tock(tock), semaphore(1, 1), parent(parent)
{
    // Nothing to do here
}

////////////////////////////////////////////////////////////////////////////////

wxThread::ExitCode Worker::Entry()
{
    int tick = 0;
    const int ticks = image->Height() - target.height +
                      image->Width()  - target.width;

    while (!TestDestroy())
    {
        if (image->Height() > target.height)     image->RemoveHorizontalSeam();
        else if (image->Width() > target.width)  image->RemoveVerticalSeam();
        else                                     break;

        // Tell the parent window to reload the bitmap
        if ((++tick) % tock == 0) {
            // Stall in an infinite loop until either
            //  a) The thread is stopped
            //  b) We get a semaphore tick (which indicates that the parent
            //     thread has loaded our last bitmap and we can overwrite it)
            while (!TestDestroy() && semaphore.TryWait() == wxSEMA_BUSY);

            bitmap = image->GetBitmap();

            wxThreadEvent* event = new wxThreadEvent(wxEVT_THREAD,
                                                     RELOAD_BITMAP);
            event->SetInt(tick*100/ticks);
            wxQueueEvent(parent, event);
        }
    }

    // Tell the parent window that we're done.
    wxQueueEvent(parent, new wxThreadEvent(wxEVT_THREAD, WORKER_DONE));

    return (wxThread::ExitCode)0;
}

////////////////////////////////////////////////////////////////////////////////

wxBitmap Worker::GetBitmap()
{
    wxBitmap tmp = bitmap;
    semaphore.Post();
    return tmp;
}
