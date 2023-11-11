#include <opencv2/opencv.hpp>
#include "mapPoint.h"

MapPoint::MapPoint(cv::Point2d globalCoordinate, std::vector<cv::Point2d> localCoordinates, cv::Point2d vector) {
    this->globalCoordinate = globalCoordinate;
    this->localCoordinates = localCoordinates;
    this->vector = vector;
}

void MapPoint::setGlobalCoordinates(double x, double y) {
    globalCoordinate.x = x;
    globalCoordinate.y = y;
}

void MapPoint::setGlobalCoordinates(cv::Point2d newGlobalCoordinate) {
    globalCoordinate = newGlobalCoordinate;
}

cv::Point2d MapPoint::getGlobalCoordinates() {
    return this->globalCoordinate;
}

void MapPoint::setVector(double x, double y) {
    vector.x = x;
    vector.y = y;
}

cv::Point2d MapPoint::getVector() {
    return this->vector;
}

double MapPoint::vectorLength() const {
    double x = vector.x, y = vector.y;
    return std::sqrt(x * x + y * y);
}

double MapPoint::calculateAngle() const {
    return std::atan2(vector.y, vector.x);
}

cv::Point2d MapPoint::calculateNewCoords(cv::Point2d startCoords,
                                         cv::Mat displacementVector3D,
                                         cv::Mat rotationMatrix3D) {
    cv::Point2d globalCoords;

    // переход от 3х3 матрицы к 2х2, учитывая вращение только по Z
    cv::Mat displacementVector2D = displacementVector3D(cv::Range(0, 2),
                                                        cv::Range::all()
                                                        );
    //убрезаем у вектора координату Z
    cv::Mat rotationMatrix2D = rotationMatrix3D(cv::Range(0, 2),
                                                cv::Range(0, 2)
                                                );
    // вычисляем обратную матрицу вращения
    cv::Mat inverseRotationMatrix = rotationMatrix2D.t();

    // вектор, повернутый в обратном направлении
    cv::Mat inverseVector = inverseRotationMatrix * displacementVector2D;

    globalCoords.x = startCoords.x + inverseVector.at<double>(0, 0);
    globalCoords.y = startCoords.y + inverseVector.at<double>(0, 1);

    return globalCoords;
}

cv::Point2d MapPoint::calculateRotationAngle(cv::Mat rotationMatrix3D) {
    cv::Point2d rotationAngle;

    rotationAngle.x = rotationMatrix3D.at<double>(0, 0);
    rotationAngle.y = rotationMatrix3D.at<double>(1, 0);

    return rotationAngle;
}
