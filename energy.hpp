#ifndef ENERGY_H
#define ENERGY_H

#include <opencv2/opencv.hpp>

cv::Mat GetEnergy(const cv::Mat& orig);
cv::Mat GetVerticalEnergy(const cv::Mat& energy);
cv::Mat GetHorizontalEnergy(const cv::Mat& energy);

#endif
