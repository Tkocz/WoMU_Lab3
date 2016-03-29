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


GeoFence::GeoFence()
{
	geofences = GeofenceMonitor::Current->Geofences;
}


GeoFence::~GeoFence()
{
}

Windows::Devices::Geolocation::Geofencing::Geofence ^ GeoFence::GenerateGeoFence(RoomModel^ room)
{
	Geofence^ geofence;
	BasicGeoposition position;
	position.Latitude = std::stof(room->latitude()->Data());
	position.Longitude = std::stof(room->longitude()->Data());
	position.Altitude = 0.0;;
	double radius;

	radius = (sqrt(room->lengthCm() * room->lengthCm()) + (room->heightCm() * room->heightCm()));

	String^ fencekey = room->title();;

	MonitoredGeofenceStates mask = static_cast<MonitoredGeofenceStates>(0);
	mask = mask | MonitoredGeofenceStates::Entered;
	mask = mask | MonitoredGeofenceStates::Exited;


	Geocircle^ geocircle = ref new Geocircle(position, radius);
	TimeSpan dwelltime;
	dwelltime.Duration = 10000000;
	bool singleUse = false;

	geofence = ref new Geofence(fencekey, geocircle, mask, singleUse, dwelltime);

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

					geofences->InsertAt(0, geofence);
				}
			}
			catch (...)
			{
			}
		}
	});
	}

void GeoFence::RegisterBackgroundTask()
{

	RoomModel^* rooms = new RoomModel^[100];

	for (int i = 0; i < 100; i++) {
		rooms[i] = nullptr;
	}

    GetRooms(rooms).then([this, rooms]() {
        boolean taskRegistered = false;
        Platform::String^ taskName = "GeoBackgroundTask";

        auto iter = BackgroundTaskRegistration::AllTasks->First();
        auto hascur = iter->HasCurrent;
        MessageDialog^ damn;
        while (hascur)
        {
            auto cur = iter->Current->Value;

            if (cur->Name == taskName)
            {
                taskRegistered = true;
                geofenceTask = safe_cast<BackgroundTaskRegistration^>(cur);
                break;
            }

            hascur = iter->MoveNext();
        }

        if (taskRegistered) {
            GenerateAllGeoFences(rooms);

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
                requestAccessTask.then([this, rooms](BackgroundAccessStatus backgroundAccessStatus)
                {

                    // Regardless of the answer, register the background task. If the user later adds this application
                    // to the lock screen, the background task will be ready to run.

                    // Create a new background task builder
                    BackgroundTaskBuilder^ geofenceTaskBuilder = ref new BackgroundTaskBuilder();

                    geofenceTaskBuilder->Name = "GeoBackgroundTask";
                    //geofenceTaskBuilder->TaskEntryPoint = "BackgroundTask.Background";
                    geofenceTaskBuilder->TaskEntryPoint = "BackgroundTask.GeofenceBackgroundTask";

                    // Create a new location trigger
                    auto trigger = ref new LocationTrigger(LocationTriggerType::Geofence);

                    // Associate the location trigger with the background task builder
                    geofenceTaskBuilder->SetTrigger(trigger);


                    // Register the background task
                    geofenceTask = geofenceTaskBuilder->Register();

                    // Register for background task completion notifications
                    taskCompletedToken = geofenceTask->Completed::add(ref new BackgroundTaskCompletedEventHandler(this, &GeoFence::OnCompleted));

                    GenerateAllGeoFences(rooms);
                });
            }
            catch (Exception^ ex)
            {
                damn = ref new MessageDialog(ex->ToString());
                damn->ShowAsync();
            }
        }
    });

}

void GeoFence::addGeoFence(RoomModel^ room)
{

	String^ title = room->title();
	bool exists;
	unsigned int index = 0;
	Windows::Foundation::Collections::IVector<Windows::Devices::Geolocation::Geofencing::Geofence^>^ vec = geofences;
	Platform::Collections::Vector<Platform::String^>^ geofencesID = ref new Platform::Collections::Vector<Platform::String^>();

	for each (auto var in vec)
	{
		geofencesID->Append(var->Id);
	}

	exists = geofencesID->IndexOf(title, &index);

	if (exists)
	{
		geofences->RemoveAt(index);
	}
	Geofence^ geo = GenerateGeoFence(room);

	geofences->InsertAt(0, geo);
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
			// add background events to listbox
			//	FillEventListBoxWithExistingEvents();
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


RoomModel^ GeoFence::ReadRoomFromStorage()
{
	auto curRoom = ((App^)Application::Current)->currentRoom;

	if (!curRoom) {
		// we dont have a room created yet so do nothing
		OutputDebugString(L"nothing to load\n");
		return nullptr;
	}

	OutputDebugString(L"loading storage from datafile.txt\n");

	auto filename = curRoom->FileName;

	concurrency::task<StorageFile^> getFileOperation(localFolder->GetFileAsync(filename));


	getFileOperation.then([this](StorageFile^ file)
	{
		return FileIO::ReadTextAsync(file);
	}).then([this](concurrency::task<String^> previousOperation) {
		String^ roomAsText;

		auto thisApp = ((App^)Application::Current);
		auto currentRoom = thisApp->currentRoom;

		try {
			roomAsText = previousOperation.get();

			const wchar_t* data = roomAsText->Data();
			auto len = wcslen(data);

			int i = 0;
			int j = 0;
			Platform::String^ rows[20] = { nullptr };
			int row = 0;

			while (i < len) {
				for (int x = i; x < len; x++) {
					if (data[x] == '\n') {
						j = x;
						break;
					}
				}

				wchar_t line[1000] = { 0 };
				wcsncpy(line, data + i, j - i);

				rows[row++] = ref new String(line);


				i = j + 1;

			}
			currentRoom->title(rows[0]);
			currentRoom->description(rows[1]);
			currentRoom->lengthCm(std::stoi(rows[2]->Data()));
			currentRoom->widthCm(std::stoi(rows[3]->Data()));
			currentRoom->heightCm(std::stoi(rows[4]->Data()));

			currentRoom->latitude(rows[5]);
			currentRoom->longitude(rows[6]);

			auto currentWall1 = ref new WallModel;
			currentWall1->title(rows[7]);
			currentWall1->description(rows[8]);

			auto currentWall2 = ref new WallModel;
			currentWall2->title(rows[9]);
			currentWall2->description(rows[10]);

			auto currentWall3 = ref new WallModel;
			currentWall3->title(rows[11]);
			currentWall3->description(rows[12]);

			auto currentWall4 = ref new WallModel;
			currentWall4->title(rows[13]);
			currentWall4->description(rows[14]);

			auto currentCeiling = ref new WallModel;
			currentCeiling->title(rows[15]);
			currentCeiling->description(rows[16]);

			auto currentFloor = ref new WallModel;
			currentFloor->title(rows[17]);
			currentFloor->description(rows[18]);

			currentRoom->wall1(currentWall1);
			currentRoom->wall2(currentWall2);
			currentRoom->wall3(currentWall3);
			currentRoom->wall4(currentWall4);
			currentRoom->ceiling(currentCeiling);
			currentRoom->floor(currentFloor);

			// + addresser för bilder

			return currentRoom;
		}
		catch (...) {
		}
	});
}