#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void triangulatePointsAndShowResult(const Mat &P1, const Mat &P2, const vector<Point2d> &points1,
                                    const vector<Point2d> &points2) {
    Mat points4D;
    triangulatePoints(P1, P2, points1, points2, points4D);

    for (int i = 0; i < points4D.cols; i++) {
        Mat point = points4D.col(i);
        double x = point.at<double>(0) / point.at<double>(3);
        double y = point.at<double>(1) / point.at<double>(3);
        double z = point.at<double>(2) / point.at<double>(3);
        cout << x << ", " << y << ", " << z << endl;
    }
    cout << endl;
}


void triangulation() {
    Mat firstFrame = imread("resources/1.jpg");
    Mat secondFrame = imread("resources/2.jpg");

    Mat K = (Mat_<double>(3, 3) << 534.546, 0, 320,
            0, 534.546, 240,
            0, 0, 1);
    Mat D = (Mat_<double>(5, 1) << 0.01, -0.001, 0.002, 0.003, 0.001);

    Mat firstGray, secondGray;
    cvtColor(firstFrame, firstGray, COLOR_BGR2GRAY);
    cvtColor(secondFrame, secondGray, COLOR_BGR2GRAY);

    Ptr<ORB> orb = ORB::create();
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    orb->detectAndCompute(firstGray, noArray(), keypoints1, descriptors1);
    orb->detectAndCompute(secondGray, noArray(), keypoints2, descriptors2);

    BFMatcher bfMatcher(NORM_HAMMING);

    vector<vector<DMatch>> matches;
    vector<DMatch> good_matches;
    bfMatcher.knnMatch(descriptors1, descriptors2, matches, 2);

    vector<Point2d> matched_points2;
    vector<Point2d> matched_points1;
    double ratio_threshold = 0.8;

    for (size_t i = 0; i < matches.size(); i++) {
        if (matches[i][0].distance < ratio_threshold * matches[i][1].distance) {
            matched_points1.push_back(keypoints1[matches[i][0].queryIdx].pt);
            matched_points2.push_back(keypoints2[matches[i][0].trainIdx].pt);
            good_matches.push_back(matches[i][0]);
        }
    }

    cv::Mat img_matches;
    cv::drawMatches(firstFrame, keypoints1, secondFrame, keypoints2, good_matches, img_matches);
    cv::imshow("Good Matches", img_matches);
    cv::waitKey(0);

    Mat mask;
    Mat E = findEssentialMat(matched_points1, matched_points2);
    //cout << E << endl;
    //cout << endl;

    Mat R, t, R1, R2, t_, _t_;
    recoverPose(E, matched_points1, matched_points2, R, t);
    //cout << R << endl;
    //cout << t << endl << endl;

    decomposeEssentialMat(E, R1, R2, t_);
    _t_ = -t;

    Mat P1 = (Mat_<double>(3, 4) << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0);
    Mat P2 = Mat::eye(3, 4, CV_64F);
    R.copyTo(P2(Rect(0, 0, 3, 3)));
    t.copyTo(P2(Rect(3, 0, 1, 3)));

    triangulatePointsAndShowResult(P1, P2, matched_points1, matched_points2);

    R1.copyTo(P2(Rect(0, 0, 3, 3)));
    t_.copyTo(P2(Rect(3, 0, 1, 3)));

    triangulatePointsAndShowResult(P1, P2, matched_points1, matched_points2);

    R2.copyTo(P2(Rect(0, 0, 3, 3)));
    t_.copyTo(P2(Rect(3, 0, 1, 3)));

    triangulatePointsAndShowResult(P1, P2, matched_points1, matched_points2);

    R1.copyTo(P2(Rect(0, 0, 3, 3)));
    _t_.copyTo(P2(Rect(3, 0, 1, 3)));

    triangulatePointsAndShowResult(P1, P2, matched_points1, matched_points2);

    R2.copyTo(P2(Rect(0, 0, 3, 3)));
    _t_.copyTo(P2(Rect(3, 0, 1, 3)));

    triangulatePointsAndShowResult(P1, P2, matched_points1, matched_points2);

}