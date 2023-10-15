#include <opencv2/opencv.hpp>
#include "detectAndComputeGFTT.h"


using namespace cv;
using namespace std;

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Ошибка при открытии видео!" << endl;
        return -1;
    }

    Mat frame1, frame2;
    Mat grayFrame1, grayFrame2;
    vector<cv::KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    Mat img;
    cap >> frame1;
    cvtColor(frame1, grayFrame1, COLOR_BGR2GRAY);

    detectAndComputeGFTT(grayFrame1, keypoints1, descriptors1);

    while (true) {
        cap >> frame2;

        if (frame2.empty()) {
            break;
        }

        cvtColor(frame2, grayFrame2, COLOR_BGR2GRAY);

        detectAndComputeGFTT(grayFrame2, keypoints2, descriptors2);

        BFMatcher matcher(NORM_HAMMING);
        vector<DMatch> matches;

        matcher.match(descriptors1, descriptors2, matches);

        drawMatches(frame1, keypoints1, frame2, keypoints2, matches, img);

        imshow("Video with Features", img);

        if (waitKey(1) == 27) {
            break;
        }
        descriptors1 = descriptors2;
        frame1 = frame2;
        keypoints1 = keypoints2;

    }

    return 0;
}