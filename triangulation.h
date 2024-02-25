#include <opencv2/opencv.hpp>

#ifndef SL4M_OUTDOOR_TRIANGULATION_H
#define SL4M_OUTDOOR_TRIANGULATION_H

#define EPS 10
#define MIN_SIZE 400
#define MIN_MATCHES 125

extern int numOfFirstUnusedFromPool;
extern cv::Mat secondFrame;

cv::Mat triangulation(const cv::Mat &firstFrame,  std::vector<cv::Mat> &, const cv::Mat &cameraMatrix, const cv::Mat &P1,
                   cv::Mat &P2);

#endif //SL4M_OUTDOOR_TRIANGULATION_H
