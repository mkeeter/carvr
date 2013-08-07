#include "image.h"

#include "energy.h"
#include "seam.h"

Image::Image(std::string filename)
    : filename(filename), img(cv::imread(filename))
{
    cv::cvtColor(img, bw, CV_BGR2GRAY);
    RecalculateEnergy();

    summed_h = cv::Mat(img.rows, img.cols, CV_32S);
    summed_v = cv::Mat(img.rows, img.cols, CV_32S);
}

////////////////////////////////////////////////////////////////////////////////

wxBitmap Image::GetBitmap() const
{
    // Generate the bitmap if it doesn't exist or is of an incorrect size
    uint8_t* const img_data = (uint8_t*)malloc(img.rows*img.cols*3);
    int a=0;
    for (int j=0; j < img.rows; ++j) {
        for (int i=0; i < img.cols; ++i) {
            cv::Vec3b pixel = img.at<cv::Vec3b>(j, i);
            img_data[a++] = pixel[2];
            img_data[a++] = pixel[1];
            img_data[a++] = pixel[0];
        }
    }
    wxImage wx_image(img.cols, img.rows, img_data);
    return wxBitmap(wx_image);
}

////////////////////////////////////////////////////////////////////////////////

void Image::RecalculateEnergy()
{
    cv::Sobel(bw, sobel_h, CV_16S, 1, 0);
    cv::Sobel(bw, sobel_h, CV_16S, 0, 0);

    energy16 = cv::abs(sobel_h) + cv::abs(sobel_v);
    energy16.convertTo(energy32, CV_32S);
}

////////////////////////////////////////////////////////////////////////////////

void Image::ChangeImageSizes(const cv::Range r, const cv::Range c)
{
    img      = img(r, c);
    bw       = bw(r, c);
    sobel_h  = sobel_h(r, c);
    sobel_v  = sobel_v(r, c);
    energy16 = energy16(r, c);
    energy32 = energy32(r, c);
}

////////////////////////////////////////////////////////////////////////////////

void Image::RemoveHorizontalSeam()
{
    GetHorizontalEnergy(energy32, summed_h);
    Seam seam = GetHorizontalSeam(summed_h);

    cv::Range c(0, img.cols - 1);
    cv::Mat new_img = img.colRange(c);
    cv::Mat new_bw  = bw.colRange(c);
    ::RemoveHorizontalSeam(img, seam, new_img);
    ::RemoveHorizontalSeam(bw,  seam, new_bw);

    ChangeImageSizes(cv::Range::all(), c);
    RecalculateEnergy();
}

////////////////////////////////////////////////////////////////////////////////

void Image::RemoveVerticalSeam()
{
    GetVerticalEnergy(energy32, summed_v);
    Seam seam = GetVerticalSeam(summed_v);

    cv::Range r(0, img.rows - 1);
    cv::Mat new_img = img.rowRange(r);
    cv::Mat new_bw  = bw.rowRange(r);
    ::RemoveVerticalSeam(img, seam, new_img);
    ::RemoveVerticalSeam(bw,  seam, new_bw);

    ChangeImageSizes(r, cv::Range::all());
    RecalculateEnergy();
}

