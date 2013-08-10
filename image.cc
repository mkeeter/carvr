#include <string>

#include "image.h"

#include "energy.h"
#include "seam.h"

Image::Image(std::string filename)
    : filename(filename), img(cv::imread(filename)), transposed(false)
{
    cv::cvtColor(img, bw, CV_BGR2GRAY);
    energy16   = cv::Mat(img.rows, img.cols, CV_16U);
    tmp16      = cv::Mat(img.rows, img.cols, CV_16U);
    tmp32      = cv::Mat(img.rows, img.cols, CV_32S);
    RecalculateEnergy();
}

////////////////////////////////////////////////////////////////////////////////

void Image::TransposeMatrices()
{
    cv::Mat* matrices[] = {&img, &bw, &tmp16, &tmp32, &energy16, &energy32};
    for (int i=0; i < 6; ++i)   cv::transpose(*matrices[i], *matrices[i]);

    transposed = !transposed;
}

////////////////////////////////////////////////////////////////////////////////

void Image::ResizeMatrices()
{
    cv::Mat* matrices[] = {&img, &bw, &tmp16, &tmp32, &energy16, &energy32};
    for (int i=0; i < 6; ++i) {
        *matrices[i] = matrices[i]->colRange(0, matrices[i]->cols-1);
    }
}

////////////////////////////////////////////////////////////////////////////////

wxBitmap Image::GetBitmap() const
{
    if (img.empty())    return wxBitmap();

    // Allocate an array of image data (it will be freed by wx)
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

    return wxBitmap(wxImage(transposed ? img.rows : img.cols,
                            transposed ? img.cols : img.rows, img_data));
}

////////////////////////////////////////////////////////////////////////////////

void Image::RecalculateEnergy()
{
    energy16.setTo(0);

    cv::Sobel(bw, tmp16, CV_16U, 1, 0);
    energy16 += tmp16;
    cv::Sobel(bw, tmp16, CV_16U, 0, 1);
    energy16 += tmp16;

    energy16.convertTo(energy32, CV_32S);
}

////////////////////////////////////////////////////////////////////////////////

void Image::RecalculateEnergyBlock(cv::Range rows, cv::Range cols)
{
    cv::Rect large_roi(
        std::max(cols.start - 4, 0), std::max(rows.start - 4, 0),
        std::min(cols.end + 4, img.cols) - std::max(cols.start - 4, 0),
        std::min(rows.end + 4, img.rows) - std::max(rows.start - 4, 0));

    cv::Rect medium_roi(
        std::max(cols.start - 2, 0), std::max(rows.start - 2, 0),
        std::min(cols.end + 2, img.cols) - std::max(cols.start - 2, 0),
        std::min(rows.end + 2, img.rows) - std::max(rows.start - 2, 0));

    energy16(medium_roi).setTo(0);
    cv::Sobel(bw(large_roi), tmp16(large_roi), CV_16U, 1, 0);
    energy16(medium_roi) += tmp16(medium_roi);
    cv::Sobel(bw(large_roi), tmp16(large_roi), CV_16U, 0, 1);
    energy16(medium_roi) += tmp16(medium_roi);

    energy16(medium_roi).convertTo(energy32(medium_roi), CV_32S);
}

////////////////////////////////////////////////////////////////////////////////

void Image::RecalculateSeamEnergy(const Seam& seam)
{
    cv::Range rows(0, 0);
    cv::Range cols(img.cols, 0);

    Seam::const_iterator itr = seam.begin();
    for (int r=0; r < img.rows; ++r, ++itr)
    {
        if (r && (r % 40) == 0)
        {
            rows.end = r;
            RecalculateEnergyBlock(rows, cols);
            rows.start = r;
            cols = cv::Range(img.cols, 0);
        }
        if (*itr < cols.start)  cols.start = *itr;
        if (*itr > cols.end)    cols.end   = *itr;
    }
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
    GetCumulativeEnergy(energy32, tmp32);
    Seam seam = GetSeam(tmp32);

    ::RemoveSeam(img, seam);
    ::RemoveSeam(bw,  seam);
    ::RemoveSeam(energy16, seam);

    ResizeMatrices();
    RecalculateSeamEnergy(seam);
}

