#ifndef SL4M_OUTDOOR_MAPPOINT_H
#define SL4M_OUTDOOR_MAPPOINT_H

#include <opencv2/opencv.hpp>
#include <vector>

class MapPoint {
private:
// переменная для хранения координаты (x, y) в какой-то момент времени
    cv::Point2d globalCoordinate;

    // массив фичей, обнаруженных в этот момент времени (их 2D координаты)
    std::vector<cv::Point2d> localCoordinates;

    cv::Mat hash;

    bool isRebuild;

    cv::Mat P;

public:
    MapPoint(const cv::Mat &P, std::vector<cv::Point2d> localCoordinates, cv::Mat hash, bool isRebuild);

    void setGlobalCoordinates(const cv::Mat &newP);

    cv::Mat getGlobalCoordinates();

    cv::Mat getR();

    cv::Mat getT();

    cv::Point2d get2DCoordinates();

    cv::Mat getHash();

    bool getIsRebuild() const;

    void setIsRebuild(bool newIsRebuild);
};

// тестирует calculateNewCoords из MapPoint
int testCoordsFinder(int angleInDegrees, cv::Point2d startPoint, cv::Mat vector);

#endif //SL4M_OUTDOOR_MAPPOINT_H
