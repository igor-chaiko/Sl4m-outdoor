#ifndef SL4M_OUTDOOR_PROCESSVIDEO_H
#define SL4M_OUTDOOR_PROCESSVIDEO_H

#define NUMBER_OF_FRAMES_IN_POOL 15
#define CALIBRATION_CAMERA_MATRIX_PATH "../resources/igorCalibratedCameraHD.txt"
#define PATH_TO_VIDEOFILE "test6.MOV"

inline int numOfFirstUnusedFramePool;

void startProcessing();

#endif //SL4M_OUTDOOR_PROCESSVIDEO_H
