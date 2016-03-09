#include "WallModel.h"

WallModel::WallModel() {
}

WallModel::~WallModel() = default;

int WallModel::areaCm2() {
    return (lengthCm * heightCm);
}
