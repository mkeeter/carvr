/*Post BITMAP_READY to main thread
waits on semaphore

main thread listens for BITMAP_READY
loads bitmap
post semaphore



Post WORKER_DONE to main thread
waits on semaphore
and halts

main thread listens for WORKER_DONE and reclaims image
then post semaphore
and join thread
*/

#include "worker.h"

Worker::Worker(Image* image, cv::Size target,
               const int tock, wxEvtHandler* const parent)
    : wxThread(wxTHREAD_JOINABLE), semaphore(0, 1), image(image),
      target(target), tock(tock), parent(parent)
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
            wxThreadEvent* event = new wxThreadEvent(wxEVT_THREAD,
                                                     RELOAD_BITMAP);
            event->SetInt(tick*100/ticks);
            wxQueueEvent(parent, event);

            // Stall in an infinite loop until either
            //  a) The thread is stopped
            //  b) We get a semaphore tick
            while (!TestDestroy() && semaphore.TryWait() == wxSEMA_BUSY);
        }
    }

    // Tell the parent window that we're done.
    wxQueueEvent(parent, new wxThreadEvent(wxEVT_THREAD, WORKER_DONE));

    return (wxThread::ExitCode)0;
}
