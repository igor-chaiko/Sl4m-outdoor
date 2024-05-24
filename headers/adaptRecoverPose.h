//
// Created by rustam on 18.05.2024.
//

#ifndef SL4M_OUTDOOR_ADAPTRECOVERPOSE_H
#define SL4M_OUTDOOR_ADAPTRECOVERPOSE_H

#include <opencv2/core/mat.hpp>

int adaptRecoverPose(cv::InputArray E, cv::InputArray _points1, cv::InputArray _points2,
                     cv::InputArray _cameraMatrix, cv::OutputArray _R, cv::OutputArray _t, double distanceThresh,
                     cv::InputOutputArray _mask, cv::OutputArray triangulatedPoints);

#endif //SL4M_OUTDOOR_ADAPTRECOVERPOSE_H
