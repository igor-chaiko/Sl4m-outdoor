//
// Created by Ruslan Chudinov on 13.05.2024.
//
#include "../headers/trafficSign.h"

trafficSign::trafficSign() {

}

trafficSign::trafficSign(bool isLeft, bool isRight, int id) {
    this->isLeft = isLeft;
    this->isRight = isRight;
    this->id = id;
}

