//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "Views\AddRoomView.xaml.h"
#include "Views\RoomListView.xaml.h"

using namespace WoMU_Lab3;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Devices::Geolocation;
using namespace Windows::Devices::Geolocation::Geofencing;
using namespace Windows::ApplicationModel::Background;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Popups;

MainPage::MainPage()
{
	InitializeComponent();
	RegisterBackgroundTask();
	/*
	for each (room in room_collection)
	{
		GenerateGeofence(room);
	}*/
	
}


void WoMU_Lab3::MainPage::GoToRoomView_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    // generera filnamn och skicka med in till AddRoomView

    ((App^)Application::Current)->currentRoom = nullptr;
	Frame->Navigate(AddRoomView::typeid);
}


void WoMU_Lab3::MainPage::GoToListRoomView_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Frame->Navigate(RoomListView::typeid);
}

void WoMU_Lab3::MainPage::RequestLocationAccess()
{
	task<GeolocationAccessStatus> geolocationAccessRequestTask(Windows::Devices::Geolocation::Geolocator::RequestAccessAsync());
	geolocationAccessRequestTask.then([this](task<GeolocationAccessStatus> accessStatusTask)
	{
		auto accessStatus = accessStatusTask.get();

		if (accessStatus == GeolocationAccessStatus::Allowed)
		{
			return;
		}
		else {
			MessageDialog^ damn = ref new MessageDialog("Nej!");
			damn->ShowAsync();
		}
	});
}

void WoMU_Lab3::MainPage::RegisterBackgroundTask()
{
	try
	{
		task<BackgroundAccessStatus> requestAccessTask(BackgroundExecutionManager::RequestAccessAsync());
		requestAccessTask.then([this](BackgroundAccessStatus backgroundAccessStatus)
		{
			// Create a new background task builder
			BackgroundTaskBuilder^ geolocTaskBuilder = ref new BackgroundTaskBuilder();

			geolocTaskBuilder->Name = backgroundTask;
			geolocTaskBuilder->TaskEntryPoint = backgroundTaskEntryPoint;

			// Create a new timer triggering at a 15 minute interval
			auto trigger = ref new TimeTrigger(15, false);

			// Associate the timer trigger with the background task builder
			geolocTaskBuilder->SetTrigger(trigger);

			// Register the background task
			//geolocTask = geolocTaskBuilder->Register();

			RequestLocationAccess();

		});
	}
	catch (...)
	{
	}
}

Windows::Devices::Geolocation::Geofencing::Geofence^ MainPage::GenerateGeofence(RoomModel^ currentRoom)
{
	Geofence^ geofence = nullptr;

	try
	{
		String^ fenceKey = currentRoom->title();	//RoomID

		BasicGeoposition position;
		position.Latitude = std::stof(currentRoom->latitude()->Data());
		position.Longitude = std::stof(currentRoom->longitude()->Data());
		position.Altitude = 0.0;
		double radius = (currentRoom->lengthCm() / 100);

		// the geofence is a circular region
		Geocircle^ geocircle = ref new Geocircle(position, radius);

		bool singleUse = false;

		MonitoredGeofenceStates mask = static_cast<MonitoredGeofenceStates>(0);

		mask = mask | MonitoredGeofenceStates::Entered;
		mask = mask | MonitoredGeofenceStates::Removed;

		geofence = ref new Geofence(fenceKey, geocircle, mask, singleUse);
		
	}
	catch(...)
	{
	}

	return geofence;
}
