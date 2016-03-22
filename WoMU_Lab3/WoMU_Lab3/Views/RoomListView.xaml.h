//
// RoomListView.xaml.h
// Declaration of the RoomListView class
//

#pragma once

#include "Models\RoomModel.h"
#include "Views\RoomListView.g.h"
#include "MainPage.xaml.h"
using namespace Windows::Storage;
using namespace Platform;

using namespace WoMU_Lab3;

namespace WoMU_Lab3
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class RoomListView sealed
	{
	public:
		RoomListView();
	private:
		StorageFolder^ localFolder = ApplicationData::Current->LocalFolder;
        concurrency::task<RoomModel^> LoadRoom(String^ filename);
        void GoToPreviousPage_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void OnRoomClick(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e);
    };
}
