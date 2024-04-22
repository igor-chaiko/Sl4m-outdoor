#ifndef SL4M_OUTDOOR_LOOPCHECKER_H
#define SL4M_OUTDOOR_LOOPCHECKER_H

#include <fstream>
#include "../headers/mapPoint.h"

#define HAMMING_DISTANCE 12
#define TRASH_HOLD_FOR_DISTANCE 8

void loopCheck(std::vector<MapPoint> &coordinatesOnMap, bool &isRebuild);

bool checkThatCoordinatesArentRebuild(std::vector<MapPoint> &loop, int startIndex, int lastIndex);

double lengthBetweenTwoPoint(cv::Point2d point1, cv::Point2d point2);

#endif //SL4M_OUTDOOR_LOOPCHECKER_H
