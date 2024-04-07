#include <opencv2/opencv.hpp>
#include "../headers/processVideo.h"

int main() {

//    Hashing hashing = Hashing();
//    try {
//        hashing.test1("../resources/testVideo2.MOV", 200);
//    } catch (const std::runtime_error& e) {
//        std::cerr << e.what() << std::endl;
//    }


    try {
        startProcessing();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }



    // Ваша исходная матрица
    cv::Mat matrix = (cv::Mat_<double>(3, 10) <<
                                              1.0, 2.0, -3.0, 4.0, 5.0, 6.0, -7.0, 8.0, 9.0, 0.0,
            -1.0, -9.0, -13.0, -14.0, -15.0, -16.0, -17.0, -18.0, -19.0, 9.999,
            2.0, 9.999, -23.0, 24.0, 25.0, 26.0, -27.0, 28.0, 29.0, 9.0);


    std::cout << "Матрица после удаления колонок:\n" << matrix << std::endl;

    std::cout << "Матрица после удаления колонок:\n" << matrix << std::endl;

    return 0;
}
