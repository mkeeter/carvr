#ifndef SEAM_H
#define SEAM_H

#include <list>
#include <opencv2/opencv.hpp>

typedef std::list<int> Seam;

Seam GetVerticalSeam(const cv::Mat& summed);
Seam GetHorizontalSeam(const cv::Mat& summed);
cv::Mat DrawVerticalSeam(const cv::Mat& in, const Seam& seam);
cv::Mat RemoveVerticalSeam(const cv::Mat& in, const Seam& seam);

#endif