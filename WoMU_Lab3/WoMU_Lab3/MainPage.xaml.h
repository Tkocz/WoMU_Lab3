
#pragma once

#include "MainPage.g.h"
#include "Models\RoomModel.h"

namespace WoMU_Lab3
{
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void GoToRoomView_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void GoToListRoomView_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void RequestLocationAccess();
		void RegisterBackgroundTask();

		Platform::String^ backgroundTask;
		Platform::String^ backgroundTaskEntryPoint;
		Windows::ApplicationModel::Background::BackgroundTaskRegistration^ geolocTask;
		Platform::String^ backgroundTaskLocator;
		Platform::String^ backgroundTaskLocatorEntry;
		Windows::ApplicationModel::Background::BackgroundTaskRegistration^ geofenceTask;
		Windows::Devices::Geolocation::Geofencing::Geofence^ GenerateGeofence(RoomModel^ room);
		Windows::Foundation::Collections::IVector<Windows::Devices::Geolocation::Geofencing::Geofence^>^ geofences;
	};
}
