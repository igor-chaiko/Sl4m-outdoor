#include <opencv2/opencv.hpp>
#include "triangulation.h"

// Function to extract keypoints and descriptors using ORB
void extractKeyPointsAndDescriptors(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors) {
    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    orb->detectAndCompute(image, cv::noArray(), keypoints, descriptors);
}

// Function to perform feature matching using the BFMatcher
void performFeatureMatching(const cv::Mat& descriptors1, const cv::Mat& descriptors2,
                            std::vector<cv::DMatch>& goodMatches, double ratioThreshold = 0.7) {
    cv::BFMatcher bfMatcher(cv::NORM_HAMMING);
    std::vector<std::vector<cv::DMatch>> matches;
    bfMatcher.knnMatch(descriptors1, descriptors2, matches, 2);

    for (auto& match : matches) {
        if (match[0].distance < ratioThreshold * match[1].distance) {
            goodMatches.push_back(match[0]);
        }
    }
}

// Function to draw and display matches
void drawAndDisplayMatches(const cv::Mat& firstFrame, const cv::Mat& secondFrame,
                           const std::vector<cv::KeyPoint>& keyPoints1, const std::vector<cv::KeyPoint>& keyPoints2,
                           const std::vector<cv::DMatch>& goodMatches) {
    cv::Mat imgMatches;
    cv::drawMatches(firstFrame, keyPoints1, secondFrame, keyPoints2, goodMatches, imgMatches);
    cv::namedWindow("Matches", cv::WINDOW_NORMAL);
    cv::imshow("Matches", imgMatches);
    cv::waitKey(1);
}

// Function to perform camera pose estimation and triangulation
cv::Mat performTriangulation(const std::vector<cv::Point2d>& matchedPoints1, const std::vector<cv::Point2d>& matchedPoints2,
                             const cv::Mat& cameraMatrix, const cv::Mat& P1, cv::Mat& P2) {
    cv::Mat E = cv::findEssentialMat(matchedPoints1, matchedPoints2, cameraMatrix);

    cv::Mat R, t;
    cv::recoverPose(E, matchedPoints1, matchedPoints2, cameraMatrix, R, t);

    cv::Mat tmp;
    cv::hconcat(R, t, tmp);

    cv::Mat newRow = (cv::Mat_<double>(1, 4) << 0, 0, 0, 1);
    tmp.push_back(newRow);

    P2 = P1 * tmp;

    cv::Mat points4D;
    cv::triangulatePoints(P1, P2, matchedPoints1, matchedPoints2, points4D);
    cv::Mat points3D = cv::Mat::zeros(3, points4D.cols, CV_64F);

    for (int i = 0; i < points4D.cols; i++) {
        cv::Mat point = points4D.col(i);
        for (int j = 0; j < 3; j++) {
            points3D.at<double>(j, i) = point.at<double>(j) / point.at<double>(3);
        }
    }

    return points3D;
}

// Main triangulation function
cv::Mat triangulation(const cv::Mat& firstFrame, const cv::Mat& secondFrame, const cv::Mat& cameraMatrix,
                      const cv::Mat& P1, cv::Mat& P2) {
    cv::Mat firstGray, secondGray;
    cv::cvtColor(firstFrame, firstGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(secondFrame, secondGray, cv::COLOR_BGR2GRAY);

    std::vector<cv::KeyPoint> keyPoints1, keyPoints2;
    cv::Mat descriptors1, descriptors2;

    extractKeyPointsAndDescriptors(firstGray, keyPoints1, descriptors1);
    extractKeyPointsAndDescriptors(secondGray, keyPoints2, descriptors2);

    std::vector<cv::DMatch> goodMatches;
    performFeatureMatching(descriptors1, descriptors2, goodMatches);

    drawAndDisplayMatches(firstGray, secondGray, keyPoints1, keyPoints2, goodMatches);

    std::vector<cv::Point2d> matchedPoints1, matchedPoints2;
    for (const auto& match : goodMatches) {
        matchedPoints1.push_back(keyPoints1[match.queryIdx].pt);
        matchedPoints2.push_back(keyPoints2[match.trainIdx].pt);
    }

    return performTriangulation(matchedPoints1, matchedPoints2, cameraMatrix, P1, P2);
}
