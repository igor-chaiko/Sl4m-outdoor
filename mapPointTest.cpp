#include <opencv2/opencv.hpp>
#include "mapPoint.h"

int testCoordsFinder(int angleInDegrees, cv::Point2d startPoint, cv::Mat vector) {
    cv::Point2d arg2, ans;
    std::vector<cv::Point2d> arg3(1);
    MapPoint test(startPoint, arg3, arg2);
    double angle = angleInDegrees * M_PI / 180.0;
    cv::Mat matrix = (cv::Mat_<double>(3, 3) <<
                                             std::cos(angle), -1 * std::sin(angle), 0,
            std::sin(angle), std::cos(angle), 0,
            0, 0, 1
    );
    ans = test.calculateNewCoords(vector, matrix);

    std::cout << "x: " << ans.x << std::endl << "y: " << ans.y << std::endl << std::endl;

    return 0;
}