#ifndef SL4M_OUTDOOR_MAP_H
#define SL4M_OUTDOOR_MAP_H

#include <opencv2/opencv.hpp>
#include "mapPoint.h"
#include <vector>

class DrawMap {
private:
    std::vector<MapPoint> coordinates;
    int canvasSize = 900;

    double maxCoordinate = std::numeric_limits<double>::min();


    void drawAxes(cv::Mat canvas);

    cv::Point2d transformationOfCoordinatesToMatrixView(cv::Point2d point);
public:
    DrawMap(std::vector<MapPoint> coordinates);

    void addPoint(MapPoint point);

    void drawMap();

    void setCanvasSize(int size);
};

#endif //SL4M_OUTDOOR_MAP_H
