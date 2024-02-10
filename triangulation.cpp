#include <opencv2/opencv.hpp>
#include "triangulation.h"

std::vector<cv::KeyPoint> findIntersection(const std::vector<cv::KeyPoint>& keyPointsORB,
                                           std::vector<cv::Point2d>& corners) {
    std::vector<cv::KeyPoint> result;
    cv::Point2d empty(0, 0);

    for (const auto& keyPoint : keyPointsORB) {
        cv::Point2d orbPoint(keyPoint.pt.x, keyPoint.pt.y);

        for (int i = 0; i < corners.size(); i++) {
            if (cv::norm(corners[i] - orbPoint) < EPS) {
//                corners[i] = empty;
                result.push_back(keyPoint);
                break;
            }
        }
    }

    if (result.size() < MIN_SIZE) {
        return keyPointsORB;
    }

    return result;
}

cv::Mat triangulation(const cv::Mat &firstFrame, const cv::Mat &secondFrame, const cv::Mat &cameraMatrix,
                      const cv::Mat &P1, cv::Mat &P2) {
    cv::Mat firstGray, secondGray;
    cv::cvtColor(firstFrame, firstGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(secondFrame, secondGray, cv::COLOR_BGR2GRAY);

    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keyPoints1, keyPoints2;
    cv::Mat descriptors1, descriptors2;
    orb->detectAndCompute(firstGray, cv::noArray(), keyPoints1, descriptors1);
    orb->detectAndCompute(secondGray, cv::noArray(), keyPoints2, descriptors2);

    std::vector<cv::Point2d> corners1;
    cv::goodFeaturesToTrack(firstGray, corners1, 200, 0.03, 35);

    std::vector<cv::Point2d> corners2;
    cv::goodFeaturesToTrack(secondGray, corners2, 200, 0.03, 35);

//    keyPoints1 = findIntersection(keyPoints1, corners1);
//    keyPoints2 = findIntersection(keyPoints2, corners2);
    corners1.clear();
    corners2.clear();

    cv::BFMatcher bfMatcher(cv::NORM_HAMMING);

    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> good_Matches;
    bfMatcher.knnMatch(descriptors1, descriptors2, matches, 2);

    std::vector<cv::Point2d> matched_points2;
    std::vector<cv::Point2d> matched_points1;
    double ratio_threshold = 0.7;

    for (auto &match: matches) {
        if (match[0].distance < ratio_threshold * match[1].distance) {
            matched_points1.push_back(keyPoints1[match[0].queryIdx].pt);
            matched_points2.push_back(keyPoints2[match[0].trainIdx].pt);
            good_Matches.push_back(match[0]);
        }
    }

    //int num_best_matches = std::min(40, static_cast<int>(good_Matches.size()));
    //good_Matches.resize(num_best_matches);
    //matched_points1.resize(num_best_matches);
    //matched_points2.resize(num_best_matches);

    cv::Mat img_matches;
    cv::drawMatches(firstGray, keyPoints1, secondGray, keyPoints2, good_Matches, img_matches);
    cv::namedWindow("Matches", cv::WINDOW_NORMAL);
    cv::imshow("Matches", img_matches);
    cv::waitKey(1);


    cv::Mat E = cv::findEssentialMat(matched_points1, matched_points2, cameraMatrix);

    cv::Mat R, t;
    cv::recoverPose(E, matched_points1, matched_points2, cameraMatrix, R, t);

    cv::Mat tmp;
    cv::hconcat(R, t, tmp);

    cv::Mat newRow = (cv::Mat_<double>(1, 4) << 0, 0, 0, 1);
    tmp.push_back(newRow);

    P2 = P1 * tmp;

    cv::Mat points4D;
    cv::triangulatePoints(P1, P2, matched_points1, matched_points2, points4D);
    cv::Mat points3D = cv::Mat::zeros(3, points4D.cols, CV_64F);

    for (int i = 0; i < points4D.cols; i++) {
        cv::Mat point = points4D.col(i);
        for (int j = 0; j < 3; j++) {
            points3D.at<double>(j, i) = point.at<double>(j) / point.at<double>(3);
        }
    }

    return points3D;
}