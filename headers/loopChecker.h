#ifndef SL4M_OUTDOOR_LOOPCHECKER_H
#define SL4M_OUTDOOR_LOOPCHECKER_H
#include <fstream>
#include "../headers/mapPoint.h"

void loopCheck(std::vector<MapPoint>& coordinatesOnMap);
bool checkThatCoordinatesArentRebuild(std::vector<MapPoint> &loop, int startIndex, int lastIndex);
double lengthBetweenTwoPoint(cv::Point2d point1, cv::Point2d point2);
#endif //SL4M_OUTDOOR_LOOPCHECKER_H
