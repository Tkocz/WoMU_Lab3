
#pragma once

#include "MainPage.g.h"

namespace WoMU_Lab3
{
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void GoToRoomView_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void GoToListRoomView_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
