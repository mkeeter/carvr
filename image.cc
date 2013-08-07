#include "image.h"

#include "energy.h"
#include "seam.h"

Image::Image(std::string filename)
    : filename(filename), img(cv::imread(filename))
{
    cv::cvtColor(img, bw, CV_BGR2GRAY);
    RecalculateEnergy();
}

////////////////////////////////////////////////////////////////////////////////

wxBitmap Image::GetBitmap() const
{
    if (img.empty())    return wxBitmap();

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
    cv::Sobel(bw, sobel_v, CV_16S, 0, 1);

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

    ::RemoveHorizontalSeam(img, seam);
    ::RemoveHorizontalSeam(bw,  seam);

    ChangeImageSizes(cv::Range(0, img.rows-1), cv::Range::all());
    RecalculateEnergy();
}

////////////////////////////////////////////////////////////////////////////////

void Image::RemoveVerticalSeam()
{
    GetVerticalEnergy(energy32, summed_v);
    Seam seam = GetVerticalSeam(summed_v);

    ::RemoveVerticalSeam(img, seam);
    ::RemoveVerticalSeam(bw,  seam);

    ChangeImageSizes(cv::Range::all(), cv::Range(0, img.cols - 1));
    RecalculateEnergy();
}

