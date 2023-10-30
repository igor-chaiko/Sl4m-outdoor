#include <opencv2/opencv.hpp>


void triangulation() {
    cv::Mat firstFrame = cv::imread("resources/1.jpg");
    cv::Mat secondFrame = cv::imread("resources/2.jpg");

    cv::Mat K1 = (cv::Mat_<double>(3, 4) << 534.546, 0, 320, 0,
            0, 534.546, 240, 0,
            0, 0, 1, 0);
    cv::Mat D1 = (cv::Mat_<double>(5, 1) << 0.01, -0.001, 0.002, 0.003, 0.001);

    cv::Mat K2 = (cv::Mat_<double>(3, 4) << 534.546, 0, 320, 0,
            0, 534.546, 240, 0,
            0, 0, 1, 0);
    cv::Mat D2 = (cv::Mat_<double>(5, 1) << 0.01, -0.001, 0.002, 0.003, 0.001);

    cv::Mat firstGray, secondGray;
    cv::cvtColor(firstFrame, firstGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(secondFrame, secondGray, cv::COLOR_BGR2GRAY);

    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    orb->detectAndCompute(firstGray, cv::noArray(), keypoints1, descriptors1);
    orb->detectAndCompute(secondGray, cv::noArray(), keypoints2, descriptors2);

    cv::BFMatcher bfMatcher(cv::NORM_HAMMING);

    std::vector<std::vector<cv::DMatch>> matches;
    bfMatcher.knnMatch(descriptors1, descriptors2, matches, 2);

    std::vector<cv::Point2f> matched_points1, matched_points2;
    double ratio_threshold = 0.8;

    for (size_t i = 0; i < matches.size(); i++) {
        if (matches[i][0].distance < ratio_threshold * matches[i][1].distance) {
            matched_points1.push_back(keypoints1[matches[i][0].queryIdx].pt);
            matched_points2.push_back(keypoints2[matches[i][0].trainIdx].pt);
        }
    }
    cv::Mat matched_points1_mat(2, matched_points1.size(), CV_32F);
    cv::Mat matched_points2_mat(2, matched_points1.size(), CV_32F);

    for (size_t i = 0; i < matched_points1.size(); i++) {
        matched_points1_mat.at<float>(0, i) = matched_points1[i].x;
        matched_points1_mat.at<float>(1, i) = matched_points1[i].y;

        matched_points2_mat.at<float>(0, i) = matched_points2[i].x;
        matched_points2_mat.at<float>(1, i) = matched_points2[i].y;
    }

    cv::Mat img_matches;
    cv::drawMatches(firstFrame, keypoints1, secondFrame, keypoints2, matches, img_matches);
    cv::resizeWindow("Matches", 800, 600);


    cv::imshow("Matches", img_matches);
    cv::waitKey(10000);

    std::cerr << matched_points1_mat << std::endl;
    std::cerr << matched_points2_mat << std::endl;

    cv::Mat points4D;
    cv::triangulatePoints(K1, K2, matched_points1_mat, matched_points2_mat, points4D);

    std::cout << points4D << std::endl;

    for (int i = 0; i < points4D.cols; i++) {
        points4D.col(i) /= points4D.at<float>(3, i);
    }

    for (int i = 0; i < points4D.cols; i++) {
        float x = points4D.at<float>(0, i);
        float y = points4D.at<float>(1, i);
        float z = points4D.at<float>(2, i);
        std::cout << "Point " << i + 1 << ": (x=" << x << ", y=" << y << ", z=" << z << ")" << std::endl;
    }
}