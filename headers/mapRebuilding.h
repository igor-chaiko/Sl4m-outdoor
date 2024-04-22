//
// Created by rustam on 23.03.24.
//

#ifndef SL4M_OUTDOOR_MAPREBUILDING_H
#define SL4M_OUTDOOR_MAPREBUILDING_H

#include "mapPoint.h"

void rebuildPath(std::vector<MapPoint> &loop, int mainIndex);

void closeLoopExp(std::vector<MapPoint> &loop, int startIndex, int lastIndex);

void softenAngle(std::vector<MapPoint> &loop, int startIndex, int lastIndex);

#endif //SL4M_OUTDOOR_MAPREBUILDING_H
