#include "calibration.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

void calibration() {
    cv::Size boardSize(7, 7);
    float squareSize = 1;

    std::vector<std::vector<cv::Point2f>> imagePoints;

    std::vector<std::vector<cv::Point3f>> objectPoints(1);

    for (int i = 0; i < boardSize.height; ++i) {
        for (int j = 0; j < boardSize.width; ++j) {
            objectPoints[0].push_back(cv::Point3f(j * squareSize, i * squareSize, 0));
        }
    }

    std::vector<cv::Mat> images;
    images.push_back(cv::imread("1.jpg"));
    images.push_back(cv::imread("2.jpg"));

    std::vector<cv::Point2f> corners;

    for (const auto &image: images) {
        bool found = cv::findChessboardCorners(image, boardSize, corners);

        if (found) {
            cv::cornerSubPix(image, corners, cv::Size(11, 11), cv::Size(-1, -1),
                             cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));

            imagePoints.push_back(corners);

//            cv::drawChessboardCorners(image, boardSize, corners, found);
//            cv::imshow("Corners", image);
//            cv::waitKey(500);
        }
    }

    cv::Mat cameraMatrix, distCoefficients;
    std::vector<cv::Mat> rvecs, tvecs;

    cv::calibrateCamera(objectPoints, imagePoints, images[0].size(), cameraMatrix, distCoefficients, rvecs, tvecs);

    cv::FileStorage fs("calibration.xml", cv::FileStorage::WRITE);
    fs << "cameraMatrix" << cameraMatrix;
    fs << "distortionCoefficients" << distCoefficients;
    fs.release();
}
