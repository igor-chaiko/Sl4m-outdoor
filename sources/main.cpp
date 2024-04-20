#include <opencv2/opencv.hpp>
#include "../headers/trafficSignsDetection.h"
#include <opencv2/img_hash.hpp>

int main() {
    cv::VideoCapture cap("test3.MOV");
    cv::Ptr<cv::img_hash::ImgHashBase> func;
    func = cv::img_hash::PHash::create();
    std::vector<cv::Mat> total;
    std::vector<cv::Mat> hashes;
    double similarity;
    bool flag;

    while (true) {
        cv::Mat image;
        cap >> image;

        if (image.empty()) {
            for (int i = 0; i < total.size(); i++) {
                cv::imshow("image" + std::to_string(i+1), total[i]);
            }

            if (cv::waitKey(0) == 27) {
                break;
            }
        }

        std::vector<cv::Mat> res = findSigns(image);

        for (cv::Mat img : res) {
            cv::Mat currHash;
            func->compute(img, currHash);
            flag = true;

            for (cv::Mat hash : hashes) {
                similarity = func->compare(hash, currHash);

                if (similarity < 30) {
                    flag = false;
                    break;
                }
            }

            if (flag) {
                total.push_back(img);
                hashes.push_back(currHash);
            }
        }
    }

//    cv::Mat image = cv::imread("1.jpg");
//    findSigns(image);

//    Hashing hashing = Hashing();
//    try {
//        hashing.test1("../resources/testVideo2.MOV", 200);
//    } catch (const std::runtime_error& e) {
//        std::cerr << e.what() << std::endl;
//    }

//    try {
//        startProcessing();
//    } catch (const std::runtime_error& e) {
//        std::cerr << e.what() << std::endl;
//    }
//
//
//
//    // Ваша исходная матрица
//    cv::Mat matrix = (cv::Mat_<double>(3, 10) <<
//                                              1.0, 2.0, -3.0, 4.0, 5.0, 6.0, -7.0, 8.0, 9.0, 0.0,
//            -1.0, -9.0, -13.0, -14.0, -15.0, -16.0, -17.0, -18.0, -19.0, 9.999,
//            2.0, 9.999, -23.0, 24.0, 25.0, 26.0, -27.0, 28.0, 29.0, 9.0);
//
//
//    std::cout << "Матрица после удаления колонок:\n" << matrix << std::endl;
//
//    std::cout << "Матрица после удаления колонок:\n" << matrix << std::endl;

    return 0;
}