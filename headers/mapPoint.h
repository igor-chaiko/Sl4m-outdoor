#ifndef SL4M_OUTDOOR_MAPPOINT_H
#define SL4M_OUTDOOR_MAPPOINT_H

#include <opencv2/opencv.hpp>
#include <vector>

class MapPoint {

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

    cv::Point2d getGlobalCoordinates();

    void setVector(double x, double y);

    cv::Point2d getVector();

    [[nodiscard]]double vectorLength() const;

    // угол в радианах
    [[nodiscard]]double calculateAngle() const;

    // вычисляем глобальные координаты точки, в которую ведет вектор
    [[nodiscard]]cv::Point2d calculateNewCoords(const cv::Mat&, const cv::Mat&) const;

    // вычисляем угол поворота камеры
    static cv::Point2d calculateRotationAngle(cv::Mat);
};

#endif //SL4M_OUTDOOR_MAPPOINT_H
