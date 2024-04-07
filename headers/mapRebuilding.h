//
// Created by rustam on 23.03.24.
//

#ifndef SL4M_OUTDOOR_MAPREBUILDING_H
#define SL4M_OUTDOOR_MAPREBUILDING_H
#include "mapPoint.h"
void closeLoopLine(std::vector<cv::Point2d> &loop);

void closeLoopLine(std::vector<MapPoint> &loop, int startIndex, int lastIndex);

void closeLoopExp(std::vector<MapPoint> &loop, int startIndex, int lastIndex);

void softenAngle(std::vector<cv::Point2d> &loop);

#endif //SL4M_OUTDOOR_MAPREBUILDING_H
