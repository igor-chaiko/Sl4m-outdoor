#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>

#define BOARD_WIDTH 5
#define BOARD_HEIGHT 7

int calibration() {
    //Координаты квадратиков на шахматной доске в системе координат самой шахаматной доски(z == 0)
    cv::Mat objP = cv::Mat::zeros(BOARD_WIDTH * BOARD_HEIGHT, 3, CV_32FC1);
    for (int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; ++i) {
        objP.at<float>(i, 0) = static_cast<float>(i % BOARD_HEIGHT);//заполняем координаты по x
        objP.at<float>(i, 1) = static_cast<float>(i) / BOARD_HEIGHT;//заполняем координаты по y
    }

    //Трёхмерные координаты точки
    std::vector<cv::Mat> objPoints;
    //Вектор векторов по которым мы будем калиброваться
    std::vector<std::vector<cv::Point2f>> imgPoints;

    //Вектор названий изображения
    std::vector<cv::String> images;
    //Получаем пути изображений по которым будем калиброваться
    cv::glob("../CalibImg/*.jpg", images);

    //Матрица для хранения изображения в серых тонах
    cv::Mat gray;

    //Критерий остановки. Максимальное количество итераций - 30, точность - 0.001
    cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001);

    //Цикл по всем путям
    for (const auto &image: images) {

        //Считываем изображение
        cv::Mat img = cv::imread(image);
        if (img.empty()) {
            continue;
        }

        //преобразовываем в серое изображение
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        //Координаты углов
        std::vector<cv::Point2f> corners;

        //Выполняется поиск углов на шахматной доске.
        bool ret = cv::findChessboardCorners(gray, cv::Size(BOARD_HEIGHT, BOARD_WIDTH), corners,
                                             cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK);
        //Если нашли, то запоминаем результат
        if (ret) {
            objPoints.push_back(objP);
            //Метод увеличивает точность найденных координат углов
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);
            imgPoints.push_back(corners);
        }
    }

    //Матрица камеры, Коэфициенты дисторсии
    cv::Mat cameraMatrix, distCoefficients;
    //Векторы вращения  трансляции. В алгоритме не используются(заглушки)
    std::vector<cv::Mat> rvecs, tvecs;

    //Функция ради которого всё затевалось
    cv::calibrateCamera(objPoints, imgPoints, gray.size(), cameraMatrix, distCoefficients, rvecs, tvecs);

    //Вывод camera matrix и матрицы коэффициентов дисторсии
    std::cout << "Camera Matrix:\n" << cameraMatrix << "\n\n";
    std::cout << "Distortion Coefficients:\n" << distCoefficients << "\n";

    //Открытие файла на запись
    std::ofstream calib("CalibratedCamera.txt");
    if (calib.is_open()) {
        //Записываем Матрицу
        calib << "Camera Matrix: ";
        for (int i = 0; i < cameraMatrix.rows; ++i) {
            for (int j = 0; j < cameraMatrix.cols; ++j) {
                calib << cameraMatrix.at<double>(i, j) << " ";
            }
        }
        calib << "\n";
        calib << "Distorsia coefficients: ";
        for (int i = 0; i < distCoefficients.rows; ++i) {
            for (int j = 0; j < distCoefficients.cols; ++j) {
                calib << distCoefficients.at<double>(i, j) << " ";
            }
        }
        calib << "\n";
        calib.close();
    } else {
        std::cerr << "Unable to open CalibratedCamera.txt for writing.\n";
        return -1;
    }

    return 0;
}

