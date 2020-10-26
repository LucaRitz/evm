
#include <iostream>
#include <evm/capturing_pipeline.hpp>
#include <evm/impl/webcam_capture.hpp>
#include <evm/impl/video_capture.hpp>
#include <evm/impl/roi_default.hpp>
#include <evm/impl/roi_face_capture.hpp>
#include <evm/processor.hpp>
#include <evm/evm_pipeline.hpp>
#include <evm/display.hpp>
#include <evm/filter/laplace_spatial_filter.hpp>
#include <evm/filter/ideal_bandpass_temporal_filter.hpp>
#include <evm/filter/amplifier.hpp>
#include <evm/filter/reconstructor.hpp>
#include <evm/filter/level_reconstructor.hpp>

int main() {
    evm::LaplaceSpatialFilter spatialFilter(4);
    evm::IdealBandpassTemporalFilter temporalFilter(0.833, 1, 30);
    evm::Amplifier amplifier{std::vector<int>{100, 1, 1, 1}};
    //evm::Reconstructor reconstructor;
    evm::LevelReconstructor reconstructor{0};
    evm::EvmPipeline evmPipeline(&spatialFilter, &temporalFilter, &amplifier, &reconstructor);

    //evm::WebcamCapture videoCapture;
    evm::VideoCapture videoCapture{"resources/test_2.mp4"};
    evm::RoiFaceCapture roiCapture;
    evm::Display display(roiCapture.getReconstructor(), 30);
    evm::Processor processor(evmPipeline, display, 60);
    evm::CapturingPipeline capturingPipeline(videoCapture, roiCapture, processor);

    capturingPipeline.join();

    return 0;
}

/*
#include <processing/laplace_pyramid.hpp>
#include <processing/filter.hpp>
#include <opencv2/opencv.hpp>
#include <processing/queue_worker.hpp>

using proc::LaplacePyramid;
using proc::QueueWorker;
using proc::Filter;

int main(int argc, char** argv) {
    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    cv::VideoCapture cap("resources/test_2.mp4");
    //QueueWorker worker;

    // Check if camera opened successfully
    if(!cap.isOpened()){
        return -1;
    }

   */
/* cv::VideoCapture cap;

    if(!cap.open(0)) {
        return -1;
    }*//*


    vector<LaplacePyramid> pyramids;
    Filter filter{0.833, 1.0f, 30};
    Mat allFrames;
    vector<Mat> frames;

    while(true){

        Mat frame;
        // Capture frame-by-frame
        cap >> frame;
        frame.convertTo(frame, CV_32FC3);

        frames.push_back(frame);

        // If the frame is empty, break immediately
        if (frame.empty())
            break;
        LaplacePyramid pyr(frame, 4);
        Mat singleFrame = pyr.at(0);
        singleFrame = singleFrame.reshape(singleFrame.channels(), singleFrame.cols*singleFrame.rows).clone();
        if(allFrames.cols == 0) {
            singleFrame.copyTo(allFrames);
        } else {
            hconcat(allFrames,singleFrame,allFrames);
        }

        if (allFrames.cols == 120) {
            Mat amplified = (filter * allFrames * 100);

            for(int i = 0; i < allFrames.cols; i++) {
                frame = frames.at(i);
                Mat line = amplified.col(i).clone();
                line = line.reshape(line.channels(), pyr.at(0).size().height).clone();

                cv::resize(line, line, frame.size(), 0, 0, cv::INTER_LINEAR);
                Mat sum = frame + line;

                double min,max;
                minMaxLoc(sum, &min, &max);
                sum.convertTo(sum, CV_8UC3, 255.0/(max-min), -min * 255.0/(max-min));

                std::cout << "Display: " << std::endl;
                cv::imshow("Video", sum);
                cv::waitKey(33);
            }
            allFrames = Mat();
            frames.clear();
        }
    }
    // When everything done, release the video capture object
    cap.release();

    return 0;
}*/
