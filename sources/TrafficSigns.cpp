#include "../headers/TrafficSigns.h"
#include <opencv2/img_hash.hpp>

TrafficSigns::TrafficSigns() = default;

std::vector<std::pair<cv::Mat, std::string>> TrafficSigns::findSigns(const cv::Mat &image) {
    std::vector<std::pair<cv::Mat, std::string>> res;
    cv::Mat image_hsv, dilateMask;
    std::vector<cv::Mat> masks;

    // переход в hsv
    cv::cvtColor(image, image_hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower_red(0, 10, 10);
    cv::Scalar upper_red(10, 255, 255);
    cv::Mat mask1, mask2;
    cv::inRange(image_hsv, lower_red, upper_red, mask1);
    cv::Scalar lower_red2(170, 10, 10);
    cv::Scalar upper_red2(180, 255, 255);
    cv::inRange(image_hsv, lower_red2, upper_red2, mask2);
    cv::Mat red_mask;
    cv::addWeighted(mask1, 1.0, mask2, 1.0, 0.0, red_mask);
    masks.push_back(red_mask);
    cv::imshow("r_mask", red_mask);
    cv::waitKey(1);

    cv::Scalar lower_yellow(35, 70, 50);
    cv::Scalar upper_yellow(40, 255, 255);
    cv::Mat yellow_mask;
    cv::inRange(image_hsv, lower_yellow, upper_yellow, yellow_mask);
    masks.push_back(yellow_mask);

    cv::Scalar lower_blue(100, 70, 50);
    cv::Scalar upper_blue(120, 255, 255);
    cv::Mat blue_mask;
    cv::inRange(image_hsv, lower_blue, upper_blue, blue_mask);
    masks.push_back(blue_mask);

    for (const cv::Mat& mask : masks) {
        // "расширение" маски - нужно для заполнения разрывов, не факт, что нам нужно
        cv::dilate(mask, dilateMask, cv::Mat(), cv::Point(-1, -1), 2);

        // извлекаем нужные контуры
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(dilateMask.clone(), contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (const auto & contour : contours) {
            double area = cv::contourArea(contour);
            if (AREA_LOWER_BOUND < area && area < AREA_UPPER_BOUND) {
                std::vector<cv::Point> approx;
                cv::approxPolyDP(contour, approx, 0.03 * cv::arcLength(contour, true), true);

                double perimeter = cv::arcLength(contour, true);
                double r = perimeter / (2 * CV_PI);
                double circularity = (area*2)/(perimeter*r);

                double a = sqrt(area);
                double k1 = (4 * area) / (perimeter * a);

                double b = perimeter / 3;
                double k2 = ( b * b * sqrt(3) / 4) / area;

                if ((k1 > 0.8 && k1 < 1.2) || (k2 > 0.8 && k2 < 1.2)
                    || (circularity > 0.75 && circularity < 1.2)) {

                    cv::Rect bounding_rect = cv::boundingRect(contour);

                    cv::Mat contour_image = image(bounding_rect).clone();
                    std::string location;
                    if (bounding_rect.x + bounding_rect.width/2 < image.cols) {
                        location = "left";
                    } else {
                        location = "right";
                    }

                    res.emplace_back(contour_image, location);
                }
            }
        }
    }

    return res;
}

std::vector<std::pair<cv::Mat, std::string>> TrafficSigns::processSigns(const cv::Mat& image) {
    bool newSign;
    double currentSimilarity;
    cv::Ptr<cv::img_hash::ImgHashBase> hashFunc;
    hashFunc = cv::img_hash::PHash::create();
    frameNum++;
    std::vector<std::pair<cv::Mat, std::string>> subTotal = TrafficSigns::findSigns(image);
    std::vector<std::pair<cv::Mat, std::string>> result;

    for (const std::pair<cv::Mat, std::string>& signPair : subTotal) {
        cv::Mat sign = signPair.first;
        newSign = true;
        cv::Mat currHash;
        hashFunc->compute(sign, currHash);

        for (const std::pair<int, cv::Mat>& pair : metSigns) {
            int frameNumInPair = pair.first;
            currentSimilarity = hashFunc->compare(pair.second, currHash);
            if (currentSimilarity < SIMILARITY && frameNum - frameNumInPair < DISTANCE_BETWEEN_TWO) {
                newSign = false;
                break;
            }
        }

        if (newSign) {
            metSigns.emplace_back(frameNum, currHash);
            result.emplace_back(sign, signPair.second);
        }
    }

    return result;
}
