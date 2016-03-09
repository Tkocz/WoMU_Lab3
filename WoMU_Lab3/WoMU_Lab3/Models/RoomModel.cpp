#include "RoomModel.h"
#include "WallModel.h"

RoomModel::RoomModel() {
}

RoomModel::~RoomModel() = default;

int RoomModel::areaCm2() {
    return (floor.lengthCm * floor.heightCm);
}

int RoomModel::volumeCm3() {
    return (ceiling.widthCm * ceiling.heightCm * east.heightCm);
}
