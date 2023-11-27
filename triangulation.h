#include <opencv2/opencv.hpp>

#ifndef SL4M_OUTDOOR_TRIANGULATION_H
#define SL4M_OUTDOOR_TRIANGULATION_H

void triangulation(const cv::Mat &firstFrame, const cv::Mat &secondFrame, cv::Mat cameraMatrix, const cv::Mat &P1,
                   const cv::Mat &P2, cv::Mat points3D);

#endif //SL4M_OUTDOOR_TRIANGULATION_H
