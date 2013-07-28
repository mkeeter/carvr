#include <string>
#include <sstream>

#include <opencv2/opencv.hpp>

#include "energy.hpp"
#include "seam.hpp"

int main(int argc, const char* argv[])
{
    cv::Mat image = cv::imread(argv[1]);

    const cv::Size size = image.size();
    for (int i=0; i < size.width/8; ++i) {
        std::cout << "\r" << i << "/" << size.width/8 << std::flush;
        const cv::Mat energy = GetEnergy(image);
        const cv::Mat vert = GetVerticalEnergy(energy);
        const Seam seam = GetVerticalSeam(vert);
        image = RemoveVerticalSeam(image, seam);
    }
    cv::imwrite("carved.png", image);
    return 0;
}
