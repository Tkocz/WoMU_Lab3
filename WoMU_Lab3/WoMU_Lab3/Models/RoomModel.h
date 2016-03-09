#pragma once

#include "WallModel.h"

#include <string>

class RoomModel {

public:
	RoomModel();
    ~RoomModel();

    volume();

	std::string title;
	std::string description;

	float longitude;
    float latitude,

	WallModel ceiling;
	WallModel floor;
	WallModel east;
	WallModel south;
	WallModel north;
	WallModel west;

private:

};

