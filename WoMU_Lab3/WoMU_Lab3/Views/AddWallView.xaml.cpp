//
// AddWallView.xaml.cpp
// Implementation of the AddWallView class
//

#include "pch.h"
#include "AddWallView.xaml.h"

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

using namespace Windows::UI::Popups;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Media::Capture;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Storage::Pickers;


AddWallView::AddWallView()
{
	InitializeComponent();
}



void WoMU_Lab3::AddWallView::CameraButton(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{

		// Create the message dialog and set its content
		MessageDialog^ msg = ref new MessageDialog("Vill du ta ett foto eller välja en bild?");

		// Add commands and set their callbacks.
		// Both commands use the same callback function instead of inline event handlers.
		UICommand^ cameraCommand = ref new UICommand(
			"Ta foto",
			ref new UICommandInvokedHandler(this, &WoMU_Lab3::AddWallView::TakePhotoCommand));
		UICommand^ fileChooseCommand = ref new UICommand(
			"Välj bild",
			ref new UICommandInvokedHandler(this, &WoMU_Lab3::AddWallView::ChoosePictureCommand));

		// Add the commands to the dialog
		msg->Commands->Append(cameraCommand);
		msg->Commands->Append(fileChooseCommand);

		// Set the command that will be invoked by default
		msg->DefaultCommandIndex = 0;

		// Set the command to be invoked when escape is pressed
		msg->CancelCommandIndex = 1;

		// Show the message dialog
		msg->ShowAsync();



}

void WoMU_Lab3::AddWallView::ChoosePictureCommand(Windows::UI::Popups::IUICommand^ command) {
	// Clear previous returned file name, if it exists, between iterations of this scenario
	//OutputTextBlock->Text = "";

	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;
	openPicker->FileTypeFilter->Append(".jpg");
	openPicker->FileTypeFilter->Append(".jpeg");
	openPicker->FileTypeFilter->Append(".png");

	/*
	create_task(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
	{
		if (file)
		{
			OutputTextBlock->Text = "Picked photo: " + file->Name;
		}
		else
		{
			OutputTextBlock->Text = "Operation cancelled.";
		}
	});*/
}

void WoMU_Lab3::AddWallView::TakePhotoCommand(Windows::UI::Popups::IUICommand^ command) {
	CameraCaptureUI^ dialog = ref new CameraCaptureUI();



	concurrency::task<StorageFile^>(dialog->CaptureFileAsync(CameraCaptureUIMode::Photo)).then([this](StorageFile^ file)
	{

		if (nullptr == file)
			return;

		concurrency::task<Streams::IRandomAccessStream^>(file->OpenAsync(FileAccessMode::Read)).then([this](Streams::IRandomAccessStream^ stream)
		{
			BitmapImage^ bitmapImage = ref new BitmapImage();
			bitmapImage->SetSource(stream);
			auto img = ref new Image();

			img->Source = bitmapImage;

			this->CameraButtonButton->Content = img;

		});

	});
}

void WoMU_Lab3::AddWallView::AddToRoom_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Frame->GoBack();
}

void WoMU_Lab3::AddWallView::GoToPreviousPage_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Frame->GoBack();
}
