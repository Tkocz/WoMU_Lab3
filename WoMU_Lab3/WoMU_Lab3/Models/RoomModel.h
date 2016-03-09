#pragma once
#include <string>
#include <Windows.h>

ref class RoomModel sealed
{
public:
	RoomModel();
private:

	std::string title;
	std::string description;
	int roomVolume;
	POINT coordinates;
	WallModel ceiling;
	WallModel floor;
	WallModel east;
	WallModel south;
	WallModel north;
	WallModel west;
};

