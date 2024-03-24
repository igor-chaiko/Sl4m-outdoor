#include <opencv2/core/types.hpp>
#include "../headers/mapRebuilding.h"

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

void closeLoopExp(std::vector<cv::Point2d> &loop) {
    if (loop.size() < 3) {
        return;
    }

    size_t num = loop.size() - 1;
    cv::Point2d mainPoint = loop[0];

    for (int i = 1; i < loop.size(); i++) {
        cv::Point2d point = loop[i];
        cv::Point2d movement =
                (point - mainPoint) * std::pow(norm((point - mainPoint)), i / (double) num) / norm((point - mainPoint));

        loop[i] -= movement;
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

    size_t num = size - 1;

    for (int i = 1; i < num; i++) {
        cv::Point2d point = loop[i];

        cv::Point2d rotated = point - mainPoint;
        rotated = cv::Point2d(
                rotated.x * std::cos(fixAngle) + rotated.y * std::sin(fixAngle),
                -rotated.x * std::sin(fixAngle) + rotated.y * std::cos(fixAngle));
        rotated += mainPoint;

        cv::Point2d movement =
                (point - rotated) * std::pow(norm((point - rotated)), i / (double) num) / norm((point - rotated));

        loop[i] -= movement;
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

