#ifndef SL4M_OUTDOOR_TRAFFICSIGNS_H
#define SL4M_OUTDOOR_TRAFFICSIGNS_H

#include "opencv2/opencv.hpp"
#define AREA_LOWER_BOUND 10000
#define AREA_UPPER_BOUND 40000
#define SIMILARITY 26
#define DISTANCE_BETWEEN_TWO 250

class TrafficSigns {
private:
    std::vector<std::pair<int, cv::Mat>> metSigns;
    int frameNum = 0;
    std::vector<std::pair<cv::Mat, std::string>> findSigns(const cv::Mat& image);
public:
    TrafficSigns();
    std::vector<std::pair<cv::Mat, std::string>> processSigns(const cv::Mat& image);
};

#endif //SL4M_OUTDOOR_TRAFFICSIGNS_H
