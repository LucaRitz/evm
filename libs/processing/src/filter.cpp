#include "../include/processing/filter.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>

proc::Filter::Filter(double lowerBound, double upperBound) : _lowerBound(lowerBound), _upperBound(upperBound) {

}

Mat proc::Filter::operator*(Mat toFilter) {
    Mat filter = findAndCreateFilterIfNotExist(toFilter);
    vector<Mat> channelsToFilter = splitByChannel(toFilter);
    vector<Mat> filteredChannels;

    for(const auto channelToFilter : channelsToFilter) {
        cv::dft(channelToFilter, channelToFilter);

       /* Mat planes[] = {Mat::zeros(channelToFilter.rows, channelToFilter.cols, CV_32FC1),
                        Mat::zeros(channelToFilter.rows, channelToFilter.cols, CV_32FC1)};
        cv::split(channelToFilter, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
        Mat magn = Mat::zeros(channelToFilter.rows, channelToFilter.cols, CV_32FC1);
        cv::magnitude(planes[0], planes[1], magn);*/


        Mat filtered;
        cv::mulSpectrums(channelToFilter, filter, filtered, 0);

        /*Mat filtered = filter.mul(magn);
        Mat filteredPlanes[] = {filtered, planes[1]};
        cv::merge(filteredPlanes, 2, filtered);*/

        cv::dft(filtered, filtered, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

        filteredChannels.push_back(filtered);
    }


    Mat res = mergeChannels(filteredChannels);
    return res;
}

Mat proc::Filter::findAndCreateFilterIfNotExist(const Mat& toFilter) {
    auto key = std::make_pair(toFilter.rows, toFilter.cols);
    if (_filter.find(key) == _filter.end()) {
        _filter[key] = buildFilterOfFrequencyBand(toFilter.cols, toFilter.rows, _lowerBound, _upperBound);
    }
    return _filter.find(key)->second;
}

vector<Mat> proc::Filter::splitByChannel(const Mat& src) {
    vector<Mat> result;

    for(int i = 0; i < src.channels(); i++) {
        Mat singleChannel(src.rows, src.cols, CV_32FC1);

        Mat out[] = {singleChannel };

        // src[i] -> singleChannel[0]
        int from_to[] = { i,0 };
        mixChannels(&src, 1, out, 1, from_to, 1 );

        Mat planes[] = {singleChannel, Mat::zeros(singleChannel.size(), CV_32FC1)};
        Mat complexI;
        merge(planes, 2, complexI);
        result.push_back(complexI);
    }

    return result;
}

Mat proc::Filter::mergeChannels(const vector<Mat>& mats) {
    if (mats.size() == 1) {
        return mats.at(0);
    }

    Mat result(mats.at(0).rows, mats.at(0).cols, CV_32FC3);
    Mat* in = new Mat[mats.size()];

    for (int i = 0; i < mats.size(); i++) {
        in[i] = mats.at(i);
    }

    cv::merge(in, mats.size(), result);

    delete[] in;

    return result;
}

Mat proc::Filter::buildFilterOfFrequencyBand(int width, int height, double lowerBound, double upperBound) {
    Mat filter = cv::Mat::zeros(height, width, CV_32FC2);
    for(int row = 0; row < height; row++) {
        for(int column = 0; column < width; column++) {
            /*float value = 0;
            for(double f = 1; f <= 1; f += 1) {
                //value += sin(2 * CV_PI * (row + column));
            }*/

            float value = std::sin(CV_2PI * (column/(float)width + row/(float)height));
            //std::cout << "value " << value << std::endl;
            filter.at<cv::Vec2f>(row, column) = cv::Vec2f(value, 0);
        }
    }

    /*for(int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            double fx = 1;
            double fy= 1;
            double freq = sqrt(fx * fx + fy * fy);
            if (freq >= lowerBound && freq <= upperBound) {
                filter.at<cv::Vec2f>(row, column) = cv::Vec2f(1, 0);
            }
        }
    }*/


    Mat planes[] = {Mat::zeros(height, width, CV_32FC1), Mat::zeros(height, width, CV_32FC1)};
    cv::dft(filter, filter);
    /*cv::split(filter, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    cv::magnitude(planes[0], planes[1], filter);

    cv::imshow("Filter", filter);*/
    Mat test;
    cv::split(filter, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    cv::magnitude(planes[0], planes[1], test);
    cv::log(test, test);
    //test.convertTo(test, CV_8UC1);
    //threshold(test, test, 0, 255, cv::THRESH_BINARY);
    //std::cout << "Test: " << (int) test.at<char>(4, 4) << std::endl;
    cv::imshow("Filter", test);
    cv::waitKey(0);

    return filter;
}