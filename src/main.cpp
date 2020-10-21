
#include <iostream>
#include <processing/laplace_pyramid.hpp>
#include <processing/filter.hpp>
#include <opencv2/opencv.hpp>

using proc::LaplacePyramid;
using proc::Filter;

int main(int argc, char** argv) {
    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    cv::VideoCapture cap("resources/test.mp4");

    // Check if camera opened successfully
    if(!cap.isOpened()){
        return -1;
    }

    Filter filter{0.4, 2};

    while(true){

        Mat frame;
        // Capture frame-by-frame
        cap >> frame;
        frame.convertTo(frame, CV_32FC3);

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        LaplacePyramid pyr(frame, 4);

        LaplacePyramid amplified = (pyr * filter * vector<double>{50, 50, 50, 1}).norm(255);

        LaplacePyramid sum = pyr + amplified;

        /*for (int i = 0; i < pyr.levels(); i++) {
            frame = pyr.at(i);
            frame.convertTo(frame, CV_8UC3);

            cv::imshow("Video", frame);
            cv::waitKey(0);
        }*/

        frame = sum.reconstruct();
        frame.convertTo(frame, CV_8UC3);

        cv::imshow("Video", frame);
        cv::waitKey(1);
    }

    // When everything done, release the video capture object
    cap.release();

    return 0;
}