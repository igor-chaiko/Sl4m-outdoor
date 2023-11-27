#include <opencv2/opencv.hpp>
#include "triangulation.h"

void triangulation(const cv::Mat &firstFrame, const cv::Mat &secondFrame, cv::Mat cameraMatrix, const cv::Mat &P1,
                   const cv::Mat &P2, cv::Mat points3D) {
    cv::Mat firstGray, secondGray;
    cv::cvtColor(firstFrame, firstGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(secondFrame, secondGray, cv::COLOR_BGR2GRAY);

    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keyPoints1, keyPoints2;
    cv::Mat descriptors1, descriptors2;
    orb->detectAndCompute(firstGray, cv::noArray(), keyPoints1, descriptors1);
    orb->detectAndCompute(secondGray, cv::noArray(), keyPoints2, descriptors2);

    cv::BFMatcher bfMatcher(cv::NORM_HAMMING);

    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> good_Matches;
    bfMatcher.knnMatch(descriptors1, descriptors2, matches, 2);

    std::vector<cv::Point2d> matched_points2;
    std::vector<cv::Point2d> matched_points1;
    double ratio_threshold = 0.8;

    for (auto & match : matches) {
        if (match[0].distance < ratio_threshold * match[1].distance) {
            matched_points1.push_back(keyPoints1[match[0].queryIdx].pt);
            matched_points2.push_back(keyPoints2[match[0].trainIdx].pt);
            good_Matches.push_back(match[0]);
        }
    }

    cv::Mat mask;
    cv::Mat E = cv::findEssentialMat(matched_points1, matched_points2, cameraMatrix.at<double>(0, 0),
                                     cv::Point2d(cameraMatrix.at<double>(0, 2), cameraMatrix.at<double>(1, 2)),
                                     cv::RANSAC, 0.999, 1.0, mask);

    cv::Mat R, t;
    cv::recoverPose(E, matched_points1, matched_points2, cameraMatrix, R, t, mask);

    R.copyTo(P2(cv::Rect(0, 0, 3, 3)));
    t.copyTo(P2(cv::Rect(3, 0, 1, 3)));

    cv::Mat points4D;
    cv::triangulatePoints(P1, P2, matched_points1, matched_points2, points4D);

    for (int i = 0; i < points4D.cols; i++) {
        cv::Mat point = points4D.col(i);
        for (int j = 0; j < 3; j++) {
            points3D.at<double>(j, i) = point.at<double>(j) / point.at<double>(3);
        }
    }

    std::cout << points3D << std::endl;
}