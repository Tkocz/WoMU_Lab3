#pragma once

#include "pch.h"
#include "WallModel.h"
#include <string>

ref class RoomModel sealed {

public:
	RoomModel();
	virtual ~RoomModel();

	void title(Platform::String^ s) { title_ = s; }
	void description(Platform::String^ s) { description_ = s; }
	Platform::String^ title() { return title_; }
	Platform::String^ description() { return description_; }

	void longitude(Platform::String^ s) { longitude_ = s; }
	void latitude(Platform::String^ s) { latitude_ = s; }

	Platform::String^ longitude() { return longitude_; }
	Platform::String^ latitude() { return latitude_; }

	void ceiling(WallModel^ w) { ceiling_ = w; }
	void floor(WallModel^ w) { floor_ = w; }
	void eastWall(WallModel^ w) { east_ = w; }
	void southWall(WallModel^ w) { south_ = w; }
	void northWall(WallModel^ w) { north_ = w; }
	void westWall(WallModel^ w) { west_ = w; }

	WallModel^ ceiling() { return ceiling_; }
	WallModel^ floor() { return floor_; }
	WallModel^ eastWall() { return east_; }
	WallModel^ southWall() { return south_; }
	WallModel^ northWall() { return north_; }
	WallModel^ westWall() { return west_; }


	void widthCm(int i) { widthCm_ = i; }
	void heightCm(int i) { heightCm_ = i; }
	void lengthCm(int i) { lengthCm_ = i; }

	int widthCm() { return widthCm_; }
	int heightCm() { return heightCm_; }
	int lengthCm() { return lengthCm_; }

	int areaCm2() { return (lengthCm_ * widthCm_); }
	int volumeCm3() { return areaCm2() * heightCm(); }

private:
	Platform::String^ title_;
	Platform::String^ description_;

	Platform::String^ longitude_;
	Platform::String^ latitude_;

	int widthCm_;
	int lengthCm_;
	int heightCm_;

	WallModel^ ceiling_;
	WallModel^ floor_;
	WallModel^ east_;
	WallModel^ south_;
	WallModel^ north_;
	WallModel^ west_;

};