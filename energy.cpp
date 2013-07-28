#include "energy.hpp"

////////////////////////////////////////////////////////////////////////////////

static int min3(const int a, const int b, const int c)
{
    if (a < b && a < c) {
        return a;
    } else if (c < b && c < a) {
        return c;
    } else {
        return b;
    }
}

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
    return cv::abs(h) + cv::abs(v);
}

////////////////////////////////////////////////////////////////////////////////

// Calculates cumulative energy going down the image.
cv::Mat GetVerticalEnergy(const cv::Mat& energy)
{
    cv::Mat summed(energy.size(), CV_32S);

    // Initialize the bottom row from the energy map
    energy.row(0).copyTo(summed.row(0));

    // Build up energy map for vertical seams.
    for(int j=1; j < energy.rows; ++j) {
        for(int i=0; i < energy.cols; ++i) {
            const int a = (i == 0) ?
                          INT_MAX : summed.at<int32_t>(j-1, i-1);
            const int b = summed.at<int32_t>(j-1, i);
            const int c = (i == energy.cols - 1) ?
                          INT_MAX : summed.at<int32_t>(j-1, i+1);
            summed.at<int>(j, i) = energy.at<int16_t>(j, i) + min3(a, b, c);
        }
    }

    return summed;
}

////////////////////////////////////////////////////////////////////////////////

// Calculates cumulative energy going across the image.
cv::Mat GetHorizontalEnergy(const cv::Mat& energy)
{
    cv::Mat summed(energy.size(), CV_32S);

    // Initialize the left column for horizontal seams
    energy.col(0).copyTo(summed.col(0));

    // Build up energy map for horizontal seams.
    for(int i = 1; i < energy.cols; ++i) {
        for(int j = 0; j < energy.rows; ++j) {
            const int a = (j == 0) ?
                          INT_MAX : summed.at<int32_t>(j-1, i-1);
            const int b = summed.at<int32_t>(j, i-1);
            const int c = (j == energy.rows - 1) ?
                          INT_MAX : summed.at<int32_t>(j+1, i-1);

            summed.at<int>(j, i) = energy.at<int16_t>(j, i) + min3(a, b, c);
        }
    }

    return summed;
}

