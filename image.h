#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include <opencv2/opencv.hpp>
#include <wx/wx.h>

#include "seam.h"

class Image
{
public:
    Image() {};
    Image(std::string filename);

    int Width() const;
    int Height() const;
    void Save(std::string filename) const;
    void RemoveHorizontalSeam();
    void RemoveVerticalSeam();
    Image* Clone() const;
    wxBitmap GetBitmap() const;

private:
    void RecalculateEnergy();
    void RecalculateEnergyBlock(cv::Range rows, cv::Range cols);
    void RecalculateBackColumnEnergy();
    void RecalculateSeamEnergy(const Seam& seam);
    void RemoveSeam();
    void TransposeMatrices();
    void RemoveOneColumn();

    // Name of the original image
    std::string filename;

    // At the end of every function, these matrices should be
    // of the correct size and storing correct data.
    cv::Mat img, bw;
    cv::Mat energy16, energy32;

    // These matrices should always be of the correct size,
    // but only hold valid data when it is being used to find a seam.
    cv::Mat tmp16, tmp32;

    // Keep track of whether we're storing the original or a transposed image
    bool transposed;
};

#endif
