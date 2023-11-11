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

    // направление оси x в локальной системе координат (нормализованное, т.е. единичное)
    cv::Point2d vector;

public:

    MapPoint(cv::Point2d globalCoordinate, std::vector<cv::Point2d> localCoordinates, cv::Point2d vector);

    void setGlobalCoordinates(double x, double y);

    void setGlobalCoordinates(cv::Point2d);

    void setVector(double x, double y);

    cv::Point2d getVector();

    double vectorLength();

    // угол в радианах
    double calculateAngle();
};

#endif //SL4M_OUTDOOR_MAPPOINT_H
