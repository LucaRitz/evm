#include "../../include/evm/filter/ideal_bandpass_temporal_filter.hpp"
#include <opencv2/opencv.hpp>

evm::IdealBandpassTemporalFilter::IdealBandpassTemporalFilter(double lowerFreq, double upperFreq, int sampleRate):
    _lowerFreq(lowerFreq),
    _upperFreq(upperFreq),
    _sampleRate(sampleRate) {

}

evm::TemporalFiltered evm::IdealBandpassTemporalFilter::operator()(const SpatialFiltered& spatialFiltered) {
    vector<shared_ptr<Pyramid>> results;
    for(int i = 0; i < spatialFiltered._spatialFiltered.size(); i++) {
        results.push_back(std::make_shared<Pyramid>());
    }

    for(int level = 0; level < spatialFiltered._levels; level++) {
        Mat toFilter = concat(level, spatialFiltered._spatialFiltered);

        Mat* channelsToFilter = new Mat[toFilter.channels()];
        cv::split(toFilter, channelsToFilter);
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
            buildFilterOfFrequencyBand(filter, _lowerFreq, _upperFreq, _sampleRate);

            cv::mulSpectrums(tempImg, filter, tempImg, cv::DFT_ROWS);

            cv::idft(tempImg, tempImg, cv::DFT_ROWS | cv::DFT_SCALE);
            tempImg(cv::Rect(0, 0, channelToFilter.cols, channelToFilter.rows)).copyTo(channelsToFilter[i]);
        }

        Mat res;
        cv::merge(channelsToFilter, toFilter.channels(), res);
        delete[] channelsToFilter;
        cv::normalize(res, res, 0, 1, cv::NORM_MINMAX);

        insert(res, spatialFiltered._spatialFiltered.at(0)->at(level).size().height, results);
    }

    return TemporalFiltered{results};
}

Mat evm::IdealBandpassTemporalFilter::concat(int level, const vector<shared_ptr<Pyramid>>& pyramids) {
    Mat allFrames;
    for (const auto& pyr : pyramids) {
        Mat singleFrame = pyr->at(level);
        singleFrame = singleFrame.reshape(singleFrame.channels(), singleFrame.cols*singleFrame.rows).clone();
        if(allFrames.cols == 0) {
            singleFrame.copyTo(allFrames);
        } else {
            hconcat(allFrames,singleFrame,allFrames);
        }
    }

    return allFrames;
}

Mat evm::IdealBandpassTemporalFilter::buildFilterOfFrequencyBand(Mat& filter, double lowerBound, double upperBound, int sampleRate) {
    double width = filter.cols;
    double height = filter.rows;

    double fl = 2 * lowerBound * width / sampleRate;
    double fh = 2 * upperBound * width / sampleRate;


    double response;

    for(int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if(x >= fl && x <= fh) {
                response = 1.0f;
            }
            else
                response = 0.0f;

            filter.at<float>(y,x) = response;
        }
    }

    return filter;
}

void evm::IdealBandpassTemporalFilter::insert(const Mat& allFrames, int originalHeight, vector<shared_ptr<Pyramid>>& vec) {
    for(int i = 0; i < allFrames.cols; i++) {
        Mat line = allFrames.col(i).clone();
        line = line.reshape(line.channels(), originalHeight).clone();
        auto& pyramid = vec.at(i);
        pyramid->push_back(line);
    }
}