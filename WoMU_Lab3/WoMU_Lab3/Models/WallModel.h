#pragma once

#using <mscorlib.dll>

class WallModel {

public:
	WallModel();
    ~WallModel();

    int areaCm2();

	System::String^ title;
	System::String^ description;

	int lengthCm;
	int heightCm;

    void* photo;

private:

};
