#ifndef SL4M_OUTDOOR_MAP_H
#define SL4M_OUTDOOR_MAP_H

#include <opencv2/opencv.hpp>
#include "mapPoint.h"
#include <vector>

#define WINDOW_NAME "Map"
#define WINDOW_SIZE_IN_PIXELS 700
#define COEFFICIENT_FOR_SCALE 1.3
#define POINT_RADIUS 3
#define FEATURES_RADIUS 1

#define CONST_FOR_SCALE 10
class Map {

private:
    std::vector<MapPoint> coordinatesOnMap; //Вектор хранящий все координаты

    int canvasSize; //Размер выводимой карты
    double maxCoordinate; //Максимальная по модулю координата среди всех координат на карте
    std::vector<cv::Mat> features = std::vector<cv::Mat>();//массив фичей
    long indexOfCurrentPointThatNeedsToBeDrawn;//Индекс последней нарисованной точки, для оптимизации вывода
    long indexOfCurrentFeaturesMatrixThatNeedsToBeDrawn;//Индекс последнего нарисованного массива фичей
    bool isMaxCoordinateChange;//Изменилась ли максимальная координата с последней отрисовки true - да | false - нет
    cv::Mat canvas;//Полотно, которое будет хранит уже отрисованную карту


    void drawAxes(cv::Mat canvas) const;

    cv::Point2d transformationOfCoordinatesToMatrixView(cv::Point2d point) const;


    void isCoordinateMoreThanMax(MapPoint point);

    void isCoordinateMoreThanMax(double x, double y);
public:
    //Дефолтный конструткор
    Map();

    //Метод добавляет фичи для отрисовки. (де факто будет отрисовываться окружение, ждём пока Рустам пофиксит, после тестим)
    void addFeaturesPoint(cv::Mat mat);

    //Метод добавляет point для отрисовки
    void addPoint(const MapPoint& point);

    void showMap(int delay);

};

#endif //SL4M_OUTDOOR_MAP_H