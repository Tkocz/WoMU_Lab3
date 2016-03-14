//
// AddRoomView.xaml.cpp
// Implementation of the AddRoomView class
//

#include "pch.h"
#include "AddRoomView.xaml.h"
#include "AddWallView.xaml.h"
#include <string>
#include "Models\RoomModel.h"



using namespace WoMU_Lab3;

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

AddRoomView::AddRoomView()
{
	InitializeComponent();
}


//--------------------------------- Storage functions------------------------------

void AddRoomView::WriteRoomToStorage()
{
	concurrency::task<StorageFile^> fileOperation =
		concurrency::create_task(localFolder->CreateFileAsync("dataFile.txt", CreationCollisionOption::ReplaceExisting));
	
	
	RoomModel^ currentRoom = ref new RoomModel;
	currentRoom->title(titleBox->Text);
	currentRoom->description(detailsBox->Text);
	currentRoom->lengthCm(lengthSlider->Value);
	currentRoom->widthCm(widthSlider->Value);
	currentRoom->heightCm(heightSlider->Value);

	fileOperation.then([this, currentRoom](StorageFile^ sampleFile)
	{

		Platform::String^ text = "";

		text += currentRoom->title() + "\n";
		text += currentRoom->description() + "\n";
		text += currentRoom->lengthCm() + "\n";
		text += currentRoom->widthCm() + "\n";
		text += currentRoom->heightCm() + "\n";


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
	concurrency::task<StorageFile^> getFileOperation(localFolder->GetFileAsync("dataFile.txt"));
	getFileOperation.then([this](StorageFile^ file)
	{
		return FileIO::ReadTextAsync(file);
	}).then([this](concurrency::task<String^> previousOperation) {
		String^ timestamp;

		try {
			// Data is contained in timestamp
			timestamp = previousOperation.get();
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
