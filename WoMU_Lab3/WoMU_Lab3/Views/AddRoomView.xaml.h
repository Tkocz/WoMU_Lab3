//
// AddRoomView.xaml.h
// Declaration of the AddRoomView class
//

#pragma once

#include "Views\AddRoomView.g.h"

namespace WoMU_Lab3
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AddRoomView sealed
	{
	public:
		AddRoomView();
	private:
		void Wall1ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void Wall2ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void Wall3ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void FloorButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void CeilingButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void Wall4ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
	};
}
