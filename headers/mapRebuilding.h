//
// Created by rustam on 23.03.24.
//

#ifndef SL4M_OUTDOOR_MAPREBUILDING_H
#define SL4M_OUTDOOR_MAPREBUILDING_H

void closeLoopLine(std::vector<cv::Point2d> &loop);

void closeLoopExp(std::vector<cv::Point2d> &loop);

void softenAngle(std::vector<cv::Point2d> &loop);

#endif //SL4M_OUTDOOR_MAPREBUILDING_H
