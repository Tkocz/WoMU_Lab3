//
// AddRoomView.xaml.cpp
// Implementation of the AddRoomView class
//

#include "pch.h"
#include "AddRoomView.xaml.h"
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

AddRoomView::AddRoomView()
{
	InitializeComponent();
}


void WoMU_Lab3::AddRoomView::Room1ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	Frame->Navigate(AddWallView::typeid);
}
