#include <opencv2/opencv.hpp>
#include "mapPoint.h"

MapPoint::MapPoint(cv::Point2d globalCoordinate, std::vector<cv::Point2d> localCoordinates, cv::Point2d vector) {
    this->globalCoordinate = globalCoordinate;
    this->localCoordinates = localCoordinates;
    this->vector = vector;
}

void MapPoint::setVector(double x, double y) {
    vector.x = x;
    vector.y = y;
}

cv::Point2d MapPoint::getVector() {
    return this->vector;
}

double MapPoint::vectorLength() {
    double x = vector.x, y = vector.y;
    return std::sqrt(x * x + y * y);
}

double MapPoint::calculateAngle() {
    return std::atan2(vector.y, vector.x);
}

void MapPoint::setGlobalCoordinates(double x, double y) {
    globalCoordinate.x = x;
    globalCoordinate.y = y;
}

void MapPoint::setGlobalCoordinates(cv::Point2d newGlobalCoordinate) {
    globalCoordinate = newGlobalCoordinate;
}