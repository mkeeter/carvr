#include "energy.hpp"

////////////////////////////////////////////////////////////////////////////////

// Calculates the energy of the image, normalized to 8-bit values.
cv::Mat GetEnergy(const cv::Mat& orig)
{
    // Convert the image to greyscale
    cv::Mat gray;
    cv::cvtColor(orig, gray, CV_BGR2GRAY);

    // Find horizontal derivative
    cv::Mat h;
    cv::Sobel(gray, h, CV_16S, 1, 0);

    // Find vertical derivative
    cv::Mat v;
    cv::Sobel(gray, v, CV_16S, 0, 1);

    // Return the sum of the two
    cv::Mat energy = cv::abs(h) + cv::abs(v);
    energy.convertTo(energy, CV_32S);
    return energy;
}

////////////////////////////////////////////////////////////////////////////////

// Calculates cumulative energy going down the image.
cv::Mat GetVerticalEnergy(const cv::Mat& energy)
{
    cv::Mat summed;
    energy.copyTo(summed);

    const int c = summed.cols - 1;
    const cv::Range left(0, c-1);
    const cv::Range mid(1, c);
    const cv::Range right(2, c+1);

    // Build up energy map for vertical seams.
    for(int j=1; j < energy.rows; ++j) {
        summed.at<int32_t>(j, 0) += std::min(summed.at<int32_t>(j-1, 0),
                                             summed.at<int32_t>(j-1, 1));

        const cv::Range r(j-1, j);
        summed(cv::Range(j, j+1), mid) += cv::min(summed(r, mid),
                                                  cv::min(summed(r, left),
                                                          summed(r, right)));

        summed.at<int32_t>(j, c) += std::min(summed.at<int32_t>(j-1, c-1),
                                             summed.at<int32_t>(j-1, c));
    }

    return summed;
}

////////////////////////////////////////////////////////////////////////////////

// Calculates cumulative energy going across the image.
cv::Mat GetHorizontalEnergy(const cv::Mat& energy)
{
    cv::Mat summed;
    energy.copyTo(summed);

    const int r = summed.rows - 1;
    const cv::Range lower(0, r-1);
    const cv::Range mid(1, r);
    const cv::Range upper(2, r+1);

    // Build up energy map for horizontal seams.
    for(int i = 1; i < energy.cols; ++i) {
        summed.at<int32_t>(0, i) += std::min(summed.at<int32_t>(0, i-1),
                                             summed.at<int32_t>(1, i-1));

        const cv::Range a(i-1, i);
        summed(cv::Range(i, i+1), mid) += cv::min(summed(a, mid),
                                                  cv::min(summed(a, lower),
                                                          summed(a, upper)));

        summed.at<int32_t>(r, i) += std::min(summed.at<int32_t>(r-1, i-1),
                                             summed.at<int32_t>(r, i-1));
    }

    return summed;
}

