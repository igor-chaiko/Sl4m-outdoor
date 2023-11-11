#include "Map.h"

DrawMap::DrawMap(std::vector<MapPoint> coordinates) {
    this->coordinates = coordinates;
    for (MapPoint mapPoint: coordinates) {
        if(abs(mapPoint.getGlobalCoordinates().x) > maxCoordinate) {
            maxCoordinate = abs(mapPoint.getGlobalCoordinates().x);
        }
        if(abs(mapPoint.getGlobalCoordinates().y) > maxCoordinate) {
            maxCoordinate = abs(mapPoint.getGlobalCoordinates().y);
        }
    }
}

void DrawMap::addPoint(MapPoint point) {
    this->coordinates.push_back(point);
    if (point.getGlobalCoordinates().x > maxCoordinate) {
        maxCoordinate = abs(point.getGlobalCoordinates().x);
    }
    if (point.getGlobalCoordinates().y > maxCoordinate) {
        maxCoordinate = abs(point.getGlobalCoordinates().y);
    }
}

void DrawMap::drawMap() {
    cv::Mat blackCanvas = cv::Mat::zeros(canvasSize, canvasSize, CV_8UC3); // байт на канал, 3 канала цвета (ргб вообщем)
    drawAxes(blackCanvas);

    cv::Point2d lastPoint = cv::Point2d(canvasSize / 2, canvasSize / 2);
    double coefficient = maxCoordinate / (double)(canvasSize / 2);
    if (coefficient >= 1) {
        coefficient = maxCoordinate * 1.1 / (double)(canvasSize / 2);
    } else {
        coefficient = maxCoordinate * 1.3 / (double)(canvasSize / 2);
    }
    for (MapPoint mapPoint:this->coordinates) {
        cv::Point2d tmpPoint = mapPoint.getGlobalCoordinates();
        tmpPoint.x = tmpPoint.x / coefficient;
        tmpPoint.y = tmpPoint.y / coefficient;
        tmpPoint = transformationOfCoordinatesToMatrixView(tmpPoint);

        cv::circle(blackCanvas, tmpPoint, 2, cv::Scalar(0, 0, 255), -1);
        cv::arrowedLine(blackCanvas, lastPoint, tmpPoint, cv::Scalar(0, 255, 255), 1);
        lastPoint = tmpPoint;
    }

    cv::imshow("Map", blackCanvas);
    cv::waitKey(0);
}

void DrawMap::drawAxes(cv::Mat canvas) {
    cv::Point2d start_x(0, canvasSize / 2);
    cv::Point2d finish_x(canvasSize, canvasSize / 2);
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);

    start_x.x = canvasSize - 10;
    start_x.y = canvasSize / 2 - 10;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);
    start_x.y = canvasSize / 2 + 10;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);

    start_x.x = canvasSize - 15;
    start_x.y = canvasSize / 2 - 20;
    finish_x.x = canvasSize - 5;
    finish_x.y = canvasSize / 2 - 10;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);

    start_x.y = canvasSize / 2 - 10;
    finish_x.y = canvasSize / 2 - 20;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);


    cv::Point2d start_y(canvasSize / 2, canvasSize);
    cv::Point2d finish_y(canvasSize / 2, 0);
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);

    start_y.x = canvasSize / 2 - 10;
    start_y.y = 10;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);
    start_y.x = canvasSize / 2 + 10;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);

    start_y.y = 15;
    finish_y.x = canvasSize / 2 + 5;
    finish_y.y = 10;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);

    start_y.x = canvasSize / 2 + 15;
    start_y.y = 10;
    finish_y.x = canvasSize / 2 + 5;
    finish_y.y = 20;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);
}

cv::Point2d DrawMap::transformationOfCoordinatesToMatrixView(cv::Point2d point) {
    int coordinateX = canvasSize / 2 + point.x;
    int coordinateY = canvasSize / 2 - point.y;
    cv::Point2d pointInNewCoordinate(coordinateX, coordinateY);
    return pointInNewCoordinate;
}

void DrawMap::setCanvasSize(int size) {
    this->canvasSize = size;
}
