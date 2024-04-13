#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>

#define BOARD_WIDTH 5
#define BOARD_HEIGHT 5

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
    cv::glob("../resources/chessboard/camera2/*.jpeg", images);

    //Матрица для хранения изображения в серых тонах
    cv::Mat gray;

    //Критерий остановки. Максимальное количество итераций - 30, точность - 0.001
    cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001);

// Цикл по всем путям
    for (const auto &image: images) {
        // Считываем изображение
        cv::Mat img = cv::imread(image);
        if (img.empty()) {
            continue;
        }

        // Преобразовываем в серое изображение
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        // Координаты углов
        std::vector<cv::Point2f> corners;

        // Выполняется поиск углов на шахматной доске.
        bool ret = cv::findChessboardCorners(gray, cv::Size(BOARD_HEIGHT, BOARD_WIDTH), corners,
                                             cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK);

        // Если нашли, то рисуем сетку на изображении
        if (ret) {
            // Метод увеличивает точность найденных координат углов
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);

            // Рисуем сетку
            cv::drawChessboardCorners(img, cv::Size(BOARD_HEIGHT, BOARD_WIDTH), corners, ret);

            // Показываем изображение с нарисованной сеткой
            cv::imshow("Chessboard Corners", img);
            cv::waitKey(0); // Ждем нажатия клавиши, чтобы закрыть окно
        }
    }

    return 0;
}
