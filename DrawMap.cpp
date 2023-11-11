#include "DrawMap.h"

class DrawMap {
private:
    std::vector<MapPoint> coordinates;

public:
    void addPoint(MapPoint point) {
        coordinates.push_back(point);
    }

    void drawMap() {
        cv::Mat blackScreen = cv::Mat::zeros(400, 600, CV_8UC3);
        // Координаты точки, которую вы хотите нарисовать
        cv::Point point(300, 200);

        // Цвет точки (в формате BGR, в данном случае - белый)
        cv::Scalar color(255, 255, 255);

        // Рисование точки на черном экране
        cv::circle(blackScreen, point, 5, color, -1); // -1 означает заполнить точку
    }


};

