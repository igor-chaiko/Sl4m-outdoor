#include <opencv2/opencv.hpp>
#include "triangulation.h"

#define DEFAULT_THRESHOLD 0.85

int currentFrameNumber;
cv::Mat secondFrame;
int numOfFirstUnusedFromPool;

cv::Ptr<cv::ORB> orb = cv::ORB::create();
cv::BFMatcher bfMatcher(cv::NORM_HAMMING);

// Function to extract keypoints and descriptors using ORB
void extractKeyPointsAndDescriptors(const cv::Mat &image, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors) {
    orb->detectAndCompute(image, cv::noArray(), keypoints, descriptors);
}

// Function to perform feature matching using the BFMatcher
void performFeatureMatching(const cv::Mat &descriptors1, const cv::Mat &descriptors2,
                            std::vector<cv::DMatch> &goodMatches, double ratioThreshold = DEFAULT_THRESHOLD) {
    std::vector<std::vector<cv::DMatch>> matches;
    bfMatcher.knnMatch(descriptors1, descriptors2, matches, 2);

    for (std::vector<cv::DMatch> &match: matches) {
        if (match[0].distance < ratioThreshold * match[1].distance) {
            goodMatches.push_back(match[0]);
        }
    }
}

// Function to check number of matches. If less than MIN_MATCHES, take next
// if u delete function call, everything still will work
std::vector<cv::DMatch> goodMatchesCheck(std::vector<cv::DMatch> &oldMatches, std::vector<cv::Mat> &framePool,
                                         cv::Mat descriptors1,
                                         std::vector<cv::KeyPoint> keyPoints2) {
    std::vector<cv::DMatch> newMatches;
    cv::Mat descriptors2, secondGray;

    if (oldMatches.size() >= MIN_MATCHES) {
        numOfFirstUnusedFromPool = framePool.size();
        return oldMatches;
    }

    for (int i = framePool.size() - 2; i >= 0; i--) {
        secondFrame = framePool[i];
        cv::cvtColor(secondFrame, secondGray, cv::COLOR_BGR2GRAY);
        extractKeyPointsAndDescriptors(secondGray, keyPoints2, descriptors2);
        performFeatureMatching(descriptors1, descriptors2, newMatches);

        numOfFirstUnusedFromPool = i + 1;
        std::cout << i << std::endl;
        if (newMatches.size() >= MIN_MATCHES) {
            return newMatches;
        }
    }

    return oldMatches;
}

// Function to draw and display matches
void drawAndDisplayMatches(const cv::Mat &firstFrame, const cv::Mat &seconddFrame,
                           const std::vector<cv::KeyPoint> &keyPoints1, const std::vector<cv::KeyPoint> &keyPoints2,
                           const std::vector<cv::DMatch> &goodMatches) {
    cv::Mat imgMatches;
    cv::drawMatches(firstFrame, keyPoints1, seconddFrame, keyPoints2, goodMatches, imgMatches);
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

void matrixAddVector(cv::Mat &matrix, cv::Mat vector) {
    for (int col = 0; col < matrix.cols; ++col) {
        matrix.col(col) += vector;
    }
}


// Function to perform camera pose estimation and triangulation
cv::Mat
performTriangulation(const std::vector<cv::Point2d> &matchedPoints1, const std::vector<cv::Point2d> &matchedPoints2,
                     const cv::Mat &cameraMatrix, const cv::Mat &P1, cv::Mat &P2) {
    cv::Mat E = cv::findEssentialMat(matchedPoints1, matchedPoints2, cameraMatrix);

    cv::Mat R, t, triangulatedPoints;
    cv::recoverPose(E, matchedPoints1, matchedPoints2, cameraMatrix, R, t, 50, cv::noArray(), triangulatedPoints);

    cv::Mat tmp;
    cv::hconcat(R, t, tmp);
    cv::Mat globalMovement = R * t;

    cv::Mat newRow = (cv::Mat_<double>(1, 4) << 0, 0, 0, 1);
    tmp.push_back(newRow);
    P2 = P1 * tmp;

    normalizeCoordinates4to3(triangulatedPoints);

    cv::Mat R1 = P1(cv::Rect(0, 0, 3, 3));
    cv::Mat t1 = P1(cv::Rect(3, 0, 1, 3));

    cv::Mat globalTP = R1 * triangulatedPoints;

    matrixAddVector(globalTP, t1);

    std::cout << P1 << std::endl;
    std::cout << globalTP << std::endl;

    return globalTP;
}

// Main triangulation function
cv::Mat triangulation(const cv::Mat &firstFrame, std::vector<cv::Mat> &framePool, const cv::Mat &cameraMatrix,
                      const cv::Mat &P1, cv::Mat &P2) {
    secondFrame = framePool[framePool.size() - 1];
    numOfFirstUnusedFromPool = framePool.size();

    cv::Mat firstGray, secondGray;
    cv::cvtColor(firstFrame, firstGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(secondFrame, secondGray, cv::COLOR_BGR2GRAY);

    std::vector<cv::KeyPoint> keyPoints1, keyPoints2;
    cv::Mat descriptors1, descriptors2;

    extractKeyPointsAndDescriptors(firstGray, keyPoints1, descriptors1);
    extractKeyPointsAndDescriptors(secondGray, keyPoints2, descriptors2);

    std::vector<cv::DMatch> goodMatches;
    performFeatureMatching(descriptors1, descriptors2, goodMatches);

    size_t was = goodMatches.size();
    goodMatches = goodMatchesCheck(goodMatches, framePool, descriptors1, keyPoints2);
    size_t become = goodMatches.size();
    if (become > was) {
        std::cout << "transform:" << was << " -> " << become << std::endl;
    }

    drawAndDisplayMatches(firstGray, secondGray, keyPoints1, keyPoints2, goodMatches);

    std::vector<cv::Point2d> matchedPoints1, matchedPoints2;
    for (const cv::DMatch &match: goodMatches) {
        matchedPoints1.push_back(keyPoints1[match.queryIdx].pt);
        matchedPoints2.push_back(keyPoints2[match.trainIdx].pt);
    }

    return performTriangulation(matchedPoints1, matchedPoints2, cameraMatrix, P1, P2);
}
