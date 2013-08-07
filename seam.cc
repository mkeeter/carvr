#include "seam.h"
#include "energy.h"

////////////////////////////////////////////////////////////////////////////////

Seam GetVerticalSeam(const cv::Mat& summed)
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

cv::Mat DrawVerticalSeam(const cv::Mat& in, const Seam& seam)
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

void RemoveVerticalSeam(cv::Mat& in, const Seam& seam)
{
    Seam::const_iterator index = seam.begin();

    for (int r=0; r < in.rows; ++r) {
        for (int c=*(index++); c < in.cols-1; ++c) {
            if (in.type() == CV_8UC3)
                in.at<cv::Vec3b>(r, c) = in.at<cv::Vec3b>(r, c+1);
            else if (in.type() == CV_8UC1)
                in.at<uint8_t>(r, c) = in.at<uint8_t>(r, c+1);
            else
                assert(false);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////

Seam GetHorizontalSeam(const cv::Mat& summed)
{
    // Find the minimum point on the far right column
    int index;
    {
        cv::Point min_pt;
        cv::minMaxLoc(summed.col(summed.cols - 1),
                      NULL, NULL, &min_pt);
        index = min_pt.y;
    }

    Seam seam;
    seam.push_back(index);

    for (int j=summed.cols - 2; j >= 0; --j) {
        const int a = index == 0 ?
                      INT_MAX : summed.at<int>(index - 1, j);
        const int b = summed.at<int>(index, j);
        const int c = (index == summed.rows - 1) ?
                      INT_MAX : summed.at<int>(index + 1, j);
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

void RemoveHorizontalSeam(cv::Mat& in, const Seam& seam)
{
    Seam::const_iterator index = seam.begin();

    for (int c=0; c < in.cols; ++c) {
        for (int r=*(index++); r < in.rows - 1; r++)
        {
            if (in.type() == CV_8UC3)
                in.at<cv::Vec3b>(r, c) = in.at<cv::Vec3b>(r+1, c);
            else if (in.type() == CV_8UC1)
                in.at<uint8_t>(r, c) = in.at<uint8_t>(r+1, c);
            else
                assert(false);
        }
    }
}

