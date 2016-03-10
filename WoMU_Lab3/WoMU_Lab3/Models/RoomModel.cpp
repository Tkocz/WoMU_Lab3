#include "pch.h"
#include "RoomModel.h"
#include "WallModel.h"

RoomModel::RoomModel() {
}

RoomModel::~RoomModel() {

}

int RoomModel::areaCm2() {
    return (floor->lengthCm() * floor->heightCm());
}

int RoomModel::volumeCm3() {
    return (areaCm2() * east->heightCm());
}
