#pragma once

#include "pch.h"
#include <string>

ref class WallModel sealed {

public:
    WallModel();
    virtual ~WallModel();

    Platform::String^ title      () { return title_;       }
    Platform::String^ description() { return description_; }


private:

    Platform::String^ title_;
    Platform::String^ description_;

    void* photo_;

};
