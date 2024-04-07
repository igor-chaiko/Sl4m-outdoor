//
// Created by Ruslan Chudinov on 01.04.2024.
//

#ifndef SL4M_OUTDOOR_HASHING_H
#define SL4M_OUTDOOR_HASHING_H

#include <opencv2/opencv.hpp>
#include <opencv2/img_hash.hpp>

class Hashing {
private:
    static double test_one(const cv::Mat &a, const cv::Mat &b);
public:
    Hashing();

    void test1(const std::string& pathToVideoFile, long frameIndex);
};

#endif //SL4M_OUTDOOR_HASHING_H
