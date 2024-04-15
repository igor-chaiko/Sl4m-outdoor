#include "../headers/loopChecker.h"
#include "opencv2/img_hash.hpp"
#include "../headers/mapRebuilding.h"


void loopCheck(std::vector<MapPoint>& coordinatesOnMap) {
    cv::Ptr<cv::img_hash::ImgHashBase> func;
    func = cv::img_hash::PHash::create();


    int length = static_cast<int>(coordinatesOnMap.size());
    MapPoint lastFrameMapPoint = coordinatesOnMap[length - 1];
    cv::Mat lastFrameHash  = lastFrameMapPoint.getHash();

    //std::cout << "start" << std::endl;
    for (int i = length - 2; i >= 0; i--) {
        MapPoint currentFrameMapPoint = coordinatesOnMap[i];
        cv::Mat currentFrameHash = currentFrameMapPoint.getHash();

        double resultOfComparing = func->compare(lastFrameHash, currentFrameHash);
        //std::cout << resultOfComparing << std::endl;
        if (resultOfComparing <= 10) {
            cv::Point2d lastFrameCoordinates = lastFrameMapPoint.get2DCoordinates();
            cv::Point2d currentFrameCoordinates = currentFrameMapPoint.get2DCoordinates();
            if (lengthBetweenTwoPoint(lastFrameCoordinates, currentFrameCoordinates) >= 5
            && checkThatCoordinatesArentRebuild(coordinatesOnMap, i, length - 1)) {
                std::cout << "distance > 5" << std::endl;
                rebuildPath(coordinatesOnMap, i);
                std::cout << i << std::endl;
                std::cout << length - 1 << std::endl;

            }
        }
    }
    //std::cout << "end" << std::endl;


}

bool checkThatCoordinatesArentRebuild(std::vector<MapPoint> &loop, int startIndex, int lastIndex) {
    for (int i = startIndex; i <= lastIndex; i++) {
        if (loop[i].getRebuiltOn() != -1) {
            return false;
        }
    }
    return true;
}

double lengthBetweenTwoPoint(cv::Point2d point1, cv::Point2d point2) {
    double x1 = point1.x;
    double y1 = point1.y;
    double x2 = point2.x;
    double y2 = point2.y;

    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
