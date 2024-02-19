#include <opencv2/opencv.hpp>
#include <utility>
#include "mapPoint.h"

MapPoint::MapPoint(cv::Point2d globalCoordinate, std::vector<cv::Point2d> localCoordinates, cv::Point2d vector) {
    this->globalCoordinate = globalCoordinate;
    this->localCoordinates = std::move(localCoordinates);
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

/**
 * фунция для нахождения координат с учетом вращения.
 * @return глобальные координаты.
 */
cv::Point2d MapPoint::calculateNewCoords(cv::Mat displacementVector3D,
                                         cv::Mat rotationMatrix3D) {
    cv::Point2d globalCoords;

    // переход от 3х3 матрицы к 2х2, учитывая вращение только вдоль Z
    cv::Mat displacementVector2D = displacementVector3D(cv::Range(0, 2),
                                                        cv::Range::all()
                                                        );
    // отрезаем у вектора координату Z
    cv::Mat rotationMatrix2D = rotationMatrix3D(cv::Range(0, 2),
                                                cv::Range(0, 2)
                                                );

    // повернутый вектор
    cv::Mat inverseVector = rotationMatrix2D * displacementVector2D;

    globalCoords.x = this->globalCoordinate.x + inverseVector.at<double>(0, 0);
    globalCoords.y = this->globalCoordinate.y + inverseVector.at<double>(0, 1);

    return globalCoords;
}

cv::Point2d MapPoint::calculateRotationAngle(cv::Mat rotationMatrix3D) {
    cv::Point2d rotationAngle;

    rotationAngle.x = rotationMatrix3D.at<double>(0, 0);
    rotationAngle.y = rotationMatrix3D.at<double>(1, 0);

    return rotationAngle;
}
