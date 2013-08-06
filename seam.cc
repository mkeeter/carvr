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

cv::Mat RemoveVerticalSeam(const cv::Mat& in, const Seam& seam)
{
    cv::Mat out(cv::Size(in.cols-1, in.rows), in.type());

    Seam::const_iterator index = seam.begin();

    for (int j=0; j < in.rows; ++j, ++index) {
        const cv::Range r(j, j+1);
        if (*index > 0) {
            in(r, cv::Range(0, *index)).copyTo(
                    out(r, cv::Range(0, *index)));
        }
        if (*index < in.cols - 1) {
            in(r, cv::Range(*index+1, in.cols)).copyTo(
                    out(r, cv::Range(*index, out.cols)));
        }
    }

    return out;
}

////////////////////////////////////////////////////////////////////////////////

cv::Mat RemoveVerticalSeam(const cv::Mat& orig)
{
    const cv::Mat energy = GetEnergy(orig);
    const cv::Mat vertical = GetVerticalEnergy(energy);
    const Seam seam = GetVerticalSeam(vertical);
    return RemoveVerticalSeam(orig, seam);
}
