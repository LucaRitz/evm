#include "../../include/evm/impl/roi_face_capture.hpp"

#include <opencv2/opencv.hpp>

evm::RoiFaceCapture::RoiFaceCapture():
    _faceDetector("resources/haarcascade_frontalface_alt2.xml"),
    _facemark(cv::face::FacemarkLBF::create()) {
    _facemark->loadModel("resources/lbfmodel.yaml");
}

evm::Roi evm::RoiFaceCapture::roi(const Mat& frame) {
    Mat gray;
    cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    gray.convertTo(gray, CV_8UC1);

    vector<cv::Rect> faces;
    int min = (int)(frame.rows*0.4f); // the bigger min the faster
    int max = (int)(frame.rows*0.8f); // the smaller max the faster
    cv::Size minSize(min, min);
    cv::Size maxSize(max, max);
    _faceDetector.detectMultiScale(gray, faces, 1.1, 3, 0, minSize, maxSize);

    if (faces.empty()) {
        return Roi{Mat{}};
    }

    Mat roi = frame(faces[0]);
    return Roi{roi, Position{faces[0].x, faces[0].y}};
}

evm::RoiReconstructor& evm::RoiFaceCapture::getReconstructor() {
    return _roiReconstructor;
}