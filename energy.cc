#include "energy.h"

////////////////////////////////////////////////////////////////////////////////

// Calculates cumulative energy going down the image.
void GetEnergy(const cv::Mat& energy, cv::Mat& summed)
{
    energy.copyTo(summed);

    const int c = summed.cols - 1;
    const cv::Range left(0, c-1);
    const cv::Range mid(1, c);
    const cv::Range right(2, c+1);

    // Build up energy map for vertical seams.
    for(int j=1; j < energy.rows; ++j) {
        summed.at<int32_t>(j, 0) += std::min(summed.at<int32_t>(j-1, 0),
                                             summed.at<int32_t>(j-1, 1));

        const cv::Range this_row(j, j+1);
        const cv::Range prev_row(j-1, j);
        summed(this_row, mid) += cv::min(
            summed(prev_row, mid), cv::min(summed(prev_row, left),
                                           summed(prev_row, right)));

        summed.at<int32_t>(j, c) += std::min(summed.at<int32_t>(j-1, c-1),
                                             summed.at<int32_t>(j-1, c));
    }
}

////////////////////////////////////////////////////////////////////////////////
