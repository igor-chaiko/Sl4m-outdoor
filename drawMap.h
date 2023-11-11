//
// Created by ruslan on 11.11.2023.
//

#ifndef SL4M_OUTDOOR_DRAWMAP_H
#define SL4M_OUTDOOR_DRAWMAP_H

#include <opencv2/opencv.hpp>
#include "mapPoint.h"
#include <vector>


class DrawMap {
private:
    std::vector<MapPoint> coordinates;
    int canvasHeight = 900;
    int canvasWeight = 900;

    void drawAxes(cv::Mat canvas);

    cv::Point2d transformationOfCoordinatesToMatrixView(cv::Point2d point);
public:
    DrawMap(std::vector<MapPoint> coordinates);

    void addPoint(MapPoint point);

    void drawMap();

    void setCanvasWeight(int weight);

    void setCanvasHeight(int height);
};

#endif //SL4M_OUTDOOR_DRAWMAP_H
