#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>

int calibration() {
    cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001);

    cv::Mat objP = cv::Mat::zeros(5 * 7, 3, CV_32FC1);
    for (int i = 0; i < 35; ++i) {
        objP.at<float>(i, 0) = i % 7;
        objP.at<float>(i, 1) = i / 7;
    }

    std::vector<cv::Mat> objPoints;
    std::vector<std::vector<cv::Point2f>> imgPoints;

    std::vector<cv::String> images;
    cv::glob("../CalibImg/*.jpg", images);

    cv::Mat gray;

    for (const auto &image: images) {
        cv::Mat img = cv::imread(image);
        if (img.empty()) {
            continue;
        }

        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Point2f> corners;
        bool ret = cv::findChessboardCorners(gray, cv::Size(7, 5), corners,
                                             cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK);

        if (ret) {
            objPoints.push_back(objP);
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);
            imgPoints.push_back(corners);
        }
    }

    cv::Mat cameraMatrix, distCoefficients;
    std::vector<cv::Mat> rvecs, tvecs;

    cv::calibrateCamera(objPoints, imgPoints, gray.size(), cameraMatrix, distCoefficients, rvecs, tvecs);

    std::cout << "Camera Matrix:\n" << cameraMatrix << "\n\n";
    std::cout << "Distortion Coefficients:\n" << distCoefficients << "\n";

    std::ofstream calib("CalibratedCamera.txt");
    if (calib.is_open()) {
        std::cout << "Camera Matrix: [";
        for (int i = 0; i < cameraMatrix.rows; ++i) {
            for (int j = 0; j < cameraMatrix.cols; ++j) {
                calib << cameraMatrix.at<double>(i, j) << " ";
            }
        }
        calib << "\n";
        for (int i = 0; i < distCoefficients.rows; ++i) {
            for (int j = 0; j < distCoefficients.cols; ++j) {
                calib << distCoefficients.at<double>(i, j) << " ";
            }
        }
        calib << "\n";
        calib.close();
    } else {
        std::cerr << "Unable to open CalibratedCamera.txt for writing.\n";
        return -1;
    }

    return 0;
}

