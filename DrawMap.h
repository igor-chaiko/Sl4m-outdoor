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

public:
    void addPoint(MapPoint point);

    void drawMap();
};

#endif //SL4M_OUTDOOR_DRAWMAP_H
