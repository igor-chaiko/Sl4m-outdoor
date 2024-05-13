//
// Created by Ruslan Chudinov on 13.05.2024.
//

#ifndef SL4M_OUTDOOR_TRAFFICSIGN_H
#define SL4M_OUTDOOR_TRAFFICSIGN_H

class trafficSign {
    bool isLeft;
    bool isRight;
    int id;

public:
    trafficSign();
    trafficSign(bool isLeft, bool isRight, int id);
};


#endif //SL4M_OUTDOOR_TRAFFICSIGN_H
