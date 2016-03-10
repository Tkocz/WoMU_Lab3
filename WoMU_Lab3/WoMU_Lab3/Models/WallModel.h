#pragma once

#include "pch.h"
#include <string>

ref class WallModel sealed {

public:
	WallModel();
    virtual ~WallModel();

	int lengthCm();
	int heightCm();

    int areaCm2();

private:

	std::string* title_;
	std::string* description_;

	int lengthCm_;
	int heightCm_;

    void* photo_;

};
