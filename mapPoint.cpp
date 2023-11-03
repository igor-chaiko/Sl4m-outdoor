#include <opencv2/opencv.hpp>

class MapPoint {
private:
//    переменная для хранения координаты (x, y) в какой-то момент времени
    cv::Point2d globalCoordinate;
//    массив точек, обноруженных в этот момент врмени(их 2д координаты)
    std::vector<cv::Point2d> localCoordinates;
//    вектор смещения
    cv::Point2d vector;

public:
    void setVector(double x, double y) {
        vector.x = x;
        vector.y = y;
    }

    double vectorLength() {
        double x = vector.x, y = vector.y;

        return std::sqrt(x * x + y * y);
    }

//    угол в радианах
    double calculateAngle() {
        return std::atan2(vector.x, vector.y);
    }
};
