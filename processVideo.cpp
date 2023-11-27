#include <opencv2/opencv.hpp>
#include <string>
#include "processVideo.h"

std::vector<cv::Mat> init() {
    std::vector<cv::Mat> frames;
    frames.reserve(NUMBER_OF_FRAMES);

    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        frames.emplace_back();
    }

    return frames;
}

int getFramesPull(std::vector<cv::Mat> &frames, cv::VideoCapture cap) {
    cv::Mat frame;

    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        cap >> frame;

        if (frame.empty() && i == 0) {
            std::cerr << "Нет кадра( " << std::endl;
            return -1;
        }

        frames[i] = frame;
    }

    return 0;
}

int entryPoint(const std::string &path) {
    cv::VideoCapture cap(path);

    if (!cap.isOpened()) {
        std::cerr << "Ошибка при открытии видео!" << std::endl;
        return -1;
    }

    std::vector<cv::Mat> firstWindow = init(), secondWindow = init();

    int status = getFramesPull(firstWindow, cap);
    status = getFramesPull(secondWindow, cap);

    while (true) {

        //...

        firstWindow = secondWindow;
        status = getFramesPull(secondWindow, cap);
        //...

        if (cv::waitKey(30) == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}