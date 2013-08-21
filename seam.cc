#include "seam.h"
#include "energy.h"

////////////////////////////////////////////////////////////////////////////////

Seam GetSeam(const cv::Mat& summed)
{
    // Find the minimum point on the top row
    int index;
    {
        cv::Point min_pt;
        cv::minMaxLoc(summed.row(summed.rows - 1),
                      NULL, NULL, &min_pt);
        index = min_pt.x;
    }

    Seam seam;
    seam.push_back(index);

    for (int j=summed.rows - 2; j >= 0; --j) {
        const int a = index == 0 ?
                      INT_MAX : summed.at<int>(j, index - 1);
        const int b = summed.at<int>(j, index);
        const int c = (index == summed.cols - 1) ?
                      INT_MAX : summed.at<int>(j, index + 1);

        if (a < b && a < c) {
            index--;
        } else if (c < a && c < b) {
            index++;
        }
        seam.push_front(index);
    }
    return seam;
}

////////////////////////////////////////////////////////////////////////////////

cv::Mat DrawSeam(const cv::Mat& in, const Seam& seam)
{
    cv::Mat out;
    in.copyTo(out);
    Seam::const_iterator itr = seam.begin();

    for (int j=0; j < in.rows; ++j, ++itr) {
        if (in.type() == CV_8UC3) {
            out.at<cv::Vec3b>(j, *itr) = cv::Vec3b(255, 0, 0);
        } else if (in.type() == CV_8U) {
            out.at<uint8_t>(j, *itr) = 128;
        }
    }

    return out;
}

////////////////////////////////////////////////////////////////////////////////

void RemoveSeam(cv::Mat& in, const Seam& seam)
{
    const int size = in.elemSize();

    Seam::const_iterator itr = seam.begin();
    for (int r=0; r < in.rows; ++r, ++itr) {
        uchar* const ptr = in.ptr(r);
        const int i = *itr;
        memmove(ptr + i*size, ptr + (i+1)*size, (in.cols - (i+1))*size);
    }
}

////////////////////////////////////////////////////////////////////////////////
