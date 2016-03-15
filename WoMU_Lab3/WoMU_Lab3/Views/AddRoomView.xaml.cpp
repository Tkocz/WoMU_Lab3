//
// AddRoomView.xaml.cpp
// Implementation of the AddRoomView class
//

#include "pch.h"
#include "AddRoomView.xaml.h"
#include "AddWallView.xaml.h"
#include <string.h>
#include "Models\RoomModel.h"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

using namespace WoMU_Lab3;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Devices::Geolocation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;


AddRoomView::AddRoomView()
{
	InitializeComponent();
	ReadRoomFromStorage();
}


//--------------------------------- Storage functions------------------------------

void AddRoomView::WriteRoomToStorage()
{
	concurrency::task<StorageFile^> fileOperation =
		concurrency::create_task(localFolder->CreateFileAsync("dataFile.txt", CreationCollisionOption::ReplaceExisting));
	
	
	currentRoom->title(titleBox->Text);
	currentRoom->description(detailsBox->Text);
	currentRoom->lengthCm(lengthSlider->Value);
	currentRoom->widthCm(widthSlider->Value);
	currentRoom->heightCm(heightSlider->Value);
	currentRoom->latitude(ScenarioOutput_Latitude->Text);
	currentRoom->longitude(ScenarioOutput_Longitude->Text);

	fileOperation.then([this](StorageFile^ sampleFile)
	{

		Platform::String^ text = "";

		text += currentRoom->title() + "\n";
		text += currentRoom->description() + "\n";
		text += currentRoom->lengthCm() + "\n";
		text += currentRoom->widthCm() + "\n";
		text += currentRoom->heightCm() + "\n";
		text += currentRoom->latitude() + "\n";
		text += currentRoom->longitude() + "\n";

		return FileIO::WriteTextAsync(sampleFile, text);
	}).then([this](concurrency::task<void> previousOperation) {
		try {
			previousOperation.get();
		}
		catch (Platform::Exception^) {
			OutputDebugString(L"Shit went wrong \n");
		}
	});
}

// Read data from a file

void AddRoomView::ReadRoomFromStorage()
{

	String^ shit = ref new String();

	TextBox^ temp = detailsBox;

	concurrency::task<StorageFile^> getFileOperation(localFolder->GetFileAsync("dataFile.txt"));


	getFileOperation.then([this](StorageFile^ file)
	{
		return FileIO::ReadTextAsync(file);
	}).then([this, temp](concurrency::task<String^> previousOperation) {
		String^ timestamp;

		try {
			// Data is contained in timestamp
			timestamp = previousOperation.get();
			temp->Text = timestamp;
		}
		catch (...) {
			// Timestamp not found
		}
	});


}


//--------------------------- Button functions-----------------------------------------

void WoMU_Lab3::AddRoomView::Wall1ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::Wall2ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::Wall3ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::FloorButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::CeilingButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::Wall4ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::LenghtSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	lengthTextBlock->Text = "Längd i cm: " + lengthSlider->Value.ToString();
}


void WoMU_Lab3::AddRoomView::WidthSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	widthTextBlock->Text = "Bredd i cm: " + widthSlider->Value.ToString();
}


void WoMU_Lab3::AddRoomView::HeightSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	heightTextBlock->Text = "Höjd i cm: " + heightSlider->Value.ToString();
}


void WoMU_Lab3::AddRoomView::GoToPreviousPage_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	WriteRoomToStorage();
	Frame->GoBack();
}

void WoMU_Lab3::AddRoomView::GetGeolocationButtonClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	GetGeolocationButton->IsEnabled = false;
	LocationDisabledMessage->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

	task<GeolocationAccessStatus> geolocationAccessRequestTask(Windows::Devices::Geolocation::Geolocator::RequestAccessAsync());
	geolocationAccessRequestTask.then([this](task<GeolocationAccessStatus> accessStatusTask)
	{
		// Get will throw an exception if the task was canceled or failed with an error
		auto accessStatus = accessStatusTask.get();

		if (accessStatus == GeolocationAccessStatus::Allowed)
		{

			auto geolocator = ref new Windows::Devices::Geolocation::Geolocator();
			geolocator->DesiredAccuracyInMeters = desiredAccuracyInMetersValue;

			task<Geoposition^> geopositionTask(geolocator->GetGeopositionAsync(), geopositionTaskTokenSource.get_token());
			geopositionTask.then([this](task<Geoposition^> getPosTask)
			{

				// Get will throw an exception if the task was canceled or failed with an error
				UpdateLocationData(getPosTask.get());

				GetGeolocationButton->IsEnabled = true;
			});
		}
		else //GeolocationAccessStatus::Unspecified:
		{
			MessageDialog^ damn = ref new MessageDialog("Unspecified error!");
			damn->ShowAsync();
			UpdateLocationData(nullptr);
		}
	});
}
void WoMU_Lab3::AddRoomView::UpdateLocationData(Windows::Devices::Geolocation::Geoposition^ position)
	{
		if (position == nullptr)
		{
			ScenarioOutput_Latitude->Text = "No data";
			ScenarioOutput_Longitude->Text = "No data";
		}
		else
		{
			ScenarioOutput_Latitude->Text = position->Coordinate->Point->Position.Latitude.ToString();
			ScenarioOutput_Longitude->Text = position->Coordinate->Point->Position.Longitude.ToString();
		}
}
