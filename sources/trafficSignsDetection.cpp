#include <opencv2/opencv.hpp>
#include "../headers/trafficSignsDetection.h"
cv::Mat image_hsv, image_mask, image_dilate;

std::vector<cv::Mat> findSigns(cv::Mat image) {
    std::vector<cv::Mat> res;
        // переход в hsv
//        cv::Mat image_hsv;
        cv::cvtColor(image, image_hsv, cv::COLOR_BGR2HSV);
//        cv::imshow("hsv", image_hsv);

        // создание маски
//        cv::Mat image_mask;
        cv::Scalar lower_bound(0, 122, 50);
        cv::Scalar upper_bound(255, 255, 255);
        cv::inRange(image_hsv, lower_bound, upper_bound, image_mask);
//        cv::imshow("mask", image_mask);

        // "расширение" маски - нужно для заполнения разрывов, не факт, что нам нужно
//        cv::Mat image_dilate;
        cv::dilate(image_mask, image_dilate, cv::Mat(), cv::Point(-1, -1), 2);
//        cv::imshow("dilated", image_dilate);

        // извлекаем нужные контуры  && area < 40000
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(image_dilate.clone(), contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // отрисовка + фильтрация
        std::vector<std::vector<cv::Point>> sortedContours;
        int i = 0;
        for (const auto & contour : contours) {
            double area = cv::contourArea(contour);
//            std::cout << area << std::endl;
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

                if ((approx.size() == 4 && k1 > 0.8 && k1 < 1.2) || (approx.size() == 3 && k2 > 0.8 && k2 < 1.2)
                || (circularity > 0.7 && circularity < 1.2)) {
                    cv::destroyAllWindows();

                    sortedContours.push_back(contour);
                    cv::Rect bounding_rect = cv::boundingRect(contour);
                    cv::Mat contour_image = image(bounding_rect).clone();
//                    cv::imshow("mask", image_dilate);
//                    cv::imshow("contour_" + std::to_string(++i), contour_image);
                    res.push_back(contour_image);
//                    std::cout << circularity << std::endl;
//                    std::cout << approx.size() << std::endl;
//                    std::cout << k1 << std::endl;
//                    std::cout << k2 << std::endl;
//                    std::cout << area << std::endl;
//                    std::cout << "-----------------\n";

//                    if (cv::waitKey(0) == 27) {
//                        continue;
//                    }
                }

                // выводим по контуру на изображение
//                cv::Rect bounding_rect = cv::boundingRect(contour);
//                cv::Mat contour_image = image(bounding_rect).clone();
//                cv::imshow("contour_" + std::to_string(i++), contour_image);
            }
        }
        contours.clear();
        hierarchy.clear();
    return res;

//        cv::drawContours(image, sortedContours, -1, cv::Scalar(70, 100, 50), 3);
//        cv::imshow("result", image);


}
