#pragma once

#include "pch.h"
#include "WallModel.h"
#include <string>

ref class RoomModel sealed {

public:
    RoomModel();
    virtual ~RoomModel();

    std::string& title      () { return title_;       }
    std::string& description() { return description_; }

    float longitude() { return longitude_; }
    float latitude()  { return latitude; }

    WallModel& ceiling  () { return ceiling_; }
    WallModel& floor    () { return floor_;   }
    WallModel& eastWall () { return east_;    }
    WallModel& southWall() { return south_;   }
    WallModel& northWall() { return north_;   }
    WallModel& westWall () { return west_;    }

    int areaCm2()   { return floor->lengthCm() * floor->heightCm(); }
    int volumeCm3() { return areaCm2() * east->heightCm();          }

private:
    std::unique_ptr<std::string> title_;
    std::unique_ptr<std::string> description_;

    float longitude_;
    float latitude_;

    WallModel^ ceiling_;
    WallModel^ floor_;
    WallModel^ east_;
    WallModel^ south_;
    WallModel^ north_;
    WallModel^ west_;

};
