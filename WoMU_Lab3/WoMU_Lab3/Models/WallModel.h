#pragma once

#using <mscorlib.dll>

public ref class WallModel {

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
