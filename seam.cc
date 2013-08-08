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
    Seam::const_iterator index = seam.begin();

    for (int j=0; j < in.rows; ++j, ++index) {
        out.at<cv::Vec3b>(j, *index) = cv::Vec3b(255, 0, 0);
    }

    return out;
}

////////////////////////////////////////////////////////////////////////////////

void RemoveSeam(cv::Mat& in, const Seam& seam)
{
    const int type = in.type();
    assert(type == CV_8UC3 || type == CV_8UC1);
    const int size = (type == CV_8UC3) ? 3 : 1;

    Seam::const_iterator index = seam.begin();
    for (int r=0; r < in.rows; ++r) {
        uchar* const ptr = in.ptr(r);
        const int i = *(index++);
        memmove(ptr + i*size, ptr + (i+1)*size, (in.cols - (i+1))*size);
    }
}

////////////////////////////////////////////////////////////////////////////////
