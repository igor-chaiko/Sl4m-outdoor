#include "drawMap.h"

DrawMap::DrawMap(std::vector<MapPoint> coordinates) {
    this->coordinates = coordinates;
}

void DrawMap::addPoint(MapPoint point) {
    this->coordinates.push_back(point);
}

void DrawMap::drawMap() {
    cv::Mat blackCanvas = cv::Mat::zeros(canvasHeight, canvasWeight, CV_8UC3); // байт на канал, 3 канала цвета (ргб вообщем)
    drawAxes(blackCanvas);

    cv::Point2d lastPoint = cv::Point2d(canvasWeight / 2, canvasHeight / 2);
    for (MapPoint mapPoint:this->coordinates) {
        cv::Point2d tmpPoint = transformationOfCoordinatesToMatrixView(mapPoint.getGlobalCoordinates());
        cv::circle(blackCanvas, tmpPoint, 2, cv::Scalar(0, 0, 255), -1);
        cv::arrowedLine(blackCanvas, lastPoint, tmpPoint, cv::Scalar(0, 255, 255), 1);
        lastPoint = tmpPoint;
    }

    cv::imshow("Map", blackCanvas);
    cv::waitKey(0);
}

void DrawMap::drawAxes(cv::Mat canvas) {
    cv::Point2d start_x(0, canvasHeight / 2);
    cv::Point2d finish_x(canvasWeight, canvasHeight / 2);
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);

    start_x.x = canvasWeight - 10;
    start_x.y = canvasHeight / 2 - 10;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);
    start_x.y = canvasHeight / 2 + 10;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);

    start_x.x = canvasWeight - 15;
    start_x.y = canvasHeight / 2 - 20;
    finish_x.x = canvasWeight - 5;
    finish_x.y = canvasHeight / 2 - 10;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);

    start_x.y = canvasHeight / 2 - 10;
    finish_x.y = canvasHeight / 2 - 20;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);


    cv::Point2d start_y(canvasWeight / 2, canvasHeight);
    cv::Point2d finish_y(canvasWeight / 2, 0);
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);

    start_y.x = canvasWeight / 2 - 10;
    start_y.y = 10;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);
    start_y.x = canvasWeight / 2 + 10;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);

    start_y.y = 15;
    finish_y.x = canvasWeight / 2 + 5;
    finish_y.y = 10;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);

    start_y.x = canvasWeight / 2 + 15;
    start_y.y = 10;
    finish_y.x = canvasWeight / 2 + 5;
    finish_y.y = 20;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);
}

cv::Point2d DrawMap::transformationOfCoordinatesToMatrixView(cv::Point2d point) {
    int coordinateX = canvasWeight / 2 + point.x;
    int coordinateY = canvasHeight / 2 - point.y;
    cv::Point2d pointInNewCoordinate(coordinateX, coordinateY);
    return pointInNewCoordinate;
}

void DrawMap::setCanvasWeight(int weight) {
    this->canvasWeight = weight;
}

void DrawMap::setCanvasHeight(int height) {
    this->canvasHeight = height;
}


