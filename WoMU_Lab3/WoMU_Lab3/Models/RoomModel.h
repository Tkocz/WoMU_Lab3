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
	void wall1(WallModel^ w) { wall1_ = w; }
	void wall2(WallModel^ w) { wall2_ = w; }
	void wall3(WallModel^ w) { wall3_ = w; }
	void wall4(WallModel^ w) { wall4_ = w; }

	WallModel^ ceiling() { return ceiling_; }
	WallModel^ floor() { return floor_; }
	WallModel^ wall1() { return wall1_; }
	WallModel^ wall2() { return wall2_; }
	WallModel^ wall3() { return wall3_; }
	WallModel^ wall4() { return wall4_; }


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
	WallModel^ wall1_;
	WallModel^ wall2_;
	WallModel^ wall3_;
	WallModel^ wall4_;

};