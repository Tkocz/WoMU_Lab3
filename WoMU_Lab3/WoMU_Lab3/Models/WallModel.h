#pragma once

#include <string>

class WallModel {

public:
	WallModel();
    ~WallModel();

    int areaCm2();

	std::string title;
	std::string description;

	int lengthCm;
	int heightCm;

    void* photo;

private:

};
