#include "../include/processing/laplace_pyramid.hpp"

#include "../include/processing/filter.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

proc::LaplacePyramid::LaplacePyramid(cv::Mat& image, t_level levels) {

    vector<Mat> gaussian_pyramid;
    gaussian_pyramid.push_back(image);

    Mat& previousWorkingImage = image;
    for (int i = 1; i < levels; i++) {
        Mat currentImage;
        cv::pyrDown(previousWorkingImage, currentImage,
                    cv::Size(previousWorkingImage.cols / 2, previousWorkingImage.rows / 2));
        gaussian_pyramid.push_back(currentImage);
        previousWorkingImage = currentImage;
    }

    _pyramid.push_back(gaussian_pyramid.at(gaussian_pyramid.size() - 1));

    previousWorkingImage = gaussian_pyramid.at(gaussian_pyramid.size() - 1);
    for (int i = gaussian_pyramid.size() - 2; i >= 0; i--) {
        Mat currentImage;
        cv::pyrUp(previousWorkingImage, currentImage, gaussian_pyramid.at(i).size());

        Mat test = gaussian_pyramid.at(i) - currentImage;
        cv::cvtColor(test, currentImage, cv::COLOR_BGR2GRAY); // Convert the image to grayscale

        _pyramid.push_back(currentImage);
        previousWorkingImage = gaussian_pyramid.at(i);
    }
}

Mat proc::LaplacePyramid::reconstruct() {
    if (_pyramid.empty()) {
        return Mat{};
    }

    auto collapsed = _pyramid.at(0);

    for (int i = 1; i < _pyramid.size(); i++) {
        auto current = _pyramid.at(i);
        cv::pyrUp(collapsed, collapsed, current.size());

        cv::cvtColor(current, current, cv::COLOR_GRAY2RGB);
        collapsed += current;
    }

    return collapsed;
}

Mat proc::LaplacePyramid::at(t_level level) {
    if (level < 0 || level > levels() - 1) {
        return Mat{};
    }
    return _pyramid.at(level);
}

t_level proc::LaplacePyramid::levels() {
   return _pyramid.size();
}

proc::LaplacePyramid proc::LaplacePyramid::norm(int max) {
    LaplacePyramid result;
    for (int i = 0; i < levels(); i++) {
        double min, currentMax;
        cv::minMaxLoc(_pyramid.at(i), &min, &currentMax);
        double factor = currentMax / max;
        result._pyramid.push_back(_pyramid.at(i) / factor);
    }
    return result;
}

proc::LaplacePyramid proc::LaplacePyramid::operator+(const LaplacePyramid &other) {
    LaplacePyramid result;

    if (this->_pyramid.size() != other._pyramid.size()) {
        return result;
    }

    for (int i = 0; i < levels(); i++) {
        Mat combined = _pyramid.at(i) + other._pyramid.at(i);
        result._pyramid.push_back(combined);
    }

    return result;
}

proc::LaplacePyramid proc::LaplacePyramid::operator*(const vector<double>& factors) {
    LaplacePyramid result;

    if (factors.size() != _pyramid.size()) {
        return result;
    }

    for (int i = 0; i < _pyramid.size(); ++i) {
        auto amplified = _pyramid.at(i) * factors.at(i);
        result._pyramid.push_back(amplified);
    }

    return result;
}

proc::LaplacePyramid proc::LaplacePyramid::operator*(Filter& filter) {
    LaplacePyramid result;

    for (const auto & frame : _pyramid) {
        auto filtered = filter * frame;
        result._pyramid.push_back(filtered);
    }

    return result;
}