#pragma once

#include "Models\RoomModel.h"
#include "Views\RoomListView.g.h"
#include "MainPage.xaml.h"
using namespace Windows::Storage;
using namespace Platform;

using namespace WoMU_Lab3;


concurrency::task<std::vector<RoomModel^>> GetRooms();
concurrency::task<RoomModel^> LoadRoom(String^ filename);
