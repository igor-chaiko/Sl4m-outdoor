#ifndef SL4M_OUTDOOR_PROCESSVIDEO_H
#define SL4M_OUTDOOR_PROCESSVIDEO_H

#define NUMBER_OF_FRAMES_IN_POOL 15
#define CALIBRATION_CAMERA_MATRIX_PATH "../resources/CalibratedCamera.txt"
#define PATH_TO_VIDEOFILE "test7.MOV"

inline int numOfFirstUnusedFramePool;

void startProcessing();

#endif //SL4M_OUTDOOR_PROCESSVIDEO_H
