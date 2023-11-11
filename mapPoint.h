//
// Created by ruslan on 11.11.2023.
//

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
    void setVector(double x, double y);

    double vectorLength();

    // угол в радианах
    double calculateAngle();
};

#endif //SL4M_OUTDOOR_MAPPOINT_H
