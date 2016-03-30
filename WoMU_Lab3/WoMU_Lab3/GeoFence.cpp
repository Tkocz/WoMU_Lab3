#include "pch.h"
#include "GeoFence.h"
#include "Models\RoomModel.h"
#include "Models\WallModel.h"
#include "RoomsList.h"

using namespace WoMU_Lab3;
using namespace Platform;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Devices::Geolocation;
using namespace Windows::Devices::Geolocation::Geofencing;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Popups;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Concurrency;
using namespace Windows::ApplicationModel::Background;
using namespace std;


GeoFence::GeoFence():
	backgroundTaskName("SampleGeofenceBackgroundTask"),
	backgroundTaskEntryPoint("BackgroundTask.GeofenceBackgroundTask")
{
	geofences = GeofenceMonitor::Current->Geofences;

	RoomModel^* rooms = new RoomModel^[100];
	for (int i = 0; i < 100; i++) {
		rooms[i] = nullptr;
	}

	GetRooms(rooms).then([this, rooms]() {
		GenerateAllGeoFences(rooms);
	});
}


GeoFence::~GeoFence()
{
}

Windows::Devices::Geolocation::Geofencing::Geofence ^ GeoFence::GenerateGeoFence(RoomModel^ room)
{
	Geofence^ geofence = nullptr;
	try {
		BasicGeoposition position;
		position.Latitude = std::stof(room->latitude()->Data());
		position.Longitude = std::stof(room->longitude()->Data());
		position.Altitude = 0.0;;
		double radius;

		radius = (sqrt(room->lengthCm() * room->lengthCm()) + (room->heightCm() * room->heightCm()))*10;

		String^ fencekey = room->title();;

		MonitoredGeofenceStates mask = static_cast<MonitoredGeofenceStates>(0);
		mask = mask | MonitoredGeofenceStates::Entered;
		mask = mask | MonitoredGeofenceStates::Exited;


		Geocircle^ geocircle = ref new Geocircle(position, radius);
		TimeSpan dwelltime;
		dwelltime.Duration = 100000U;
		bool singleUse = false;

		geofence = ref new Geofence(fencekey, geocircle, mask, singleUse, dwelltime);
	}
	catch (...) {

	}
	return geofence;
}


void GeoFence::GenerateAllGeoFences(RoomModel^ rooms[])
{
	geofences->Clear();

	concurrency::create_task([rooms, this]() {
		std::this_thread::sleep_for(1s);
		for (int i = 0; i < 100; i++) {
			if (rooms[i] == nullptr)
				continue;

			OutputDebugString(L"HEJ\n");
			auto room = rooms[i];

			try {
				if (room->title() != "") {
					Geofence^ geofence = GenerateGeoFence(room);
					char msgbuf[100] = {0};
					sprintf(msgbuf, "%ls\n", room->latitude()->Data());
					OutputDebugStringA(msgbuf);
					geofences->InsertAt(0, geofence);
				}
			}
			catch (...)
			{
				continue;
			}
		}
	});
}

void GeoFence::RegisterBackgroundTask()
{
	// Look for an already registered background task
	geofenceTask = nullptr;

	MessageDialog^ damn;

	auto iter = BackgroundTaskRegistration::AllTasks->First();
	while (iter->HasCurrent)
	{
		auto task = iter->Current;
		if (task->Value->Name == backgroundTaskName)
		{
			geofenceTask = safe_cast<BackgroundTaskRegistration^>(task->Value);
			break;
		}
		iter->MoveNext();
        }

	if (geofenceTask != nullptr){

        // Register for background task completion notifications
        taskCompletedToken = geofenceTask->Completed::add(ref new BackgroundTaskCompletedEventHandler(this, &GeoFence::OnCompleted));

        try
        {
            // Check the background access status of the application and display the appropriate status message
            switch (BackgroundExecutionManager::GetAccessStatus())
            {
            case BackgroundAccessStatus::Unspecified:
            case BackgroundAccessStatus::Denied:
                damn = ref new MessageDialog("Not able to run in background.");
                damn->ShowAsync();
                break;

            default:
                break;
            }
        }
        catch (Exception^ ex)
        {
            damn = ref new MessageDialog(ex->ToString());
            damn->ShowAsync();
        }
    }
    else
    {
        MessageDialog^ damn2;
        try
        {
            // Get permission for a background task from the user. If the user has already answered once,
            // this does nothing and the user must manually update their preference via PC Settings.
            task<BackgroundAccessStatus> requestAccessTask(BackgroundExecutionManager::RequestAccessAsync());
            requestAccessTask.then([this](BackgroundAccessStatus backgroundAccessStatus)
            {
				MessageDialog^ damn;
                // Regardless of the answer, register the background task. If the user later adds this application
                // to the lock screen, the background task will be ready to run.

                // Create a new background task builder
                BackgroundTaskBuilder^ geofenceTaskBuilder = ref new BackgroundTaskBuilder();

                geofenceTaskBuilder->Name = "GeoBackgroundTask";
                geofenceTaskBuilder->TaskEntryPoint = "BackgroundTask.GeofenceBackgroundTask";

                // Create a new location trigger
                auto trigger = ref new LocationTrigger(LocationTriggerType::Geofence);

                // Associate the location trigger with the background task builder
                geofenceTaskBuilder->SetTrigger(trigger);

                // Register the background task
                geofenceTask = geofenceTaskBuilder->Register();

                // Register for background task completion notifications
                taskCompletedToken = geofenceTask->Completed::add(ref new BackgroundTaskCompletedEventHandler(this, &GeoFence::OnCompleted));

				// Check the background access status of the application and display the appropriate status message
				switch (backgroundAccessStatus)
				{
				case BackgroundAccessStatus::Unspecified:
				case BackgroundAccessStatus::Denied:
					damn = ref new MessageDialog("Not able to run in background.");
					damn->ShowAsync();
					break;

				default:
					damn = ref new MessageDialog("bgtask registered.");
					damn->ShowAsync();

					RequestLocationAccess();
					break;
				}
            });
        }
        catch (Exception^ ex)
        {
            damn = ref new MessageDialog(ex->ToString());
            damn->ShowAsync();
        }
	}
}

void GeoFence::RequestLocationAccess()
{
	task<GeolocationAccessStatus> geolocationAccessRequestTask(Windows::Devices::Geolocation::Geolocator::RequestAccessAsync());
	geolocationAccessRequestTask.then([this](task<GeolocationAccessStatus> accessStatusTask)
	{
		auto accessStatus = accessStatusTask.get();

		switch (accessStatus)
		{
		case GeolocationAccessStatus::Allowed:
			break;
		case GeolocationAccessStatus::Denied:
		{MessageDialog^ damn = ref new MessageDialog("Denied!");
		damn->ShowAsync();
		break; }
		case GeolocationAccessStatus::Unspecified:
		{MessageDialog^ damn2 = ref new MessageDialog("Unspecified error!");
		damn2->ShowAsync(); }
			break;
		}
	});
}

void GeoFence::OnCompleted(BackgroundTaskRegistration^ task, Windows::ApplicationModel::Background::BackgroundTaskCompletedEventArgs^ args)
{
	// Update the UI with progress reported by the background task
	// We need to dispatch to the UI thread to display the output
	Windows::UI::Core::CoreDispatcher^ Dispatcher;
	Dispatcher->RunAsync(
		CoreDispatcherPriority::Normal,
		ref new DispatchedHandler(
			[this, args]()
	{
		try
		{
			// Throw an exception if the background task had an unrecoverable error
			args->CheckResult();

			// Update the UI with the completion status of the background task
			auto settings = ApplicationData::Current->LocalSettings->Values;
			if (settings->HasKey("Status"))
			{
				MessageDialog^ damn9 = ref new MessageDialog("Status");
				damn9->ShowAsync();
			}

			// do app work here

		}
		catch (Exception^ ex)
		{
			// The background task had an error
			MessageDialog^ damn10 = ref new MessageDialog(ex->Message);
			damn10->ShowAsync();
		}
	},
			CallbackContext::Any
		)
		);
}