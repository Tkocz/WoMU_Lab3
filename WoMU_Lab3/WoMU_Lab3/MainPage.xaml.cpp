//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "Views\AddRoomView.xaml.h"
#include "Views\RoomListView.xaml.h"
#include "Controllers\Assometer.h"
#include "GeoFence.h"
#include "RoomsList.h"

using namespace WoMU_Lab3;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Devices::Geolocation;
using namespace Windows::Devices::Geolocation::Geofencing;
using namespace Windows::ApplicationModel::Background;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Popups;

MainPage::MainPage()
{
	InitializeComponent();

    Assometer::Enable();

	Geolocator^ geolocator = ref new Geolocator();
	GeoFence^ geoFencer = ref new GeoFence();
	geoFencer->RegisterBackgroundTask();
}


void WoMU_Lab3::MainPage::GoToRoomView_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    // generera filnamn och skicka med in till AddRoomView


    ((App^)Application::Current)->currentRoom = nullptr;
	Frame->Navigate(AddRoomView::typeid);
}


void WoMU_Lab3::MainPage::GoToListRoomView_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Frame->Navigate(RoomListView::typeid);
}
