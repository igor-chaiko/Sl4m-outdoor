#ifndef SL4M_OUTDOOR_MAP_H
#define SL4M_OUTDOOR_MAP_H

#include <opencv2/opencv.hpp>
#include "mapPoint.h"
#include <vector>

#define WINDOW_NAME "Map"


class Map {
private:
    std::vector<MapPoint> coordinates;
    int canvasSize = 900;

    double maxCoordinate = std::numeric_limits<double>::min();


    void drawAxes(cv::Mat canvas);

    cv::Point2d transformationOfCoordinatesToMatrixView(cv::Point2d point);
public:
    Map(std::vector<MapPoint> coordinates);

    Map();


    void addPoint(MapPoint point);

    void showMap();

    void setCanvasSize(int size);
};

#endif //SL4M_OUTDOOR_MAP_H
