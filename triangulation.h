#include <opencv2/opencv.hpp>

#ifndef SL4M_OUTDOOR_TRIANGULATION_H
#define SL4M_OUTDOOR_TRIANGULATION_H

cv::Mat triangulation(const cv::Mat &firstFrame, const cv::Mat &secondFrame, const cv::Mat &cameraMatrix, const cv::Mat &P1,
                   cv::Mat &P2);

#endif //SL4M_OUTDOOR_TRIANGULATION_H
