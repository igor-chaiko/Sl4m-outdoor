#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(0);

    cv::Mat frame;
    cv::Mat grayFrame;
    std::vector<cv::Point2f> corners;

    while (true) {
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        cv::goodFeaturesToTrack(grayFrame, corners, 100, 0.05, 15);

        for (const cv::Point2f &corner: corners) {
            cv::circle(frame, corner, 5, cv::Scalar(0, 0, 255), -1);
        }

        cv::imshow("Corner Detection", frame);

        cv::waitKey(1);
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}