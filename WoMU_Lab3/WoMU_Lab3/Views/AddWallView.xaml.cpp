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


using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Media::Capture;
using namespace Windows::Storage;
using namespace Windows::Foundation;

AddWallView::AddWallView()
{
	InitializeComponent();
}


void WoMU_Lab3::AddWallView::CameraButton(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
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
