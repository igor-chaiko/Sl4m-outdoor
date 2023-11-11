#include <opencv2/opencv.hpp>
#include "DrawMap.h"
void isImageOpen(const cv::Mat& image) {
    if (image.empty()) {
        std::cerr << "Error: Unable to open image." << std::endl;
        exit(1);

    } else {
        std::cout << "Image successfully opened." << std::endl;
    }
}

int main() {



    return 0;


    /* Итак, сначала нам нужно найти как можно больше возможных совпадений
     * между двумя изображениями, чтобы найти фундаментальную
     * матрицу.
     *
     * Для этого мы используем дескрипторы SIFT
     * с помощью средства сопоставления на основе
     * FLANN и теста соотношения.
     * */


    /* Открываем
     *      левое изображение цветное
     *      правое изображение цветное
     *      левое изображение серое
     *      правое изображение серое
     *
     *      + делаем проверку, что оно открылось
     */
    std::string path_file_left = "Sources/left_photo4.jpg";
    std::string path_file_right = "Sources/right_photo4.jpg";
    cv::Mat left_photo_color = cv::imread(path_file_left, cv::IMREAD_COLOR);
    isImageOpen(left_photo_color);
    cv::Mat right_photo_color = cv::imread(path_file_right, cv::IMREAD_COLOR);
    isImageOpen(right_photo_color);
    cv::Mat left_photo_grayscale = cv::imread(path_file_left, cv::IMREAD_GRAYSCALE);
    isImageOpen(left_photo_grayscale);
    cv::Mat right_photo_grayscale = cv::imread(path_file_right, cv::IMREAD_GRAYSCALE);
    isImageOpen(right_photo_grayscale);

    //выводит все 4 картинки
    //только пользователь нажимает клавишу, они закрываются
//    cv::imshow("Left Photo Color", left_photo_color);
//    cv::imshow("Right Photo Color", right_photo_color);
//    cv::imshow("Left Photo Grayscale", left_photo_grayscale);
//    cv::imshow("Right Photo Grayscale", right_photo_grayscale);
//
//    cv::waitKey(0);

    // Теперь можно использовать объект "sift" для SIFT операций
    cv::Ptr<cv::SIFT> sift = cv::SIFT::create();


    // Находим кейпоинты и дескрипторы для изображения 1
    std::vector<cv::KeyPoint> key_points_left_img;
    cv::Mat descriptors_left_img;
    sift->detectAndCompute(left_photo_grayscale,
                           cv::noArray(), key_points_left_img, descriptors_left_img);

    // Находим ключевые точки и дескрипторы для изображения 2
    std::vector<cv::KeyPoint> key_points_right_img;
    cv::Mat descriptors_right_img;
    sift->detectAndCompute(right_photo_grayscale,
                           cv::noArray(), key_points_right_img, descriptors_right_img);

    //Создём объект DiscriptorMatcher
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);

    // Выполнение k-ближайших соседей
    std::vector< std::vector<cv::DMatch> > knn_matches;
    matcher->knnMatch( descriptors_left_img, descriptors_right_img, knn_matches, 2);

    // устанавливаем порог для лоутеста
    const float ratio_thresh = 0.5f;
    std::vector<cv::DMatch> good_matches;

    // Фильтрация сопоставлений с использованием теста Лоу
    for (size_t i = 0; i < knn_matches.size(); i++) {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
            good_matches.push_back(knn_matches[i][0]);
        }
    }


    //-- Draw matches
    cv::Mat img_matches;
    drawMatches( left_photo_color, key_points_left_img,
                 right_photo_color, key_points_right_img,
                 good_matches, img_matches,
                 cv::Scalar::all(-1), cv::Scalar::all(-1),
                 std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //-- Show detected matches
    imshow("Good Matches", img_matches );
    cv::waitKey();


    return 0;
}