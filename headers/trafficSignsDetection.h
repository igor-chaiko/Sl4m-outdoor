#include "opencv2/opencv.hpp"

#ifndef SL4M_OUTDOOR_TRAFICSIGNSDETECTION_H
#define SL4M_OUTDOOR_TRAFICSIGNSDETECTION_H

std::vector<cv::Mat> findSigns(const cv::Mat& image);
cv::Mat convolution(cv::Mat& source, const cv::Mat& kernel);
double euclideanDistance(const cv::Mat& A, const cv::Mat& B);
#define AREA_LOWER_BOUND 7500
#define AREA_UPPER_BOUND 40000

#endif //SL4M_OUTDOOR_TRAFICSIGNSDETECTION_H

//h(оттенок/цвет) - 0 соответствует красному, 120 - зеленому, 240 - синему
//
//s(насыщенность) - параметр указывает на "чистоту" цвета. Насыщенность описывает, насколько цвет является "чистым",
// без белого света. Он измеряется в процентах от 0% (серый) до 100% (полный цвет).
//
//v(значение) - Этот параметр указывает на яркость цвета. Значение определяет, насколько ярким или темным будет цвет.
// Он также измеряется в процентах от 0% (черный) до 100% (белый)
