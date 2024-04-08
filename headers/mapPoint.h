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

    cv::Mat image;

    cv::Mat P;

public:

    MapPoint(cv::Point2d globalCoordinate, std::vector<cv::Point2d> localCoordinates,
             cv::Mat hash, bool isRebuild, cv::Mat image);

    void setGlobalCoordinates(double x, double y);

    void setGlobalCoordinates(cv::Point2d);

    cv::Point2d getGlobalCoordinates();


    // вычисляем глобальные координаты точки, в которую ведет вектор
    cv::Point2d calculateNewCoords(cv::Mat, cv::Mat);

    // вычисляем угол поворота камеры
    static cv::Point2d calculateRotationAngle(cv::Mat);

    cv::Mat getHash();

    bool getIsRebuild();

    cv::Mat getImage();

    void setIsRebuild(bool isRebuild);
};

// тестирует calculateNewCoords из MapPoint
int testCoordsFinder(int angleInDegrees, cv::Point2d startPoint, cv::Mat vector);

#endif //SL4M_OUTDOOR_MAPPOINT_H
