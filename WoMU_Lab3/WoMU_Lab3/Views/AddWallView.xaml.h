//
// AddWallView.xaml.h
// Declaration of the AddWallView class
//

#pragma once

#include "Views\AddWallView.g.h"

namespace WoMU_Lab3
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AddWallView sealed
	{
	public:
		AddWallView();
	private:
		void CameraButton(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void TakePhotoCommand(Windows::UI::Popups::IUICommand^command);
		void ChoosePictureCommand(Windows::UI::Popups::IUICommand^command);
		void AddToRoom_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void GoToPreviousPage_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
