#pragma once

#include "WallModel.h"

#using <mscorlib.dll>

__gc class RoomModel {

public:
	RoomModel();
    ~RoomModel();

    int areaCm2();
    int volumeCm3();

	String^ title;
	String^ description;

	float longitude;
    float latitude,

	WallModel^ ceiling;
	WallModel^ floor;
	WallModel^ east;
	WallModel^ south;
	WallModel^ north;
	WallModel^ west;

private:

};

