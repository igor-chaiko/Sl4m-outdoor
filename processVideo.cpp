#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include "processVideo.h"
#include "mapPoint.h"
#include "Map.h"
#include "triangulation.h"

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
            std::cerr << "I have no frame( " << std::endl;
            return -1;
        }

        frames[i] = frame;
    }

    return 0;
}

int entryPoint(const std::string &path) {
    std::ifstream inputFile("CalibratedCamera.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Unable to open the file(" << std::endl;
        return -1;
    }

    cv::Mat cameraMatrix(3, 3, CV_64F);

    for (int i = 0; i < cameraMatrix.rows; i++) {
        for (int j = 0; j < cameraMatrix.cols; j++) {
            double value;
            if (inputFile >> value) {
                cameraMatrix.at<double>(i, j) = value;
            } else {
                std::cerr << "No value((" << std::endl;
                return -1;
            }
        }
    }

    inputFile.close();

    cv::VideoCapture cap(path);

    if (!cap.isOpened()) {
        std::cerr << "Open video error!" << std::endl;
        return -1;
    }

    std::vector<cv::Mat> firstWindow = init(), secondWindow = init();

    int status = getFramesPull(firstWindow, cap);

    cv::Mat P1 = cv::Mat::eye(3, 4, CV_64F), P2 = cv::Mat::zeros(3, 4, CV_64F);
    cv::Mat image1 = firstWindow[0], image2;
    cv::Mat points3D;



    cv::Point2d start = cv::Point2d(0, 0);
    cv::Point2d vector = cv::Point2d(1, 0);
    std::vector<cv::Point2d> points;

    MapPoint firstPoint = MapPoint(start, points, vector);
    Map map = Map();
    map.addPoint(firstPoint);

    int fstOperation = 1;
    while (true) {

        if(getFramesPull(secondWindow, cap) == -1) {
            break;
        }

        image2 = secondWindow[0];

        cv::imshow(" ", image2);
        points3D = triangulation(image1, image2, cameraMatrix, P1, P2);

        std::cout << P2 << std::endl;


        //std::cout << points3D.size << std::endl;

        //на первой итерации координаты мега кал
        if (fstOperation == 1) {
            fstOperation = 0;
        } else {
            map.addFeaturesPoint(points3D);
        }

        //std::cout << P2 << std::endl;

        double y = -P2.at<double>(2, 3);
        double z = P2.at<double>(1, 3);
        double x = P2.at<double>(0, 3);

        cv::Point2d currentPositionInSpace = cv::Point2d(x, y);
        MapPoint currentMapPoint = MapPoint(currentPositionInSpace, points, vector);

        map.addPoint(currentMapPoint);

        firstWindow = secondWindow;
        image1 = image2;
        P1 = P2;

        if (cv::waitKey(30) == 27) {
            break;
        }
        map.showMap(1);
    }


    map.showMap(0);




    cap.release();
    cv::destroyAllWindows();

    return 0;
}