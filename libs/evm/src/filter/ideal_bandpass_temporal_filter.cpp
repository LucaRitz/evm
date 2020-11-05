#include "../../include/evm/filter/ideal_bandpass_temporal_filter.hpp"
#include <opencv2/opencv.hpp>
#include <algorithm>

evm::IdealBandpassTemporalFilter::IdealBandpassTemporalFilter(double lowerFreq, double upperFreq, int level) :
        _lowerFreq(lowerFreq),
        _upperFreq(upperFreq),
        _level(level),
        _processedFrames(0),
        _totalFrames(0),
        _bufferSize(0) {

}

evm::TemporalFiltered evm::IdealBandpassTemporalFilter::operator()(const SpatialFiltered& spatialFiltered, double fps) {
    int optimalBufferSize = bufferSize(static_cast<int>(fps));
    vector<shared_ptr<Pyramid>> results;

    int newFrames = spatialFiltered._spatialFiltered.size();
    _bufferSize += _bufferSize >= optimalBufferSize ? 0 : std::min(newFrames,  optimalBufferSize - _bufferSize);
    _totalFrames += newFrames;

    if (_bufferSize >= optimalBufferSize) {
        int toProcess = _totalFrames - _processedFrames;
        //toProcess -= toProcess >= optimalBufferSize ? 0 : 1;
        _processedFrames += std::max(0, toProcess);

        for(int i = 0; i < toProcess; i++) {
            results.push_back(std::make_shared<Pyramid>());
        }
    }

    for (int level = 0; level < spatialFiltered._levels; level++) {

        if (_level != -1 && level != _level) {
            continue;
        }

        if (_filterMat.find(level) == _filterMat.end()) {
            _filterMat[level] = Mat{};
        }

        auto& filterMat = _filterMat.find(level)->second;

        int originalHeight = spatialFiltered._spatialFiltered.at(0)->at(level).size().height;
        concat(level, filterMat, optimalBufferSize, spatialFiltered._spatialFiltered);

        /*if (_level != -1 && level != _level) {
            insert(filterMat, originalHeight, results);
            continue;
        }*/

        Mat* channelsToFilter = new Mat[filterMat.channels()];
        cv::split(filterMat, channelsToFilter);
        vector<Mat> filteredChannels;

        for (int i = 0; i < filterMat.channels(); i++) {
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
            buildFilterOfFrequencyBand(filter, _lowerFreq, _upperFreq, fps);

            cv::mulSpectrums(tempImg, filter, tempImg, cv::DFT_ROWS);

            cv::idft(tempImg, tempImg, cv::DFT_ROWS | cv::DFT_SCALE);
            tempImg(cv::Rect(0, 0, channelToFilter.cols, channelToFilter.rows)).copyTo(channelsToFilter[i]);
        }

        Mat res;
        cv::merge(channelsToFilter, filterMat.channels(), res);
        delete[] channelsToFilter;
        cv::normalize(res, res, 0, 1, cv::NORM_MINMAX);

        insert(res, originalHeight, results);
    }

    return TemporalFiltered{results};
}

int evm::IdealBandpassTemporalFilter::bufferSize(int fps) {
    // Calculate number of images needed to represent 2 seconds of film material
    unsigned int round = (unsigned int) std::max(2*fps,16);
    round--;
    round |= round >> 1;
    round |= round >> 2;
    round |= round >> 4;
    round |= round >> 8;
    round |= round >> 16;
    round++;

    return round;
}

Mat evm::IdealBandpassTemporalFilter::concat(int level, Mat& dest, int maxSize, const vector<shared_ptr<Pyramid>>& pyramids) {
    for (const auto& pyr : pyramids) {
        Mat singleFrame = pyr->at(level);
        if (_originalSizes.levels() - 1 < level) {
            _originalSizes.push_back(singleFrame);
        }
        cv::resize(singleFrame, singleFrame, _originalSizes.at(level).size(), 0, 0, cv::INTER_LINEAR);
        singleFrame = singleFrame.reshape(singleFrame.channels(), singleFrame.cols * singleFrame.rows).clone();

        if (dest.cols == 0) {
            singleFrame.copyTo(dest);
        } else {
            hconcat(dest, singleFrame, dest);
        }

        if(dest.cols > maxSize && maxSize != 0) {
            dest.colRange(1, dest.cols).copyTo(dest);
        }
    }
    return dest;
}

Mat evm::IdealBandpassTemporalFilter::buildFilterOfFrequencyBand(Mat& filter, double lowerBound, double upperBound,
                                                                 double sampleRate) {
    double width = filter.cols;
    double height = filter.rows;

    double fl = 2 * lowerBound * width / sampleRate;
    double fh = 2 * upperBound * width / sampleRate;


    double response;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x >= fl && x <= fh) {
                response = 1.0f;
            } else
                response = 0.0f;

            filter.at<float>(y, x) = response;
        }
    }

    return filter;
}

void
evm::IdealBandpassTemporalFilter::insert(const Mat& allFrames, int originalHeight, vector<shared_ptr<Pyramid>>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        int column = allFrames.cols - vec.size() + i;
        Mat line = allFrames.col(column).clone();
        line = line.reshape(line.channels(), originalHeight).clone();
        auto& pyramid = vec.at(i);
        pyramid->push_back(line);
    }
}