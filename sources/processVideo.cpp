#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include "../headers/processVideo.h"
#include "../headers/mapPoint.h"
#include "../headers/Map.h"
#include "../headers/triangulation.h"

/**
 * Глобальная переменная для захвата видео-потока.
 */
cv::VideoCapture cap;

int numOfFirstUnusedFromPool;
cv::Mat image2;

// Function to check number of matches. If less than MIN_MATCHES, take next
// if u delete function call, everything still will work
void goodMatchesCheck(std::vector<cv::DMatch>& oldMatches, std::vector<cv::Mat> &framePool,
                                         cv::Mat descriptors1) {
    std::vector<cv::KeyPoint> keyPoints2;
    std::vector<cv::DMatch> newMatches;
    cv::Mat descriptors2, secondGray;

    if (oldMatches.size() >= MIN_MATCHES) {
        numOfFirstUnusedFromPool = framePool.size();
        return;
    }

    for (int i = framePool.size() - 2; i >= 0; i--) {
        image2 = framePool[i];
        cv::cvtColor(image2, secondGray, cv::COLOR_BGR2GRAY);
        extractKeyPointsAndDescriptors(secondGray, keyPoints2, descriptors2);
        performFeatureMatching(descriptors1, descriptors2, newMatches);

        numOfFirstUnusedFromPool = i + 1;
//        std::cout << i << std::endl;
        if (newMatches.size() >= MIN_MATCHES) {
            return;
        }
    }

}

std::vector<cv::Mat> shiftValues(std::vector<cv::Mat> &frames) {
    std::vector<cv::Mat> remainingFrames;

    for (int i = numOfFirstUnusedFromPool; i < frames.size(); i++) {
        remainingFrames.push_back(frames[i]);
    }

    return remainingFrames;
}

/**
 * Получаем очередной пулл кадров.
 * Начинаем заполнять с первого, неиспользуемого(следующий после secondFrame).
 */
int getFramesPool(std::vector<cv::Mat> &frames) {
    cv::Mat frame;

    frames = shiftValues(frames);

    for (int i = frames.size(); i < NUMBER_OF_FRAMES_IN_POOL; i++) {
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "frames are over" << std::endl;
            return -1;
        }

        frames.push_back(frame);
    }

    return 0;
}

/**
 * заполняем матрицу отколиброванными значениями.
 * @param inputFile заготовленный файл.
 * @return заполненная матрица.
 */
cv::Mat fillCameraMatrix(std::ifstream& inputFile) {
    cv::Mat cameraMatrix(3, 3, CV_64F);
    cv::Mat empty;

    for (int i = 0; i < cameraMatrix.rows; i++) {
        for (int j = 0; j < cameraMatrix.cols; j++) {
            double value;
            if (inputFile >> value) {
                cameraMatrix.at<double>(i, j) = value;
            } else {
                std::cerr << "No value" << std::endl;
                return empty;
            }
        }
    }

    return cameraMatrix;
}

/**
 * Точка входа в приложение.
 */
int entryPoint(const std::string &path) {

    std::ifstream cameraMatrixFile("../resources/CalibratedCamera.txt");

    if (!cameraMatrixFile.is_open()) {
        std::cerr << "Unable to open the file" << std::endl;
        return -1;
    }

    cv::Mat cameraMatrix = fillCameraMatrix(cameraMatrixFile);
    if (cameraMatrix.empty()) {
        return -1;
    }

    cameraMatrixFile.close();
    cap.open(path);

    if (!cap.isOpened()) {
        std::cerr << "Open video error" << std::endl;
        return -1;
    }

    std::vector<cv::Mat> framePool;

    cv::Mat P1 = cv::Mat::eye(3, 4, CV_64F), P2 = cv::Mat::zeros(3, 4, CV_64F);
    cv::Mat image1;
    cv::Mat points3D;

    cap >> image1;
    std::vector<cv::KeyPoint> keyPoints1, keyPoints2;
    cv::Mat descriptors1, descriptors2;
    std::vector<cv::DMatch> oldMatches;

    extractKeyPointsAndDescriptors(image1, keyPoints1, descriptors1);

    cv::Point2d start = cv::Point2d(0, 0);
    cv::Point2d vector = cv::Point2d(1, 0);
    std::vector<cv::Point2d> points;

    MapPoint firstPoint = MapPoint(start, points, vector);
    Map map = Map();
    map.addPoint(firstPoint);

    int fstOperation = 1;

    while (true) {

        if (getFramesPool(framePool) == -1) {
            break;
        }

        image2 = framePool[framePool.size()-1];
        extractKeyPointsAndDescriptors(image2, keyPoints2, descriptors2);
        performFeatureMatching(descriptors1, descriptors2, oldMatches);

        auto b = goodMatchesCheck(oldMatches, framePool, descriptors1);

        //cv::imshow(" ", image2);
        points3D = triangulation(image1, image2, cameraMatrix, P1, P2);

        //на первой итерации координаты плохие
        if (fstOperation == 1) {
            fstOperation = 0;
        } else {
            map.addFeaturesPoint(points3D);
        }

        double y = P2.at<double>(2, 3);
        double z = P2.at<double>(1, 3);
        double x = P2.at<double>(0, 3);

        cv::Point2d currentPositionInSpace = cv::Point2d(x, y);
        MapPoint currentMapPoint = MapPoint(currentPositionInSpace, points, vector);

        map.addPoint(currentMapPoint);

        image1 = image2;
        descriptors1 = descriptors2;
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