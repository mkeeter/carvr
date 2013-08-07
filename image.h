#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include <opencv2/opencv.hpp>
#include <wx/wx.h>

class Image
{
public:
    Image() {};
    Image(std::string filename);
    void RemoveHorizontalSeam();
    void RemoveVerticalSeam();
    wxBitmap GetBitmap() const;
private:
    void RecalculateEnergy();
    void ChangeImageSizes(const cv::Range r, const cv::Range c);

    std::string filename;

    // At the end of every function, these matrices should be
    // of the correct size and storing correct data.
    cv::Mat img, bw;
    cv::Mat sobel_h, sobel_v;
    cv::Mat energy16, energy32;

    // These matrices should always be of the correct size,
    // but only hold valid data when it is being used to find a seam.
    cv::Mat summed_h, summed_v;
};

#endif
