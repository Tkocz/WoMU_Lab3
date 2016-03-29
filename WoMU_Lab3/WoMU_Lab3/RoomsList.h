#pragma once

#include "Models\RoomModel.h"
#include "Views\RoomListView.g.h"
#include "MainPage.xaml.h"
using namespace Windows::Storage;
using namespace Platform;

using namespace WoMU_Lab3;

ref class RoomsList sealed
{
public:
	RoomsList();
private:
	StorageFolder^ localFolder = ApplicationData::Current->LocalFolder;
	concurrency::task<RoomModel^> LoadRoom(String^ filename);
protected:
	RoomModel^ rooms[99];
};

