#ifndef ENERGY_H
#define ENERGY_H

#include <opencv2/opencv.hpp>

void GetVerticalEnergy(const cv::Mat& energy, cv::Mat& summed);
void GetHorizontalEnergy(const cv::Mat& energy, cv::Mat& summed);

#endif
