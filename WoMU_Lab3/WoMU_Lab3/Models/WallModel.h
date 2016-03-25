#pragma once

#include "pch.h"
#include <string>

ref class WallModel sealed {

public:
    WallModel();
    virtual ~WallModel();

	void title(Platform::String^ s) { title_ = s; }
	void description(Platform::String^ s) { description_ = s; }

    Platform::String^ title      () { return title_;       }
    Platform::String^ description() { return description_; }

    property Platform::String^ ImageFile;

private:

    Platform::String^ title_;
    Platform::String^ description_;

    void* photo_;

};
