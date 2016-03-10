#pragma once

#include "pch.h"
#include "WallModel.h"
#include <string>

ref class RoomModel sealed {

public:
	RoomModel();
    virtual ~RoomModel();

    int areaCm2();
    int volumeCm3();

private:

	std::string* title;
	std::string* description;

	float longitude;
    float latitude;

	WallModel^ ceiling;
	WallModel^ floor;
	WallModel^ east;
	WallModel^ south;
	WallModel^ north;
	WallModel^ west;

};
