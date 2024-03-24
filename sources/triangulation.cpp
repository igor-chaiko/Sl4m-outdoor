#include <opencv2/opencv.hpp>
#include "../headers/triangulation.h"

cv::Ptr<cv::ORB> orb = cv::ORB::create();
cv::BFMatcher bfMatcher(cv::NORM_HAMMING);

void extractKeyPointsAndDescriptors(const cv::Mat &image, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors) {
    orb->detectAndCompute(image, cv::noArray(), keypoints, descriptors);
}

void performFeatureMatching(const cv::Mat &descriptors1, const cv::Mat &descriptors2,
                            std::vector<cv::DMatch> &goodMatches) {
    std::vector<std::vector<cv::DMatch>> matches;
    bfMatcher.knnMatch(descriptors1, descriptors2, matches, 2);

    for (std::vector<cv::DMatch> &match: matches) {
        if (match[0].distance < DEFAULT_THRESHOLD * match[1].distance) {
            goodMatches.push_back(match[0]);
        }
    }
}

void drawAndDisplayMatches(const cv::Mat &firstFrame, const cv::Mat &secondFrame,
                           const std::vector<cv::KeyPoint> &keyPoints1, const std::vector<cv::KeyPoint> &keyPoints2,
                           const std::vector<cv::DMatch> &goodMatches) {
    cv::Mat imgMatches;
    cv::drawMatches(firstFrame, keyPoints1, secondFrame, keyPoints2, goodMatches, imgMatches);
    cv::namedWindow("Matches", cv::WINDOW_NORMAL);
    cv::imshow("Matches", imgMatches);
    cv::waitKey(1);
}

void normalizeCoordinates4to3(cv::Mat &matrix) {
    for (int i = 0; i < matrix.cols; i++) {
        matrix.col(i) = matrix.col(i) / matrix.at<double>(3, i);
    }
    matrix = matrix.rowRange(0, matrix.rows - 1);
}

void matrixAddVector(cv::Mat &matrix, const cv::Mat &vector) {
    for (int col = 0; col < matrix.cols; ++col) {
        matrix.col(col) += vector;
    }
}

void filterTriangulatedPoints(cv::Mat &points) {
    cv::Mat result(3, 0, CV_64F);

    for (int j = 0; j < points.cols; ++j) {
        if (abs(points.at<double>(0, j)) < MAX_FEATURE_DISTANCE
            & abs(points.at<double>(1, j)) < MAX_FEATURE_DISTANCE
            & abs(points.at<double>(2, j)) < MAX_FEATURE_DISTANCE
            & points.at<double>(2, j) > 0) {
            cv::Mat newCol = points.col(j);
            newCol.at<double>(2,0) = -newCol.at<double>(2,0);
            hconcat(result, newCol, result);

        }
    }

    points = result;
}

cv::Mat
performTriangulation(const std::vector<cv::Point2d> &matchedPoints1, const std::vector<cv::Point2d> &matchedPoints2,
                     const cv::Mat &cameraMatrix, const cv::Mat &P1, cv::Mat &P2) {
    cv::Mat E = cv::findEssentialMat(matchedPoints1, matchedPoints2, cameraMatrix);

    cv::Mat R, t, triangulatedPoints;
    cv::recoverPose(E, matchedPoints1, matchedPoints2, cameraMatrix, R, t, DISTANCE_THRESH, cv::noArray(),
                    triangulatedPoints);

    cv::Mat tmp;
    cv::hconcat(R, t, tmp);
    cv::Mat globalMovement = R * t;

    cv::Mat newRow = (cv::Mat_<double>(1, 4) << 0, 0, 0, 1);
    tmp.push_back(newRow);
    P2 = P1 * tmp;

    normalizeCoordinates4to3(triangulatedPoints);

    filterTriangulatedPoints(triangulatedPoints);

    cv::Mat R1 = P1(cv::Rect(0, 0, 3, 3));
    cv::Mat t1 = P1(cv::Rect(3, 0, 1, 3));

    cv::Mat globalTP(3, 0, CV_64F);

    if (!triangulatedPoints.empty()) {
        globalTP = R1 * triangulatedPoints;

        matrixAddVector(globalTP, t1);

//        std::cout << P1 << std::endl;
//        std::cout << globalTP << std::endl;
    }

    return globalTP;
}

cv::Mat triangulation(const cv::Mat &firstFrame, const cv::Mat &secondFrame, const cv::Mat &cameraMatrix,
                      const cv::Mat &P1, cv::Mat &P2) {
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
    for (const cv::DMatch &match: goodMatches) {
        matchedPoints1.push_back(keyPoints1[match.queryIdx].pt);
        matchedPoints2.push_back(keyPoints2[match.trainIdx].pt);
    }

    return performTriangulation(matchedPoints1, matchedPoints2, cameraMatrix, P1, P2);
}