#include <opencv2/opencv.hpp>
#include <string>
#include "processVideo.h"

int entryPoint(const std::string& path) {
    cv::VideoCapture cap(0);
    cv::Mat frame;

    if (!cap.isOpened()) {
        std::cerr << "Ошибка при открытии видео!" << std::endl;
        return -1;
    }

    // в векторе хранятся 2 окна
    std::vector<cv::Mat> previousFrames;

    while (true) {
        if (previousFrames.size() == NUMBER_OF_FRAMES * 2) {
            //...
        }

        cap >> frame;

        if (frame.empty()) {
            std::cout << "Конец видео" << std::endl;
            break;
        }

        previousFrames.push_back(frame);

        if (previousFrames.size() > NUMBER_OF_FRAMES * 2) {
            previousFrames.erase(previousFrames.begin());
        }

        cv::imshow("Видосик", frame);

        if (cv::waitKey(30) == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}