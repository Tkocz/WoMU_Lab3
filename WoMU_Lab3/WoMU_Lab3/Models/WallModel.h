#pragma once

#include "pch.h"
#include <string>

ref class WallModel sealed {

public:
    WallModel();
    virtual ~WallModel();

    std::string& title      () { return title_;       }
    std::string& description() { return description_; }

    int heightCm() { return heightCm_; }
    int lengthCm() { return lengthCm_; }

    int areaCm2 () { return (lengthCm_ * heightCm_); }

private:

    std::string* title_;
    std::string* description_;

    int lengthCm_;
    int heightCm_;

    void* photo_;

};
