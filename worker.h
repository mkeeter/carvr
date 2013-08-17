#ifndef WORKER_H
#define WORKER_H

#include <opencv2/opencv.hpp>
#include <wx/wx.h>

#include "image.h"

#define RELOAD_BITMAP   2000
#define WORKER_DONE     2001

class Worker : public wxThread
{
public:
    Worker(Image* const image, const cv::Size target,
           const int tock, wxEvtHandler* const parent);

    wxThread::ExitCode Entry();
    wxBitmap GetBitmap();

private:
    Image* image;
    wxBitmap bitmap;
    const cv::Size target;
    const int tock;

    wxSemaphore semaphore;
    wxEvtHandler* const parent;
};

#endif
