#include <opencv2/core/types.hpp>
#include "../headers/mapRebuilding.h"


void rebuildPath(std::vector<MapPoint> &loop, int startIndex, int lastIndex) {
    if (lastIndex - startIndex < 4
        || startIndex < 0
        || lastIndex >= loop.size()) {
        return;
    }

    softenAngle(loop, startIndex, lastIndex);

    closeLoopExp(loop, startIndex, lastIndex);

    softenAngle(loop, startIndex, lastIndex);
}


void closeLoopExp(std::vector<MapPoint> &loop, int startIndex, int lastIndex) {
    if (lastIndex - startIndex < 3
        || startIndex < 0
        || lastIndex >= loop.size()) {
        return;
    }

    size_t num = lastIndex - startIndex;
    cv::Point2d mainPoint = loop[startIndex].getGlobalCoordinates();

    for (int i = startIndex + 1; i <= lastIndex; i++) {
        cv::Point2d point = loop[i].getGlobalCoordinates();
        cv::Point2d movement = (mainPoint - point) * (std::pow(num + 1, i / (double) num) - 1) / (double) num;
        loop[i].setGlobalCoordinates(point + movement);
    }
}

void softenAngle(std::vector<MapPoint> &loop, int startIndex, int lastIndex) {
    size_t size = loop.size();
    if (loop.size() < 4) {
        return;
    }

    cv::Point2d mainPoint = loop[startIndex].getGlobalCoordinates();
    cv::Point2d vectorMain = mainPoint - loop[1].getGlobalCoordinates();
    cv::Point2d vector = loop[lastIndex - 1].getGlobalCoordinates() - loop[lastIndex].getGlobalCoordinates();

    double fixAngle = atan2(vector.y, vector.x) - atan2(vectorMain.y, vectorMain.x);
    if (fixAngle > CV_PI) {
        fixAngle -= 2 * CV_PI;
    } else if (fixAngle < -CV_PI) {
        fixAngle += 2 * CV_PI;
    }

    size_t num = size - 1;

    for (int i = startIndex + 1; i <= lastIndex; i++) {
        cv::Point2d point = loop[i].getGlobalCoordinates() - mainPoint;

        double rotate = fixAngle * (std::pow(num + 1, i / (double) num) - 1) / (double) num;
        cv::Point2d rotated = cv::Point2d(
                point.x * std::cos(rotate) + point.y * std::sin(rotate),
                -point.x * std::sin(rotate) + point.y * std::cos(rotate));

        loop[i].setGlobalCoordinates(rotated + mainPoint);
    }
}

void softenAngle(std::vector<cv::Point2d> &loop) {
    size_t size = loop.size();

    if (loop.size() < 4) {
        return;
    }

    cv::Point2d mainPoint = loop[0];
    cv::Point2d vectorMain = loop[0] - loop[1];
    cv::Point2d vector = loop[size - 2] - loop[size - 1];

    double fixAngle = atan2(vector.y, vector.x) - atan2(vectorMain.y, vectorMain.x);
    if (fixAngle > CV_PI) {
        fixAngle -= 2 * CV_PI;
    } else if (fixAngle < -CV_PI) {
        fixAngle += 2 * CV_PI;
    }

    size_t num = size - 1;

    for (int i = 1; i <= num; i++) {
        cv::Point2d point = loop[i] - mainPoint;

        double rotate = fixAngle * (std::pow(num + 1, i / (double) num) - 1) / (double) num;
        cv::Point2d rotated = cv::Point2d(
                point.x * std::cos(rotate) + point.y * std::sin(rotate),
                -point.x * std::sin(rotate) + point.y * std::cos(rotate));

        loop[i] = rotated + mainPoint;
    }
}


void closeLoopLine(std::vector<cv::Point2d> &loop) {
    if (loop.size() < 2) {
        return;
    }

    size_t num = loop.size() - 1;
    cv::Point2d mainPoint = loop[0];

    for (int i = 1; i < loop.size(); i++) {
        cv::Point2d point = loop[i];
        cv::Point2d movement = (point - mainPoint) * (i / (double) num);

        loop[i] -= movement;
    }
}

void closeLoopLine(std::vector<MapPoint> &loop, int startIndex, int lastIndex) {
    if (lastIndex - startIndex < 2 || startIndex > lastIndex) {
        return;
    }

    size_t num = lastIndex - startIndex;
    MapPoint &mainMapPoint = loop[startIndex];
    mainMapPoint.setIsRebuild(true);
    cv::Point2d mainPoint = mainMapPoint.getGlobalCoordinates();
    for (int i = startIndex + 1; i <= lastIndex; i++) {
        MapPoint &currentMapPoint = loop[i];
        currentMapPoint.setIsRebuild(true);
        cv::Point2d point = currentMapPoint.getGlobalCoordinates();
        cv::Point2d movement = (point - mainPoint) * (i / (double) num);
        currentMapPoint.setGlobalCoordinates(point - movement);
    }
}

void closeLoopExp(std::vector<cv::Point2d> &loop) {
    if (loop.size() < 3) {
        return;
    }

    size_t num = loop.size() - 1;
    cv::Point2d mainPoint = loop[0];

    for (int i = 1; i < loop.size(); i++) {
        cv::Point2d point = loop[i];

        cv::Point2d movement = (mainPoint - point) * (std::pow(num + 1, i / (double) num) - 1) / (double) num;

        loop[i] += movement;
    }
}


std::vector<cv::Point2d> goldSection() {
    std::vector<cv::Point2d> loop;
    for (double angle = CV_PI; angle < 3 * CV_PI; angle += 0.1) {
        double x = std::cos(angle) * 10 * angle + 150; // Adjusting x to center the loop
        double y = std::sin(angle) * 10 * angle + 150; // Adjusting y to center the loop
        loop.push_back(cv::Point2d(x, y));
    }
    return loop;
}

