#pragma once

#include "MainPage.xaml.h"
#include "Models\RoomModel.h"
#include "pch.h"

using namespace Windows::Storage;

ref class GeoFence sealed
{
public:
	GeoFence();
	virtual ~GeoFence();

	void RegisterBackgroundTask();
	void addGeoFence(RoomModel^ room);

private:
	StorageFolder^ localFolder = ApplicationData::Current->LocalFolder;
	Windows::Devices::Geolocation::Geofencing::Geofence^ GenerateGeoFence(RoomModel^ room);
	void RequestLocationAccess();
	void GenerateAllGeoFences(RoomModel^ rooms[]);
	void OnCompleted(Windows::ApplicationModel::Background::BackgroundTaskRegistration^ sender, Windows::ApplicationModel::Background::BackgroundTaskCompletedEventArgs^ e);

private:
	Platform::Collections::Vector<Platform::String^>^ geofenceBackgroundEvents;
	Platform::String^ backgroundTaskName;
	Platform::String^ backgroundTaskEntryPoint;

	Windows::Foundation::Collections::IVector<Windows::Devices::Geolocation::Geofencing::Geofence^>^ geofences;
	void RegisterBackgroundTaskOrg(Platform::Object^ sender);
	Windows::ApplicationModel::Background::BackgroundTaskRegistration^ geofenceTask;
	Windows::Foundation::EventRegistrationToken taskCompletedToken;

};