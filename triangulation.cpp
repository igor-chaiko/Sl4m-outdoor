#include <opencv2/opencv.hpp>
#include "triangulation.h"

void triangulation(const cv::Mat &firstFrame, const cv::Mat &secondFrame, const cv::Mat &cameraMatrix, const cv::Mat &P1,
              cv::Mat &P2, cv::Mat points3D) {
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
    double ratio_threshold = 0.5;

    for (auto &match: matches) {
        if (match[0].distance < ratio_threshold * match[1].distance) {
            matched_points1.push_back(keyPoints1[match[0].queryIdx].pt);
            matched_points2.push_back(keyPoints2[match[0].trainIdx].pt);
            good_Matches.push_back(match[0]);
        }
    }

    int num_best_matches = std::min(13, static_cast<int>(good_Matches.size()));
    good_Matches.resize(num_best_matches);
    matched_points1.resize(num_best_matches);
    matched_points2.resize(num_best_matches);

    std::cout << matched_points1.size() << std::endl;

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

//    cv::Mat points4D;
//    cv::triangulatePoints(P1, P2, matched_points1, matched_points2, points4D);
//    points3D = cv::Mat::zeros(3, points4D.cols, CV_64F);
//
//    for (int i = 0; i < points4D.cols; i++) {
//        cv::Mat point = points4D.col(i);
//        for (int j = 0; j < 3; j++) {
//            points3D.at<double>(j, i) = point.at<double>(j) / point.at<double>(3);
//        }
//    }
}