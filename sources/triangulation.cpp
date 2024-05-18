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
    cv::resizeWindow("Matches", 1000, 600);
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

cv::Mat choseCorrectPoints(cv::Mat &src, cv::Mat &mask) {
    int countCorrect = 0;
    for (int i = 0; i < mask.rows; i++) {
        countCorrect += mask.at<char>(i, 0);
    }

    cv::Mat globalTP(3, countCorrect, CV_64F);

    int curPos = 0;
    for (int i = 0; i < src.cols; i++) {
        if (mask.at<char>(i, 0)) {
            for (int j = 0; j < 3; j++) {
                globalTP.at<double>(j, curPos) = src.at<double>(j, i);
            }
            curPos++;
        }
        if (curPos == countCorrect) {
            break;
        }
    }

    return globalTP;
}

cv::Mat
performTriangulation(const std::vector<cv::Point2d> &matchedPoints1, const std::vector<cv::Point2d> &matchedPoints2,
                     const cv::Mat &cameraMatrix, const cv::Mat &P1, cv::Mat &P2) {
    cv::Mat R, t, triangulatedPoints, mask;

    cv::Mat E = cv::findEssentialMat(matchedPoints1, matchedPoints2, cameraMatrix,
                                     cv::RANSAC, 0.999, 1.0, 1000, mask);

    cv::recoverPose(E, matchedPoints1, matchedPoints2, cameraMatrix, R, t,
                    DISTANCE_THRESH, mask);

//    cv::Mat tmp;
//    cv::hconcat(R, t, tmp);
//    cv::Mat newRow = (cv::Mat_<double>(1, 4) << 0, 0, 0, 1);
//    tmp.push_back(newRow);
//    P2 = P1 * tmp;
    P2.colRange(0, 3) = R * P1.colRange(0, 3);
    P2.col(3) = P1.col(3) + P1.colRange(0, 3) * t;

    cv::triangulatePoints(cameraMatrix * P1, cameraMatrix * P2, matchedPoints1, matchedPoints2, triangulatedPoints);

    normalizeCoordinates4to3(triangulatedPoints);

    cv::Mat globalTP = choseCorrectPoints(triangulatedPoints, mask);

    cv::Mat R1 = P1.colRange(0, 3);
    cv::Mat t1 = P1.col(3);
    if (!globalTP.empty()) {
        globalTP = R1 * globalTP;
        matrixAddVector(globalTP, t1);
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

    drawAndDisplayMatches(firstFrame, secondFrame, keyPoints1, keyPoints2, goodMatches);

    std::vector<cv::Point2d> matchedPoints1, matchedPoints2;
    for (const cv::DMatch &match: goodMatches) {
        matchedPoints1.push_back(keyPoints1[match.queryIdx].pt);
        matchedPoints2.push_back(keyPoints2[match.trainIdx].pt);
    }

    cv::Mat points = performTriangulation(matchedPoints1, matchedPoints2, cameraMatrix, P1, P2);

//    for (auto i: matchedPoints1) {
//        circle(firstFrame, i, 3, cv::Scalar(0, 255, 0), -1);
//    }
//    imshow("Image with Points", firstFrame);
//    //cv::waitKey(0);
    return points;
}

void findSolutionByDecomposing(cv::Mat &cameraMatrix, cv::Mat &essentialMatrix,
                               cv::Mat &rOut, cv::Mat& tOut,
                               cv::Mat &triangulatedPoints) {
    cv::Mat R1, R2, t;

    cv::decomposeEssentialMat(essentialMatrix, R1, R2, t);



};
