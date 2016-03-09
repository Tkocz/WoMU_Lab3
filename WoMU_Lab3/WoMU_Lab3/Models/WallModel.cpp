#include "WallModel.h"

WallModel::WallModel() {
}

WallModel::~WallModel() = default;

int WallModel::volume() {
    return (ceiling.widthCm * ceiling.heightCm * east.heightCm);
}
