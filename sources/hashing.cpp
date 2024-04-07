#include "../headers/hashing.h"


void Hashing::test1(const std::string& pathToVideoFile, long frameIndex) {
    cv::VideoCapture cap(pathToVideoFile);
    if (!cap.isOpened()) {
        throw std::runtime_error("Open video error.");
    }

    cv::Mat frame;
    cap.set(cv::CAP_PROP_POS_FRAMES, static_cast<double> (frameIndex));
    if (!cap.read(frame)) {
        throw std::runtime_error("Frame read error.");
    }

    cv::imshow("Frame", frame);
//    int k = cv::waitKey(1000);
//    if (k == 's') {
//        std::cout << "here will be save a photo." << std::endl;
//    }
    std::string filename = "../testResults/(start)image_" + std::to_string(frameIndex) + ".jpg";
    if (!cv::imwrite(filename, frame)) {
        std::cerr << "save error" << std::endl;
    }


    cv::Mat frameForComparing;
    cap.set(cv::CAP_PROP_POS_FRAMES, static_cast<double> (0));
    int j = 0;
    while(cap.read(frameForComparing)) {
        double resultOfComparing = test_one(frame, frameForComparing);
        if (resultOfComparing <= 11.5 && j - frameIndex >= 300) {
            std::cout << j << std::endl;

            std::string filename = "../testResults/image_" + std::to_string(j) + ".jpg";
            if (!cv::imwrite(filename, frameForComparing)) {
                std::cerr << "save error" << std::endl;
            }
//~10 сек

        }
        j++;
    }
}

double Hashing::test_one(const cv::Mat &a, const cv::Mat &b) {
    cv::Mat hashA, hashB;
    cv::Ptr<cv::img_hash::ImgHashBase> func;
    func = cv::img_hash::PHash::create();


    func->compute(a, hashA);

    func->compute(b, hashB);
    double resultOfComparing = func->compare(hashA, hashB);
    return resultOfComparing;
}
