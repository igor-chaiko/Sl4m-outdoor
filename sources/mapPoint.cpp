#include <opencv2/opencv.hpp>
#include <utility>
#include "../headers/mapPoint.h"

MapPoint::MapPoint(const cv::Mat &P, std::vector<cv::Point2d> localCoordinates,
                   cv::Mat hash, bool isRebuild) {
    setGlobalCoordinates(P);
    this->localCoordinates = std::move(localCoordinates);
    this->hash = std::move(hash);
    this->isRebuild = isRebuild;
}

void MapPoint::setGlobalCoordinates(const cv::Mat &newP) {
    this->P = newP.clone();

    double y = P.at<double>(2, 3);
    double z = P.at<double>(1, 3);
    double x = P.at<double>(0, 3);

    this->globalCoordinate = cv::Point2d(x, y);
}

cv::Mat MapPoint::getGlobalCoordinates() {
    return this->P.clone();
}

cv::Mat MapPoint::getR() {
    return P.colRange(0, 3).clone();
}

void MapPoint::setR(const cv::Mat &newR) {
    newR.copyTo(P.colRange(0, 3));
}

cv::Point3d MapPoint::getT() {
    double y = P.at<double>(2, 3);
    double z = P.at<double>(1, 3);
    double x = P.at<double>(0, 3);

    return {x, y, z};
}

void MapPoint::setT(const cv::Point3d &newT) {
    P.at<double>(2, 3) = newT.y;
    P.at<double>(1, 3) = newT.z;
    P.at<double>(0, 3) = newT.x;

    this->globalCoordinate = cv::Point2d(newT.x, newT.y);
}

cv::Point2d MapPoint::get2DCoordinates() {
    return this->globalCoordinate;
}

cv::Mat MapPoint::getHash() {
    return this->hash;
}

bool MapPoint::getIsRebuild() const {
    return this->isRebuild;
}

void MapPoint::setIsRebuild(bool newIsRebuild) {
    this->isRebuild = newIsRebuild;
}
