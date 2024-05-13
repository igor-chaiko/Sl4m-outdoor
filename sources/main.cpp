#include <opencv2/opencv.hpp>
#include "../headers/processVideo.h"

int main() {
    try {
        startProcessing();
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }







    return 0;
}
