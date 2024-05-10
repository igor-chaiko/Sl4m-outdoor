#include <opencv2/opencv.hpp>
#include "../headers/trafficSignsDetection.h"
#include <opencv2/img_hash.hpp>
#include "../headers/triangulation.h"

int main() {
    cv::VideoCapture cap("test7.MOV");
    cv::Ptr<cv::img_hash::ImgHashBase> hashFunc;
    hashFunc = cv::img_hash::PHash::create();
    std::vector<cv::Mat> total;
    std::vector<cv::Mat> hashes;
    std::vector<std::pair<int, cv::Mat>> hashes2;
    double similarity;
    bool differentFrame;
    int frameNum = 0, z = 0;
    cv::Mat image, hsv;

//    std::vector<std::pair<cv::Mat, cv::Mat>> signsDB = createDB("sample");

    while (true) {
        frameNum++;
        cap >> image;

        if (image.empty()) {
//            for (int i = 0; i < total.size(); i++) {
//                cv::imshow("image" + std::to_string(i+1), total[i]);
//            }

            if (cv::waitKey(0) == 27) {
                break;
            }
        }

//        cv::imshow("image", image);

        std::vector<cv::Mat> res = findSigns(image);

        for (const cv::Mat& currSign : res) {
            cv::Mat currHash;
            hashFunc->compute(currSign, currHash);
            differentFrame = true;

            std::cout << hashes2.size() << ": ";
            for (const std::pair<int, cv::Mat>& pair : hashes2) {
                int frameNumInPair = pair.first;
                similarity = hashFunc->compare(pair.second, currHash);
                std::cout << similarity << " ";
                if (similarity < 26 && frameNum - frameNumInPair < 150) {
                    differentFrame = false;
                    break;
                }
            }
            std::cout << std::endl;

            if (differentFrame) {
                total.push_back(currSign);
                std::pair<int, cv::Mat> pair(frameNum, currHash);
                cv::imshow("contour_" + std::to_string(++z), currSign);
                hashes2.push_back(pair);

                if (cv::waitKey(0) == 27){

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
