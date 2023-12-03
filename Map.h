#ifndef SL4M_OUTDOOR_MAP_H
#define SL4M_OUTDOOR_MAP_H

#include <opencv2/opencv.hpp>
#include "mapPoint.h"
#include <vector>

#define WINDOW_NAME "Map"


class Map {
private:
    std::vector<MapPoint> coordinates;
    int canvasSize = 600;

    double maxCoordinate = std::numeric_limits<double>::min();

    void drawAxes(cv::Mat canvas);

    cv::Point2d transformationOfCoordinatesToMatrixView(cv::Point2d point);

    std::vector<cv::Mat> feachures = std::vector<cv::Mat>();
public:
    Map(std::vector<MapPoint> coordinates);

    Map();

    void addFeaturesPoint(cv::Mat mat);

    void addPoint(MapPoint point);

    void showMap(int delay);

    void setCanvasSize(int size);
};

#endif //SL4M_OUTDOOR_MAP_H
