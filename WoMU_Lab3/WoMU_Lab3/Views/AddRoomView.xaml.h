//
// AddRoomView.xaml.h
// Declaration of the AddRoomView class
//

#pragma once

#include "Views\AddRoomView.g.h"
#include "MainPage.xaml.h"

#include "Models\RoomModel.h"
#include <string.h>

using namespace Platform;
using namespace Windows::Storage;
using namespace WoMU_Lab3;

namespace WoMU_Lab3
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AddRoomView sealed
	{
	public:
		AddRoomView();
	private:
        String^ GetNextRoomFilename();
		void WriteRoomToStorage();
		void ReadRoomFromStorage(bool loadFromFile);
		//RoomModel^ currentRoom = ref new RoomModel;
		WallModel^ currentWall1 = ref new WallModel();
		WallModel^ currentWall2 = ref new WallModel();
		WallModel^ currentWall3 = ref new WallModel();
		WallModel^ currentWall4 = ref new WallModel();
		WallModel^ currentCeiling = ref new WallModel();
		WallModel^ currentFloor = ref new WallModel();
		StorageFolder^ localFolder = ApplicationData::Current->LocalFolder;
		void Wall1ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void Wall2ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void Wall3ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void FloorButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void CeilingButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void Wall4ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void LenghtSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void WidthSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void HeightSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void GoToPreviousPage_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		concurrency::cancellation_token_source geopositionTaskTokenSource;
		void GetGeolocationButtonClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void UpdateLocationData(Windows::Devices::Geolocation::Geoposition^ position);
		
		unsigned int desiredAccuracyInMetersValue = 0;
	
		internal:
			static AddRoomView^ rootPage;
	};
}
