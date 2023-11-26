
#include <opencv2/opencv.hpp>



// void triangulatePointsAndShowResult(const cv::Mat &P1, const cv::Mat &P2, const vector<Point2d> &points1,
//                                     const vector<Point2d> &points2) {
//     cv::Mat points4D;
//     triangulatePoints(P1, P2, points1, points2, points4D);
//
//     for (int i = 0; i < points4D.cols; i++) {
//         cv::Mat point = points4D.col(i);
//         double x = point.at<double>(0) / point.at<double>(3);
//         double y = point.at<double>(1) / point.at<double>(3);
//         double z = point.at<double>(2) / point.at<double>(3);
//         cout << x << ", " << y << ", " << z << endl;
//     }
//     cout << endl;
// }

void triangulation() {
    cv::Mat firstFrame = cv::imread("resources/4.jpg");
    cv::Mat secondFrame = cv::imread("resources/3.jpg");

    cv::Mat K = (cv::Mat_<double>(3, 3) << 534.546, 0, 320,
            0, 534.546, 240,
            0, 0, 1);
    cv::Mat D = (cv::Mat_<double>(5, 1) << 0.01, -0.001, 0.002, 0.003, 0.001);

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
    std::vector<cv::DMatch> good_Matches;
    bfMatcher.knnMatch(descriptors1, descriptors2, matches, 2);

    std::vector<cv::Point2d> matched_points2;
    std::vector<cv::Point2d> matched_points1;
    double ratio_threshold = 0.8;

    for (size_t i = 0; i < matches.size(); i++) {
        if (matches[i][0].distance < ratio_threshold * matches[i][1].distance) {
            matched_points1.push_back(keypoints1[matches[i][0].queryIdx].pt);
            matched_points2.push_back(keypoints2[matches[i][0].trainIdx].pt);
            good_Matches.push_back(matches[i][0]);
        }
    }

    cv::Mat mask;
    cv::Mat E = cv::findEssentialMat(matched_points1, matched_points2, K.at<double>(0, 0), cv::Point2d(K.at<double>(0, 2), K.at<double>(1, 2)), cv::RANSAC, 0.999, 1.0, mask);

    cv::Mat R, t;
    cv::recoverPose(E, matched_points1, matched_points2, K, R, t, mask);

    cv::Mat P1 = cv::Mat::eye(3, 4, CV_64F);
    cv::Mat P2 = cv::Mat::eye(3, 4, CV_64F);
    R.copyTo(P2(cv::Rect(0, 0, 3, 3)));
    t.copyTo(P2(cv::Rect(3, 0, 1, 3)));

    cv::Mat points4D;
    cv::triangulatePoints(P1, P2, matched_points1, matched_points2, points4D);

    cv::Mat points3D = (cv::Mat_<double>(4, points4D.cols));
    for (int i = 0; i < points4D.cols; i++) {
        cv::Mat point = points4D.col(i);
        for (int j = 0; j < 3; j++) {
            points3D.at<double>(j, i) = point.at<double>(j) / point.at<double>(3);
        }
    }

    std::cout << points3D << std::endl;
}