#include "../include/processing/filter.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>

proc::Filter::Filter(double lowerBound, double upperBound, int sampleRate) : _lowerBound(lowerBound), _upperBound(upperBound), _sampleRate(sampleRate) {

}

Mat proc::Filter::operator*(Mat toFilter) {
    //Mat filter = findAndCreateFilterIfNotExist(toFilter);
    std::cout << "split filter" << std::endl;

    //vector<Mat> channelsToFilter = splitByChannel(toFilter);
    Mat * channelsToFilter = new Mat[toFilter.channels()];
    cv::split(toFilter, channelsToFilter);
    std::cout << "split" << std::endl;
    vector<Mat> filteredChannels;

    for(int i = 0; i < toFilter.channels(); i++) {
        Mat channelToFilter = channelsToFilter[i];


        int width = channelToFilter.cols;
        int height = cv::getOptimalDFTSize(channelToFilter.rows);

        Mat tempImg;
        copyMakeBorder(channelToFilter, tempImg,
                       0, height - channelToFilter.rows,
                       0, width - channelToFilter.cols,
                       cv::BORDER_CONSTANT, cv::Scalar::all(0));

        cv::dft(tempImg, tempImg, cv::DFT_ROWS | cv::DFT_SCALE);

        Mat filter = tempImg.clone();
        buildFilterOfFrequencyBand(filter, _lowerBound, _upperBound, _sampleRate);

       /* Mat planes[] = {Mat::zeros(channelToFilter.rows, channelToFilter.cols, CV_32FC1),
                        Mat::zeros(channelToFilter.rows, channelToFilter.cols, CV_32FC1)};
        cv::split(channelToFilter, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
        Mat magn = Mat::zeros(channelToFilter.rows, channelToFilter.cols, CV_32FC1);
        cv::magnitude(planes[0], planes[1], magn);*/

        std::cout << "channel: " << tempImg.type() << "   filter: " << filter.type() << std::endl;
        cv::mulSpectrums(tempImg, filter, tempImg, cv::DFT_ROWS);

        std::cout << "mul ok" << std::endl;
        /*Mat filtered = filter.mul(magn);
        Mat filteredPlanes[] = {filtered, planes[1]};
        cv::merge(filteredPlanes, 2, filtered);*/

        cv::idft(tempImg, tempImg, cv::DFT_ROWS | cv::DFT_SCALE);
        std::cout << "oook" << std::endl;
        //filteredChannels.push_back(channelToFilter);
        tempImg(cv::Rect(0, 0, channelToFilter.cols, channelToFilter.rows)).copyTo(channelsToFilter[i]);
    }

    //Mat res = mergeChannels(filteredChannels);
    Mat res;
    cv::merge(channelsToFilter, toFilter.channels(), res);
    delete[] channelsToFilter;
    cv::normalize(res, res, 0, 1, cv::NORM_MINMAX);
    return res;
}

Mat proc::Filter::findAndCreateFilterIfNotExist(const Mat& toFilter) {
    auto key = std::make_pair(toFilter.rows, toFilter.cols);
    /*if (_filter.find(key) == _filter.end()) {
        _filter[key] = buildFilterOfFrequencyBand(toFilter.cols, toFilter.rows, _lowerBound, _upperBound, _sampleRate);
    }*/
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

Mat proc::Filter::buildFilterOfFrequencyBand(Mat& filter, double lowerBound, double upperBound, int sampleRate) {
    /*cv::Vec<float, 30> mask;
    for (int i = 0; i < mask.cols; i++) {
        float frequency = (i/(float)mask.cols)*sampleRate;
        mask[i] = frequency >= lowerBound && frequency <= upperBound ? 1.0f : 0.0f;
    }

    Mat filter = cv::Mat::zeros(height, width, CV_32FC1);
    std::cout << "Create filter "<< std::endl;
    cv::repeat(mask, height/30, width, filter);
    cv::merge(vector<Mat>{filter, filter}, filter);
    std::cout << "ok" << std::endl;*/

    // Calculate frequencies according to framerate and size
    double width = filter.cols;
    double height = filter.rows;

    double fl = 2 * lowerBound * width / sampleRate;
    double fh = 2 * upperBound * width / sampleRate;

    std::cout << "fl " << fl << "  fh "  << fh << std::endl;

    double response;

    //Mat filter = cv::Mat::zeros(height, width, CV_32FC2);
    // Create the filtermask, looks like the quarter of a circle
    for(int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if(x >= fl && x <= fh) {
                response = 1.0f;
                //std::cout << "1 at row " << y << " col " << x << std::endl;
            }
            else
                response = 0.0f;

            filter.at<float>(y,x) = response;
        }
    }


    /*Mat channels[2];
    split(filter,channels);
    Mat cop = channels[0];
    cop.convertTo(cop, CV_8UC1);
    threshold(cop, cop, 0, 255, cv::THRESH_BINARY);

    cv::imshow("filter", cop);
    cv::waitKey(0);*/

    /* for(int row = 0; row < height; row++) {
        for(int column = 0; column < width; column++) {
            *//*float value = 0;
            for(double f = 1; f <= 1; f += 1) {
                //value += sin(2 * CV_PI * (row + column));
            }*//*

            float value = std::sin(CV_2PI * (column/(float)width + row/(float)height));
            //std::cout << "value " << value << std::endl;
            filter.at<cv::Vec2f>(row, column) = cv::Vec2f(value, 0);
        }
    }*/

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
    //cv::dft(filter, filter);
    /*cv::split(filter, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    cv::magnitude(planes[0], planes[1], filter);

    cv::imshow("Filter", filter);*/
    /*Mat test;
    cv::split(filter, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    cv::magnitude(planes[0], planes[1], test);
    cv::log(test, test);
    //test.convertTo(test, CV_8UC1);
    //threshold(test, test, 0, 255, cv::THRESH_BINARY);
    //std::cout << "Test: " << (int) test.at<char>(4, 4) << std::endl;
    cv::imshow("Filter", test);
    cv::waitKey(0);*/

    return filter;
}