#include <opencv2/opencv.hpp>
#include "../headers/trafficSignsDetection.h"
#include <opencv2/img_hash.hpp>

int main() {

    cv::VideoCapture cap("test5.MOV");
    cv::Ptr<cv::img_hash::ImgHashBase> func;
    func = cv::img_hash::PHash::create();
    std::vector<cv::Mat> total;
    std::vector<cv::Mat> hashes;
    std::vector<std::pair<int, cv::Mat>> hashes2;
    double similarity;
    bool flag;
    int j = 0, z = 0;
    cv::Mat image;

    while (true) {
        j++;
        cap >> image;
//        image = image + cv::Scalar(5, 5, 5);

        if (image.empty()) {
//            for (int i = 0; i < total.size(); i++) {
//                cv::imshow("image" + std::to_string(i+1), total[i]);
//            }

            if (cv::waitKey(0) == 27) {
                break;
            }
        }

        cv::imshow("image", image);
        std::vector<cv::Mat> res = findSigns(image);

        for (const cv::Mat& img : res) {
            cv::Mat currHash;
            func->compute(img, currHash);
            flag = true;

            for (const std::pair<int, cv::Mat>& pair : hashes2) {
                int pairJ = pair.first;
                similarity = func->compare(pair.second, currHash);
                if (similarity < 35 && j - pairJ < 100) {
                    flag = false;
                    break;
                }
            }

            if (flag) {
                total.push_back(img);
                std::pair<int, cv::Mat> pair(j, currHash);
                cv::imshow("contour_" + std::to_string(++z), img);
                if (cv::waitKey(0) == 27){
                    hashes2.push_back(pair);
                }
            }
        }

        cv::waitKey(1);
        cv::destroyWindow("image");
    }

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