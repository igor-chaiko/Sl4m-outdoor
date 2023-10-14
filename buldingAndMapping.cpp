#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int buildingFeaturesInVideoUsingGFTT(string path) {
    VideoCapture cap(path);
    if (!cap.isOpened()) {
        std::cerr << "Ошибка при открытии видео!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    cv::Mat grayFrame;
//    Point2f - это структура или класс (в зависимости от того, как она определена в вашем коде), используемая в
//    библиотеке OpenCV для представления двухмерных точек на плоскости. Эта структура обычно используется для хранения
//    координат (x, y) точек, которые могут представлять, например, пиксели на изображении.
    std::vector<cv::Point2f> corners;

    while (true) {
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        cv::goodFeaturesToTrack(grayFrame, corners, 100, 0.05, 15);

        for (const cv::Point2f &corner : corners) {
            cv::circle(frame, corner, 5, cv::Scalar(0, 0, 255), -1);
        }

        cv::imshow("Video with Features", frame);

        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    return 0;
}

int featuresByOrb(string path) {
    VideoCapture cap(path);
    if (!cap.isOpened()) {
        std::cerr << "Ошибка при открытии видео!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    cv::Mat grayFrame;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    cv::Ptr<cv::ORB> orb = cv::ORB::create(500);

    while (true) {
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        orb->detectAndCompute(grayFrame, cv::noArray(), keypoints, descriptors);

        cv::drawKeypoints(frame, keypoints, frame, cv::Scalar(0, 0, 255));

        cv::imshow("Video with Features", frame);

        if (cv::waitKey(1) == 27) {
            break;
        }
    }
    return 0;
}

int unionOfTwo(string path) {
    VideoCapture cap(path);
    if (!cap.isOpened()) {
        std::cerr << "Ошибка при открытии видео!" << std::endl;
        return -1;
    }

    cv::Mat frame1, frame2;
    cv::Mat grayFrame1, grayFrame2;
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    cv::Ptr<cv::ORB> orb = cv::ORB::create(500);
    std::vector<cv::Point2f> corners1, corners2;
    Mat img;
    cap >> frame1;
    cv::cvtColor(frame1, grayFrame1, cv::COLOR_BGR2GRAY);

    cv::goodFeaturesToTrack(grayFrame1, corners1, 200, 0.05, 15);

    for (const cv::Point2f &corner : corners1) {
        keypoints1.emplace_back(corner, 1.0);
    }

    orb->compute(grayFrame1, keypoints1, descriptors1);

    while (true) {
        keypoints2.clear();
        cap >> frame2;

        if (frame2.empty()) {
            break;
        }

        cv::cvtColor(frame2, grayFrame2, cv::COLOR_BGR2GRAY);

        cv::goodFeaturesToTrack(grayFrame2, corners2, 200, 0.05, 15);

        for (const cv::Point2f &corner : corners2) {
            keypoints2.emplace_back(corner, 1.0);
        }

        orb->compute(grayFrame2, keypoints2, descriptors2);

        BFMatcher matcher1(NORM_HAMMING);

        std::vector<DMatch> matches;

        matcher1.match(descriptors1, descriptors2, matches);

        drawMatches(frame1, keypoints1, frame2, keypoints2, matches, img);

        cv::imshow("Video with Features", img);

        if (cv::waitKey(1) == 27) {
            break; 
        }
        descriptors1 = descriptors2;
        frame1 = frame2;
        keypoints1 = keypoints2;
        grayFrame1 = grayFrame2;
    }

    return 0;
}