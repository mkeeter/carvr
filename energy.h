#ifndef ENERGY_H
#define ENERGY_H

#include <opencv2/opencv.hpp>

void GetCumulativeEnergy(const cv::Mat& energy, cv::Mat& summed);

#endif
