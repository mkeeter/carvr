#ifndef SEAM_H
#define SEAM_H

#include <list>
#include <opencv2/opencv.hpp>

typedef std::list<int> Seam;

Seam GetSeam(const cv::Mat& summed);
cv::Mat DrawSeam(const cv::Mat& in, const Seam& seam);
void RemoveSeam(cv::Mat& in, const Seam& seam);

#endif
