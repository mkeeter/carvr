#include <string>

#include "image.h"

#include "energy.h"
#include "seam.h"

////////////////////////////////////////////////////////////////////////////////

Image::Image(std::string filename)
    : filename(filename), img(cv::imread(filename)), transposed(false)
{
    cv::cvtColor(img, bw, CV_BGR2GRAY);
    energy16   = cv::Mat(img.rows, img.cols, CV_16S);
    tmp16      = cv::Mat(img.rows, img.cols, CV_16S);
    tmp32      = cv::Mat(img.rows, img.cols, CV_32S);
    RecalculateEnergy();
}

////////////////////////////////////////////////////////////////////////////////

int Image::Width() const
{
    return transposed ? img.rows : img.cols;
}

////////////////////////////////////////////////////////////////////////////////

int Image::Height() const
{
    return transposed ? img.cols : img.rows;
}

////////////////////////////////////////////////////////////////////////////////

std::string Image::Filename() const
{
    return filename;
}


////////////////////////////////////////////////////////////////////////////////

std::string Image::Extension() const
{
    return filename;
}

////////////////////////////////////////////////////////////////////////////////

void Image::Save(std::string filename) const
{
    if (transposed) {
        cv::imwrite(filename, img.t());
    } else {
        cv::imwrite(filename, img);
    }
}

////////////////////////////////////////////////////////////////////////////////

void Image::TransposeMatrices()
{
    cv::Mat* matrices[] = {&img, &bw, &tmp16, &tmp32, &energy16, &energy32};
    for (int i=0; i < 6; ++i) {
        cv::transpose(*matrices[i], *matrices[i]);
    }

    transposed = !transposed;
}

////////////////////////////////////////////////////////////////////////////////

void Image::RemoveOneColumn()
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

    cv::Sobel(bw, tmp16, CV_16S, 1, 0);
    cv::absdiff(tmp16, cv::Scalar::all(0), tmp16);
    energy16 += tmp16;

    cv::Sobel(bw, tmp16, CV_16S, 0, 1);
    cv::absdiff(tmp16, cv::Scalar::all(0), tmp16);
    energy16 += tmp16;

    energy16.convertTo(energy32, CV_32S);
}

////////////////////////////////////////////////////////////////////////////////

void Image::RecalculateEnergyBlock(cv::Range rows, cv::Range cols)
{
    cv::Rect large_roi(
        std::max(cols.start - 6, 0), std::max(rows.start - 6, 0),
        std::min(cols.end + 6, img.cols) - std::max(cols.start - 6, 0),
        std::min(rows.end + 6, img.rows) - std::max(rows.start - 6, 0));

    cv::Rect medium_roi(
        std::max(cols.start - 4, 0), std::max(rows.start - 4, 0),
        std::min(cols.end + 4, img.cols) - std::max(cols.start - 4, 0),
        std::min(rows.end + 4, img.rows) - std::max(rows.start - 4, 0));

    energy16(medium_roi).setTo(0);

    cv::Sobel(bw(large_roi), tmp16(large_roi), CV_16S, 1, 0);
    cv::absdiff(tmp16(large_roi), cv::Scalar::all(0), tmp16(large_roi));
    energy16(medium_roi) += tmp16(medium_roi);

    cv::Sobel(bw(large_roi), tmp16(large_roi), CV_16S, 0, 1);
    cv::absdiff(tmp16(large_roi), cv::Scalar::all(0), tmp16(large_roi));
    energy16(medium_roi) += tmp16(medium_roi);

    energy16(medium_roi).convertTo(energy32(medium_roi), CV_32S);
}

////////////////////////////////////////////////////////////////////////////////

void Image::RecalculateSeamEnergy(const Seam& seam)
{
    Seam::const_iterator itr = seam.begin();
    cv::Range rows(0, 0);
    cv::Range cols = cv::Range(img.cols, 0);

    for (int r=0; r < img.rows; ++r, ++itr)
    {
        cols.start = std::min(cols.start, *itr);
        cols.end   = std::max(cols.end,   *itr);

        if (r != 0 && (r % 40 == 0 || r == img.rows - 1))
        {
            rows.end = r + 1;
            RecalculateEnergyBlock(rows, cols);
            rows.start = rows.end;
            cols = cv::Range(img.cols, 0);
        }
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

    RemoveOneColumn();
    RecalculateSeamEnergy(seam);
}

////////////////////////////////////////////////////////////////////////////////

Image* Image::Clone() const
{
    Image* c = new Image();

    // Copy over all of the things, making clones of the cv::Mat objects
    c->filename = filename;
    c->img      = img.clone();
    c->bw       = bw.clone();
    c->energy16 = energy16.clone();
    c->energy32 = energy32.clone();
    c->tmp16    = tmp16.clone();
    c->tmp32    = tmp32.clone();
    c->transposed = transposed;

    return c;
}

