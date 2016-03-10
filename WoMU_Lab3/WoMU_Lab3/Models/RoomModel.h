#pragma once

#include "pch.h"
#include "WallModel.h"
#include <string>

ref class RoomModel sealed {

public:
    RoomModel();
    virtual ~RoomModel();

    Platform::String^ title      () { return title_;       }
    Platform::String^ description() { return description_; }

    float longitude() { return longitude_; }
    float latitude()  { return latitude_; }

    WallModel^ ceiling  () { return ceiling_; }
    WallModel^ floor    () { return floor_;   }
    WallModel^ eastWall () { return east_;    }
    WallModel^ southWall() { return south_;   }
    WallModel^ northWall() { return north_;   }
    WallModel^ westWall () { return west_;    }

    int areaCm2()   { return floor_->lengthCm() * floor_->heightCm(); }
    int volumeCm3() { return areaCm2() * east_->heightCm();          }

private:
    Platform::String^ title_;
    Platform::String^ description_;

    float longitude_;
    float latitude_;

    WallModel^ ceiling_;
    WallModel^ floor_;
    WallModel^ east_;
    WallModel^ south_;
    WallModel^ north_;
    WallModel^ west_;

};
