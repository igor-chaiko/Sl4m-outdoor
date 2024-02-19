#include "Map.h"

Map::Map() {
    this->coordinatesOnMap = std::vector<MapPoint>();
    this->canvasSize = WINDOW_SIZE_IN_PIXELS;
    this->maxCoordinate = std::numeric_limits<double>::min();
    this->indexOfCurrentPointThatNeedsToBeDrawn = 0;
    this->isMaxCoordinateChange = false;
    this->canvas = cv::Mat::zeros(canvasSize, canvasSize, CV_8UC3); // байт на канал, 3 канала цвета (ргб вообщем)
}

void Map::addPoint(const MapPoint& point) {
    this->coordinatesOnMap.push_back(point);
    isCoordinateMoreThanMax(point);
}

void Map::isCoordinateMoreThanMax(MapPoint point) {
    double currAbsX = abs(point.getGlobalCoordinates().x);
    if (currAbsX > maxCoordinate) {
        maxCoordinate = currAbsX + CONST_FOR_SCALE;
        isMaxCoordinateChange = true;
    }

    double currAbsY = abs(point.getGlobalCoordinates().y);
    if (currAbsY > maxCoordinate) {
        maxCoordinate = currAbsY + CONST_FOR_SCALE;
        isMaxCoordinateChange = true;
    }
}


void Map::addFeaturesPoint(cv::Mat mat) {
//    this->features.push_back(mat);
//
//    int weight = mat.rows;
//    int height = mat.cols;
//
//
//    for (int i = 0; i < height; i++) {
//        // double y = -P2.at<double>(2, 3);
//        // double z = P2.at<double>(1, 3);
//        // double x = P2.at<double>(0, 3);
//
//        double y = -mat.at<double>(2, i);
//        double z = mat.at<double>(1, i);
//        double x = mat.at<double>(0, i);
//
//        if (abs(y) > maxCoordinate && abs(y) < 100) {
//            maxCoordinate = abs(y);
//        }
//
//        if (abs(x) > maxCoordinate && abs(x) < 100) {
//            maxCoordinate = abs(x);
//        }
//    }
}
void Map::showMap(int delay) {
    double coefficient = maxCoordinate * COEFFICIENT_FOR_SCALE / ((double)canvasSize / 2);

    if (isMaxCoordinateChange) {
        isMaxCoordinateChange = false;

        this->canvas = cv::Mat::zeros(canvasSize, canvasSize, CV_8UC3);
        drawAxes(this->canvas);

        cv::Point2d lastPoint = cv::Point2d((double)canvasSize / 2, (double)canvasSize / 2);//центр полотна
        for (MapPoint mapPoint : this->coordinatesOnMap) {
            cv::Point2d tmpPoint = mapPoint.getGlobalCoordinates();
            tmpPoint.x = tmpPoint.x / coefficient;
            tmpPoint.y = tmpPoint.y / coefficient;
            tmpPoint = transformationOfCoordinatesToMatrixView(tmpPoint);

            cv::circle(this->canvas, tmpPoint, POINT_RADIUS, cv::Scalar(0, 0, 255), 1);
            cv::arrowedLine(this->canvas, lastPoint, tmpPoint, cv::Scalar(0, 255, 255), 1);
            lastPoint = tmpPoint;
        }
        indexOfCurrentPointThatNeedsToBeDrawn = static_cast<long>(this->coordinatesOnMap.size());
    } else if (coordinatesOnMap.size() != this->indexOfCurrentPointThatNeedsToBeDrawn) {

        for ( ; indexOfCurrentPointThatNeedsToBeDrawn < coordinatesOnMap.size(); indexOfCurrentPointThatNeedsToBeDrawn++) {
            cv::Point2d lastPoint;
            if (this->indexOfCurrentPointThatNeedsToBeDrawn == 0) {
                lastPoint = cv::Point2d((double)canvasSize / 2, (double)canvasSize / 2);//центр полотна
            } else {
                lastPoint = this->coordinatesOnMap[indexOfCurrentPointThatNeedsToBeDrawn - 1].getGlobalCoordinates();
                lastPoint.x = lastPoint.x / coefficient;
                lastPoint.y = lastPoint.y / coefficient;
            }
            lastPoint = transformationOfCoordinatesToMatrixView(lastPoint);

            cv::Point2d tmpPoint = this->coordinatesOnMap[indexOfCurrentPointThatNeedsToBeDrawn].getGlobalCoordinates();
            tmpPoint.x = tmpPoint.x / coefficient;
            tmpPoint.y = tmpPoint.y / coefficient;
            tmpPoint = transformationOfCoordinatesToMatrixView(tmpPoint);
            cv::circle(this->canvas, tmpPoint, POINT_RADIUS, cv::Scalar(0, 0, 255), 1);
            cv::arrowedLine(this->canvas, lastPoint, tmpPoint, cv::Scalar(0, 255, 255), 1);
        }
        indexOfCurrentPointThatNeedsToBeDrawn = static_cast<long>(this->coordinatesOnMap.size());
    }


    //Блок по обработке фичей, но пока фичи не работают
//    for (cv::Mat pointMatrix : features) {
//        std::cout << pointMatrix.size << std::endl;
//
//        int weight = pointMatrix.cols;
//        //int height = pointMatrix.rows;
//
//        for (int i = 0; i < weight; i++) {
//            // double y = -P2.at<double>(2, 3);
//            // double z = P2.at<double>(1, 3);
//            // double x = P2.at<double>(0, 3);
//
//            double y = pointMatrix.at<double>(2, i) / coefficient;
//            //double z = pointMatrix.at<double>(1, i);
//            double x = pointMatrix.at<double>(0, i) / coefficient;
//            cv::Point2d tmpPoint = cv::Point2d(x, y);
//            tmpPoint = transformationOfCoordinatesToMatrixView(tmpPoint);
//
//
//            cv::circle(blackCanvas, tmpPoint, 1, cv::Scalar(169, 242, 202), -1);
//        }
//    }

    cv::imshow(WINDOW_NAME, this->canvas);
    cv::waitKey(delay);
}

void Map::drawAxes(cv::Mat canvas) const {
    cv::Point2d start_x(0, static_cast<double>(canvasSize) / 2);
    cv::Point2d finish_x(canvasSize, static_cast<double>(canvasSize) / 2);
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);

    start_x.x = canvasSize - 10;
    start_x.y = static_cast<double>(canvasSize) / 2 - 10;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);
    start_x.y = static_cast<double>(canvasSize) / 2 + 10;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);

    start_x.x = canvasSize - 15;
    start_x.y = static_cast<double>(canvasSize) / 2 - 20;
    finish_x.x = canvasSize - 5;
    finish_x.y = static_cast<double>(canvasSize) / 2 - 10;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);

    start_x.y = static_cast<double>(canvasSize) / 2 - 10;
    finish_x.y = static_cast<double>(canvasSize) / 2 - 20;
    cv::line(canvas, start_x, finish_x, cv::Scalar(255,255,255), 1);


    cv::Point2d start_y(static_cast<double>(canvasSize) / 2, canvasSize);
    cv::Point2d finish_y(static_cast<double>(canvasSize) / 2, 0);
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);

    start_y.x = static_cast<double>(canvasSize) / 2 - 10;
    start_y.y = 10;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);
    start_y.x = static_cast<double>(canvasSize) / 2 + 10;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);

    start_y.y = 15;
    finish_y.x = static_cast<double>(canvasSize) / 2 + 5;
    finish_y.y = 10;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);

    start_y.x = static_cast<double>(canvasSize) / 2 + 15;
    start_y.y = 10;
    finish_y.x = static_cast<double>(canvasSize) / 2 + 5;
    finish_y.y = 20;
    cv::line(canvas, start_y, finish_y, cv::Scalar(255,255,255), 1);
}

cv::Point2d Map::transformationOfCoordinatesToMatrixView(cv::Point2d point) const {
    double coordinateX = static_cast<double>(canvasSize) / 2 + point.x;
    double coordinateY = static_cast<double>(canvasSize) / 2 - point.y;
    return {coordinateX, coordinateY};
}
