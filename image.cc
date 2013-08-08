#include "image.h"

#include "energy.h"
#include "seam.h"

Image::Image(std::string filename)
    : filename(filename), img(cv::imread(filename)), transposed(false)
{
    cv::cvtColor(img, bw, CV_BGR2GRAY);
    energy16   = cv::Mat(img.rows, img.cols, CV_16U);
    summed     = cv::Mat(img.rows, img.cols, CV_32S);
    RecalculateEnergy();
}

////////////////////////////////////////////////////////////////////////////////

void Image::TransposeMatrices()
{
    cv::Mat* matrices[] = {&img, &bw, &sobel_h, &sobel_v,
                           &energy16, &energy32, &summed};
    for (int i=0; i < 7; ++i) {
        cv::transpose(*matrices[i], *matrices[i]);
    }

    transposed = !transposed;
}

////////////////////////////////////////////////////////////////////////////////

wxBitmap Image::GetBitmap() const
{
    if (img.empty())    return wxBitmap();

    // Generate the bitmap if it doesn't exist or is of an incorrect size
    uint8_t* const img_data = (uint8_t*)malloc(img.rows*img.cols*3);

    int a=0;
    if (transposed) {
        for (int i=0; i < img.cols; ++i) {
            for (int j=0; j < img.rows; ++j) {
                cv::Vec3b pixel = img.at<cv::Vec3b>(j, i);
                img_data[a++] = pixel[2];
                img_data[a++] = pixel[1];
                img_data[a++] = pixel[0];
            }
        }
    } else {
        for (int j=0; j < img.rows; ++j) {
            for (int i=0; i < img.cols; ++i) {
                cv::Vec3b pixel = img.at<cv::Vec3b>(j, i);
                img_data[a++] = pixel[2];
                img_data[a++] = pixel[1];
                img_data[a++] = pixel[0];
            }
        }
    }

    wxImage wx_image(transposed ? img.rows : img.cols,
                     transposed ? img.cols : img.rows, img_data);
    return wxBitmap(wx_image);
}

////////////////////////////////////////////////////////////////////////////////

void Image::RecalculateEnergy()
{
    cv::Sobel(bw, sobel_h, CV_16U, 1, 0);
    cv::Sobel(bw, sobel_v, CV_16U, 0, 1);

    energy16.setTo(0);
    energy16 += sobel_h;
    energy16 += sobel_v;

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
    if (!transposed)    TransposeMatrices();
    RemoveSeam();
}

////////////////////////////////////////////////////////////////////////////////

void Image::RemoveVerticalSeam()
{
    if (transposed)     TransposeMatrices();
    RemoveSeam();
}

////////////////////////////////////////////////////////////////////////////////

void Image::RemoveSeam()
{
    GetEnergy(energy32, summed);
    Seam seam = GetSeam(summed);

    ::RemoveSeam(img, seam);
    ::RemoveSeam(bw,  seam);

    ChangeImageSizes(cv::Range::all(), cv::Range(0, img.cols-1));
    RecalculateEnergy();
}


