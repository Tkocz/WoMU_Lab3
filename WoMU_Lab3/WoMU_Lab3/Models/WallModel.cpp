#include "pch.h"
#include "WallModel.h"

WallModel::WallModel() {
}

WallModel::~WallModel() {

}


int WallModel::lengthCm() {
	return lengthCm_;
}

int WallModel::heightCm() {
	return heightCm_;
}


int WallModel::areaCm2() {
    return (lengthCm_ * heightCm_);
}
