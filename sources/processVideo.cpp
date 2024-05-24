#include <opencv2/opencv.hpp>
#include <string>
#include <stdexcept>
#include <fstream>
#include <opencv2/img_hash.hpp>
#include "../headers/processVideo.h"
#include "../headers/mapPoint.h"
#include "../headers/Map.h"
#include "../headers/triangulation.h"
#include "../headers/loopChecker.h"

cv::Mat image2;

/**
 * Метод проверяет количество мэтчей.
 * Проверяем все кадры с конца.
 * Если меньше чем MIN_MATCHES, берётся предыдущий.
 */
void goodMatchesCheck(const std::vector<cv::DMatch> &oldMatches,
                      const std::vector<cv::Mat> &framePool,
                      const cv::Mat &descriptors1) {
    std::vector<cv::KeyPoint> keyPoints2;
    std::vector<cv::DMatch> newMatches;
    cv::Mat descriptors2, secondGray;

    if (oldMatches.size() >= MIN_MATCHES) {
        numOfFirstUnusedFramePool = static_cast<int>(framePool.size());
        return;
    }

    for (int i = static_cast<int>(framePool.size()) - 2; i >= 0; i--) {
        image2 = framePool[i];
        cvtColor(image2, secondGray, cv::COLOR_BGR2GRAY);
        extractKeyPointsAndDescriptors(secondGray, keyPoints2, descriptors2);
        performFeatureMatching(descriptors1, descriptors2, newMatches);
        numOfFirstUnusedFramePool = i + 1;
        if (newMatches.size() >= MIN_MATCHES) {
            return;
        }
    }
}

/**
 * функция сдвига кадров на случай, если произошел сдвиг влево,
 * то есть был недостаток матчей в какой-то момент.
 */
std::vector<cv::Mat> shiftValues(const std::vector<cv::Mat> &frames) {
    std::vector<cv::Mat> remainingFrames;

    for (int i = numOfFirstUnusedFramePool; i < frames.size(); i++) {
        remainingFrames.push_back(frames[i]);
    }

    return remainingFrames;
}

/**
 * Получаем очередной пулл кадров.
 * Начинаем заполнять с первого, неиспользуемого(следующий после secondFrame).
 *
 * @param frames - Возвращаемое значение.
 * @param cap - Завхат.
 * @return -    0 - нашли пул кадров.
 *              1 - не нашли.
 */
int getFramesPool(std::vector<cv::Mat> &frames, cv::VideoCapture cap) {
    cv::Mat frame;

    frames = shiftValues(frames);

    for (int i = static_cast<int>(frames.size()); i < NUMBER_OF_FRAMES_IN_POOL; i++) {
        cap >> frame;

        if (frame.empty()) {
            return -1;
        }

        frames.push_back(frame);
    }

    return 0;
}

/**
 * Данный метод считывает значения матрицы камеры из файла и записывает их.
 *
 * @param pathToCalibrationCamera - путь до матрциы камеры.
 * @return - возвращает заполненную матрицу.
 */
cv::Mat readCameraMatrix(const cv::String &pathToCalibrationCamera) {
    //Пытаемся открыть файл с матрицой калибровки камеры.
    std::ifstream cameraMatrixFile(pathToCalibrationCamera);
    //Проверка на удачное открытие файла.
    if (!cameraMatrixFile.is_open()) {
        ("Path: " + pathToCalibrationCamera + '\n'
         + "Calibration file was not opened.");
    }

    //Создаём новую матрицу.
    cv::Mat cameraMatrix(3, 3, CV_64F);
    //Пытаемся заполнить матрицу.
    for (int i = 0; i < cameraMatrix.rows; i++) {
        for (int j = 0; j < cameraMatrix.cols; j++) {
            if (double value; cameraMatrixFile >> value) {
                cameraMatrix.at<double>(i, j) = value;
            } else {
                cameraMatrixFile.close();
                throw std::runtime_error("Camera matrix was not filled full.");
            }
        }
    }
    cameraMatrixFile.close();

    return cameraMatrix;
}

/**
 * Метод ответственные за начало обработки видео.
 */
void startProcessing() {
    cv::Mat cameraMatrix;
    try {
        //Читаем камеру матрицы из файла.
        cameraMatrix = readCameraMatrix(CALIBRATION_CAMERA_MATRIX_PATH);
    } catch (const std::runtime_error &e) {
        //В случае неудачи пробрасываем исключение на уровень выше.
        throw;
    }

    //Захват видеофайла и проверка на успешноое открытие файла.
    cv::VideoCapture cap(PATH_TO_VIDEOFILE);
    if (!cap.isOpened()) {
        throw std::runtime_error("Open video error.");
    }

    //Пошла лютая инициализация переменных//
    //Вектор из которого будет производится выборка наилучшего кадра для триангуляции.
    std::vector<cv::Mat> framePool;

    //Это для триангуляции//
    cv::Mat P1 = cv::Mat::eye(3, 4, CV_64F);
    cv::Mat P2 = cv::Mat::zeros(3, 4, CV_64F);
    cv::Mat image1;
    cv::Mat points3D;
    cap >> image1;

    std::vector<cv::KeyPoint> keyPoints1, keyPoints2;
    cv::Mat descriptors1, descriptors2;
    std::vector<cv::DMatch> oldMatches;

    extractKeyPointsAndDescriptors(image1, keyPoints1, descriptors1);

    //Это для карты//
    cv::Ptr<cv::img_hash::ImgHashBase> func;
    func = cv::img_hash::PHash::create();
    cv::Point2d start = cv::Point2d(0, 0);
    std::vector<cv::Point2d> points;
    cv::Mat imageHash1;
    func->compute(image1, imageHash1);
    MapPoint firstPoint = MapPoint(P1, points, imageHash1);
    Map map = Map();
    map.addPoint(firstPoint);

    bool isFirstOperation = true;

    while (true) {
        if (getFramesPool(framePool, cap) == -1) {
            break;
        }

        image2 = framePool[framePool.size() - 1];
        extractKeyPointsAndDescriptors(image2, keyPoints2, descriptors2);
        performFeatureMatching(descriptors1, descriptors2, oldMatches);

        goodMatchesCheck(oldMatches, framePool, descriptors1);
        oldMatches.clear();

        points3D = triangulation(image1, image2, cameraMatrix, P1, P2);

        map.addFeaturesPoint(points3D);

        cv::Mat imageHash2;
        func->compute(image2, imageHash2);
        MapPoint currentMapPoint = MapPoint(P2, points,imageHash2);

        map.addPoint(currentMapPoint);

        loopCheck(map.getMapPoints(), map.getIsRebuild());


        image1 = image2;
        descriptors1 = descriptors2;
        P1 = currentMapPoint.getGlobalCoordinates();

        if (cv::waitKey(1) == 27) {
            break;
        }
        map.showMap(1);


//        if (isFirstOperation) {
//            cv::waitKey(0);
//            isFirstOperation = false;
//        }
    }

    map.showMap(0);
    cap.release();
    cv::destroyAllWindows();
}
