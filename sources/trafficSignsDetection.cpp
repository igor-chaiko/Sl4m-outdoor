#include <opencv2/opencv.hpp>
#include "../headers/trafficSignsDetection.h"
#include "../headers/triangulation.h"

std::vector<cv::Mat> findSigns(const cv::Mat& image) {
    std::vector<cv::Mat> res;
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
    cv::imshow("video_r", red_mask);

    for (const cv::Mat& mask : masks) {
        // "расширение" маски - нужно для заполнения разрывов, не факт, что нам нужно
        cv::dilate(mask, dilateMask, cv::Mat(), cv::Point(-1, -1), 2);

        // извлекаем нужные контуры
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(dilateMask.clone(), contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

//        cv::imshow("mask", dilateMask);
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
                    cv::Rect bounding_rect_s = bounding_rect;
                    if (bounding_rect_s.width > bounding_rect_s.height) {
                        bounding_rect_s.height += (bounding_rect_s.width-bounding_rect_s.height);
                    } else if (bounding_rect_s.width < bounding_rect_s.height) {
                        bounding_rect_s.width += (bounding_rect_s.height-bounding_rect_s.width);
                    }

                    if (bounding_rect_s.width % 2 != 0) {
                        bounding_rect_s.width++;
                        bounding_rect_s.height++;
                    }

                    cv::Mat contour_image = image(bounding_rect).clone();
//                cv::imshow("contour_" + std::to_string(++i), contour_image);
                    res.push_back(contour_image);
//                std::cout << circularity << std::endl;
//                std::cout << approx.size() << std::endl;
//                std::cout << k1 << std::endl;
//                std::cout << k2 << std::endl;
//                std::cout << area << std::endl;
//                std::cout << "-----------------\n";

//                if (cv::waitKey(0) == 27) {
//                    continue;
//                }
                }
            }
        }
    }

    return res;
}

cv::Mat convolution(cv::Mat& source, const cv::Mat& kernel) {
    cv::Mat res(source.rows-kernel.rows+1, source.cols-kernel.cols+1, CV_64F);
    double sum = 0;

    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            for (int n = 0; n < kernel.rows; n++) {
                for (int m = 0; m < kernel.cols; m++) {
                    double kernelV = kernel.at<double>(n, m);
                    double sourceV = static_cast <double >(source.at<uchar>(i+n, j+m));

                    sum += kernelV * sourceV;
                }
            }

            res.at<double>(i, j) = sum;
            sum = 0;
        }
    }

    return res;
}

double euclideanDistance(const cv::Mat& A, const cv::Mat& B) {
    assert(A.size() == B.size());

    cv::Mat vecA(1, A.cols * A.rows, CV_64F);
    cv::Mat vecB(1, A.cols * A.rows, CV_64F);

    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++) {
            vecA.at<double>(0, i*A.cols+j) = static_cast <double > (A.at<uchar>(i, j));
            vecB.at<double>(0, i*A.cols+j) = static_cast <double > (B.at<uchar>(i, j));
        }
    }

//    std::cout << "A: " << vecA << std::endl;
//    std::cout << "B: " << vecB << std::endl;
    double distance = cv::norm(vecA, vecB, cv::NORM_L2); // cv::NORM_L2 - Евклидово расстояние
//    std::cout << distance << std::endl;

    return distance;
}
