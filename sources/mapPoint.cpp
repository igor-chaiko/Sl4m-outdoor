#include <opencv2/opencv.hpp>
#include <utility>
#include "../headers/mapPoint.h"

MapPoint::MapPoint(const cv::Mat &P, std::vector<cv::Point2d> localCoordinates,
                   cv::Mat hash) {
    setGlobalCoordinates(P);
    this->localCoordinates = std::move(localCoordinates);
    this->hash = std::move(hash);
    this->rebuiltOn = -1;
}

void MapPoint::setGlobalCoordinates(const cv::Mat &newP) {
    this->P = newP.clone();
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

cv::Mat MapPoint::getMatT() {
    return P.col(3).clone();
}

void MapPoint::setT(const cv::Point3d &newT) {
    P.at<double>(2, 3) = newT.y;
    P.at<double>(1, 3) = newT.z;
    P.at<double>(0, 3) = newT.x;
}

void MapPoint::setMatT(const cv::Mat &newT) {
    newT.copyTo(P.col(3));
}

cv::Point2d MapPoint::get2DCoordinates() {
    double y = P.at<double>(2, 3);
    double z = P.at<double>(1, 3);
    double x = P.at<double>(0, 3);

    return {x, y};
}

cv::Mat MapPoint::getHash() {
    return this->hash;
}

size_t MapPoint::getRebuiltOn() const {
    return this->rebuiltOn;
}

void MapPoint::setRebuiltOn(size_t newIsRebuild) {
    this->rebuiltOn = newIsRebuild;
}
