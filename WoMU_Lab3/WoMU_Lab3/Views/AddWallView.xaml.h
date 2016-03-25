//
// AddWallView.xaml.h
// Declaration of the AddWallView class
//

#pragma once

#include "Views\AddWallView.g.h"
#include "Models\WallModel.h"

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

using namespace Windows::UI::Popups;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Media::Capture;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;

namespace WoMU_Lab3
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AddWallView sealed
	{
	public:
		AddWallView();
	private:
        property String^ ImageFile;
        int wallIndex;
		WallModel^ currentWall = ref new WallModel;
		void CameraButton(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void TakePhotoCommand(Windows::UI::Popups::IUICommand^command);
		void ChoosePictureCommand(Windows::UI::Popups::IUICommand^command);
		void AddToRoom_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void GoToPreviousPage_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        Windows::Storage::StorageFile^ cpp_sucks_file;
	};
}
