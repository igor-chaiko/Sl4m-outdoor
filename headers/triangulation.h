#include <opencv2/opencv.hpp>

#ifndef SL4M_OUTDOOR_TRIANGULATION_H
#define SL4M_OUTDOOR_TRIANGULATION_H

#define DEFAULT_THRESHOLD 0.8
#define MIN_MATCHES 220
#define DISTANCE_THRESH 30

cv::Mat
triangulation(const cv::Mat &firstFrame, const cv::Mat &secondFrame, const cv::Mat &cameraMatrix, const cv::Mat &P1,
              cv::Mat &P2);

void performFeatureMatching(const cv::Mat &descriptors1, const cv::Mat &descriptors2,
                            std::vector<cv::DMatch> &goodMatches);

void extractKeyPointsAndDescriptors(const cv::Mat &image, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors);

#endif //SL4M_OUTDOOR_TRIANGULATION_H
