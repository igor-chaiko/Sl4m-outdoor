#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


const double quality = 0.05;
const double minDistance = 10;

int buildingFeaturesInVideoUsingGFTT(string path) {
    VideoCapture cap(path);
    if (!cap.isOpened()) {
        cerr << "Ошибка при открытии видео!" << endl;
        return -1;
    }

    Mat frame;
    Mat grayFrame;

//  Point2f - структура, которая описывает точки на кадре в формате (х, у)
    vector<Point2f> corners;

    while (true) {
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

        goodFeaturesToTrack(grayFrame, corners, 100, 0.05, 15);

        for (const Point2f &corner: corners) {
            circle(frame, corner, 5, Scalar(0, 0, 255), -1);
        }

        imshow("Video with Features", frame);

        if (waitKey(1) == 27) {
            break;
        }
    }

    return 0;
}

int featuresByOrb() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Ошибка при открытии видео!" << endl;
        return -1;
    }

    Mat frame;
    Mat grayFrame;
    vector<KeyPoint> keypoints;
    Mat descriptors;
    Ptr<ORB> orb = ORB::create(100);

    while (true) {
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

        orb->detectAndCompute(grayFrame, noArray(), keypoints, descriptors);

        drawKeypoints(frame, keypoints, frame, Scalar(0, 0, 255));

        imshow("Video with Features", frame);

        if (waitKey(1) == 27) {
            break;
        }
    }
    return 0;
}

int unionOfTwoWays(string path) {
    VideoCapture cap(path);
    if (!cap.isOpened()) {
        cerr << "Ошибка при открытии видео!" << endl;
        return -1;
    }

    Mat frame1, frame2;
    Mat grayFrame1, grayFrame2;
    vector<cv::KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    Ptr<ORB> orb = ORB::create(100);
    vector<Point2f> corners1, corners2;
    Mat img;
    cap >> frame1;
    cvtColor(frame1, grayFrame1, COLOR_BGR2GRAY);

    goodFeaturesToTrack(grayFrame1, corners1, 200, 0.05, 15);

    for (const Point2f &corner: corners1) {
        keypoints1.emplace_back(corner, 1.0);
    }

    orb->compute(grayFrame1, keypoints1, descriptors1);

    while (true) {
        keypoints2.clear();
        cap >> frame2;

        if (frame2.empty()) {
            break;
        }

        cvtColor(frame2, grayFrame2, COLOR_BGR2GRAY);

        goodFeaturesToTrack(grayFrame2, corners2, 200, quality, minDistance);

        for (const Point2f &corner: corners2) {
            keypoints2.emplace_back(corner, 1.0);
        }

        orb->compute(grayFrame2, keypoints2, descriptors2);

        BFMatcher matcher1(NORM_HAMMING);

        vector<DMatch> matches;

        matcher1.match(descriptors1, descriptors2, matches);

        drawMatches(frame1, keypoints1, frame2, keypoints2, matches, img);

        imshow("Video with Features", img);

        if (waitKey(1) == 27) {
            break;
        }
        descriptors1 = descriptors2;
        frame1 = frame2;
        keypoints1 = keypoints2;
        grayFrame1 = grayFrame2;
    }

    return 0;
}


void detectAndComputeGFTT(Mat grayImage,
                          CV_OUT CV_IN_OUT vector<KeyPoint> &keypoints,
                          Mat descriptors) {
    keypoints.clear();
    descriptors.release();

    vector<KeyPoint> orbKeypoints;
    Mat orbDescriptors;
    Ptr<ORB> orb = ORB::create(200);
    vector<Point2f> features;

    goodFeaturesToTrack(grayImage, features, 200, quality, minDistance);

    orb->detectAndCompute(grayImage, noArray(), orbKeypoints, orbDescriptors);

    for (size_t i = 0; i < orbKeypoints.size(); i++) {
        for (auto feature : features) {
            if (norm(orbKeypoints[i].pt - feature) < 1) {
                keypoints.push_back(orbKeypoints[i]);
                descriptors.push_back(orbDescriptors.row(i));
            }
        }
    }
}