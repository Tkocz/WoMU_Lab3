#pragma once

#include "pch.h"
#include <string>

ref class WallModel sealed {

public:
    WallModel();
    virtual ~WallModel();

    Platform::String^ title      () { return title_;       }
    Platform::String^ description() { return description_; }

    int heightCm() { return heightCm_; }
    int lengthCm() { return lengthCm_; }

    int areaCm2 () { return (lengthCm_ * heightCm_); }

private:

    Platform::String^ title_;
    Platform::String^ description_;

    int lengthCm_;
    int heightCm_;

    void* photo_;

};
