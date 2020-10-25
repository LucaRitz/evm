#include "laplace_pyramid.hpp"

#include <opencv2/opencv.hpp>

evm::LaplacePyramid::LaplacePyramid(const Mat& image, int levels) {
    vector<Mat> gaussian_pyramid;
    gaussian_pyramid.push_back(image);

    Mat clone = image.clone();
    Mat& previousWorkingImage = clone;
    for (int i = 1; i < levels; i++) {
        Mat currentImage;
        cv::pyrDown(previousWorkingImage, currentImage,
                    cv::Size(previousWorkingImage.cols / 2, previousWorkingImage.rows / 2));
        gaussian_pyramid.push_back(currentImage);
        previousWorkingImage = currentImage;
    }

    _levels.push_back(gaussian_pyramid.at(gaussian_pyramid.size() - 1));

    previousWorkingImage = gaussian_pyramid.at(gaussian_pyramid.size() - 1);
    for (int i = gaussian_pyramid.size() - 2; i >= 0; i--) {
        Mat currentImage;
        cv::pyrUp(previousWorkingImage, currentImage, gaussian_pyramid.at(i).size());

        Mat test = gaussian_pyramid.at(i) - currentImage;
        cv::cvtColor(test, currentImage, cv::COLOR_BGR2GRAY); // Convert the image to grayscale

        _levels.push_back(currentImage);
        previousWorkingImage = gaussian_pyramid.at(i);
    }
}