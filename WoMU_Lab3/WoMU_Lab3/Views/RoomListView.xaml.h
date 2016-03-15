//
// RoomListView.xaml.h
// Declaration of the RoomListView class
//

#pragma once

#include "Views\RoomListView.g.h"
using namespace Windows::Storage;

namespace WoMU_Lab3
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class RoomListView sealed
	{
	public:
		RoomListView();
	private:
		StorageFolder^ localFolder = ApplicationData::Current->LocalFolder;
		void GoToPreviousPage_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
