#ifndef SL4M_OUTDOOR_MAPPOINT_H
#define SL4M_OUTDOOR_MAPPOINT_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "trafficSign.h"

class MapPoint {
private:
    // массив фичей, обнаруженных в этот момент времени (их 2D координаты)
    std::vector<cv::Point2d> localCoordinates;

    cv::Mat hash;

    size_t rebuiltOn;

    cv::Mat P;

    std::vector<trafficSign> signs;


public:
    MapPoint(const cv::Mat &P, std::vector<cv::Point2d> localCoordinates, cv::Mat hash);

    void setGlobalCoordinates(const cv::Mat &newP);

    cv::Mat getGlobalCoordinates();

    cv::Mat getR();

    void setR(const cv::Mat &newR);

    cv::Point3d getT();

    void setT(const cv::Point3d &newT);

    cv::Mat getMatT();

    void setMatT(const cv::Mat &newT);

    cv::Point2d get2DCoordinates();

    cv::Mat getHash();

    size_t getRebuiltOn() const;

    void setRebuiltOn(size_t newIsRebuild);
};

// тестирует calculateNewCoords из MapPoint
int testCoordsFinder(int angleInDegrees, cv::Point2d startPoint, cv::Mat vector);

#endif //SL4M_OUTDOOR_MAPPOINT_H
