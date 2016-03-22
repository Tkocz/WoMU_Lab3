#define _CRT_SECURE_NO_WARNINGS

//
// AddRoomView.xaml.cpp
// Implementation of the AddRoomView class
//

#include "pch.h"
#include "AddRoomView.xaml.h"
#include "AddWallView.xaml.h"
#include <string.h>
#include "Models\RoomModel.h"
#include <sstream>
#include <algorithm>
#include <iterator>
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

String^ AddRoomView::GetNextRoomFilename() {
    static const char alphanum[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    const int len = 24;
    char s[len+1] = { 0 };


    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;

    wchar_t buf[255] = { 0 };
    mbstowcs(buf, s, 128);
    return ref new String(buf) + ".txt";
}


AddRoomView::AddRoomView()
{
    InitializeComponent();

    // if thisApp->currentRoom == nullptr, create new storage file
    // otherwise, we just returned from wall view

    
    auto thisApp = ((App^)Application::Current);
    
    thisApp->currentWall = nullptr;
    
    if (!thisApp->currentRoom) {
        thisApp->currentRoom = ref new RoomModel;
        thisApp->currentRoom->FileName = GetNextRoomFilename();

        thisApp->currentRoom->wall1(ref new WallModel);
        thisApp->currentRoom->wall2(ref new WallModel);
        thisApp->currentRoom->wall3(ref new WallModel);
        thisApp->currentRoom->wall4(ref new WallModel);
        thisApp->currentRoom->floor(ref new WallModel);
        thisApp->currentRoom->ceiling(ref new WallModel);

        OutputDebugString(L"new room model (filename=");
        OutputDebugString(thisApp->currentRoom->FileName->Data());
        OutputDebugString(L")\n");
    }
    else {
        ReadRoomFromStorage(false);
    }
    
}


//--------------------------------- Storage functions------------------------------

void AddRoomView::WriteRoomToStorage()
{
    auto thisApp = ((App^)Application::Current);
    auto currentRoom = thisApp->currentRoom;

    currentRoom->title(titleBox->Text);
    if (currentRoom->title()->Length() == 0)
        return;

    auto filename = thisApp->currentRoom->FileName;

    OutputDebugString(L"writing to ");
    OutputDebugString(filename->Data());
    OutputDebugString(L"\n");

    concurrency::task<StorageFile^> fileOperation =
        concurrency::create_task(localFolder->CreateFileAsync(filename, CreationCollisionOption::ReplaceExisting));
    
    
    currentRoom->description(detailsBox->Text);
    currentRoom->lengthCm(lengthSlider->Value);
    currentRoom->widthCm(widthSlider->Value);
    currentRoom->heightCm(heightSlider->Value);
    currentRoom->latitude(Output_Latitude->Text);
    currentRoom->longitude(Output_Longitude->Text);

    fileOperation.then([this](StorageFile^ sampleFile)
    {

        Platform::String^ text = "";

        auto thisApp = ((App^)Application::Current);
        auto currentRoom = thisApp->currentRoom;

        text += currentRoom->title() + "\n";
        text += currentRoom->description() + "\n";
        text += currentRoom->lengthCm() + "\n";
        text += currentRoom->widthCm() + "\n";
        text += currentRoom->heightCm() + "\n";

        text += currentRoom->latitude() + "\n";
        text += currentRoom->longitude() + "\n";

        text += currentRoom->wall1()->title() + "\n";
        text += currentRoom->wall1()->description() + "\n";

        text += currentRoom->wall2()->title() + "\n";
        text += currentRoom->wall2()->description() + "\n";

        text += currentRoom->wall3()->title() + "\n";
        text += currentRoom->wall3()->description() + "\n";

        text += currentRoom->wall4()->title() + "\n";
        text += currentRoom->wall4()->description() + "\n";

        text += currentRoom->ceiling()->title() + "\n";
        text += currentRoom->ceiling()->description() + "\n";

        text += currentRoom->floor()->title() + "\n";
        text += currentRoom->floor()->description() + "\n";

        // + adresser för bilder

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

void AddRoomView::ReadRoomFromStorage(bool loadFromFile)
{
    auto curRoom = ((App^)Application::Current)->currentRoom;

    if (!curRoom) {
        // we dont have a room created yet so do nothing
        OutputDebugString(L"nothing to load\n");
        return;
    }

    if (!loadFromFile) {

        titleBox->Text = curRoom->title();
        detailsBox->Text = curRoom->description();
        lengthSlider->Value = curRoom->lengthCm();
        widthSlider->Value = curRoom->widthCm();
        heightSlider->Value = curRoom->heightCm();
        Output_Latitude->Text = curRoom->latitude();
        Output_Longitude->Text = curRoom->longitude();
        return;
    }

    OutputDebugString(L"loading storage from");
    OutputDebugString(curRoom->FileName->Data());
    OutputDebugString(L"\n");

    auto filename = curRoom->FileName;

    concurrency::task<StorageFile^> getFileOperation(localFolder->GetFileAsync(filename));


    getFileOperation.then([this](StorageFile^ file)
    {
        return FileIO::ReadTextAsync(file);
    }).then([this](concurrency::task<String^> previousOperation) {
        String^ roomAsText;

        auto thisApp = ((App^)Application::Current);
        auto currentRoom = thisApp->currentRoom;

        try {
            roomAsText = previousOperation.get();

            const wchar_t* data = roomAsText->Data();
            auto len = wcslen(data);

            int i = 0;
            int j = 0;
            Platform::String^ rows[20] = { nullptr };
            int row = 0;

            while (i < len) {
                for (int x = i; x < len; x++) {
                    if (data[x] == '\n') {
                        j = x;
                        break;
                    }
                }

                wchar_t line[1000] = { 0 };
                wcsncpy(line, data + i, j - i);

                rows[row++] = ref new String(line);


                i = j + 1;

            }
            currentRoom->title(rows[0]);
            currentRoom->description(rows[1]);
            currentRoom->lengthCm(std::stoi(rows[2]->Data()));
            currentRoom->widthCm(std::stoi(rows[3]->Data()));
            currentRoom->heightCm(std::stoi(rows[4]->Data()));

            currentRoom->latitude(rows[5]);
            currentRoom->longitude(rows[6]);

            currentWall1->title(rows[7]);
            currentWall1->description(rows[8]);

            currentWall2->title(rows[9]);
            currentWall2->description(rows[10]);

            currentWall3->title(rows[11]);
            currentWall3->description(rows[12]);

            currentWall4->title(rows[13]);
            currentWall4->description(rows[14]);

            currentCeiling->title(rows[15]);
            currentCeiling->description(rows[16]);

            currentFloor->title(rows[17]);
            currentFloor->description(rows[18]);

            currentRoom->wall1(ref new WallModel);
            currentRoom->wall2(ref new WallModel);
            currentRoom->wall3(ref new WallModel);
            currentRoom->wall4(ref new WallModel);
            currentRoom->floor(ref new WallModel);
            currentRoom->ceiling(ref new WallModel);
            /*
            currentRoom->wall1(currentWall1);
            currentRoom->wall2(currentWall2);
            currentRoom->wall3(currentWall3);
            currentRoom->wall4(currentWall4);
            currentRoom->ceiling(currentCeiling);
            currentRoom->floor(currentFloor);*/

            // + addresser för bilder

            App^ thisApp = (App^)Application::Current;

            thisApp->currentRoom = currentRoom;


            titleBox->Text = currentRoom->title();
            detailsBox->Text = currentRoom->description();
            lengthSlider->Value = currentRoom->lengthCm();
            widthSlider->Value = currentRoom->widthCm();
            heightSlider->Value = currentRoom->heightCm();
            Output_Latitude->Text = currentRoom->latitude();
            Output_Longitude->Text = currentRoom->longitude();

//            OutputDebugString(L"view model updated\n");
            
        }
        catch (...) {
//			OutputDebugString(L"Not Good");
        }
    });


}


//--------------------------- Button functions-----------------------------------------

void WoMU_Lab3::AddRoomView::Wall1ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
    WriteRoomToStorage();
    App^ thisApp = (App^)Application::Current;
    thisApp->currentWall = thisApp->currentRoom->wall1();
    thisApp->currentWallIndex = 1;
    Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::Wall2ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
    WriteRoomToStorage();
    App^ thisApp = (App^)Application::Current;
    thisApp->currentWall = thisApp->currentRoom->wall2();
    thisApp->currentWallIndex = 2;
    Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::Wall3ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
    WriteRoomToStorage();
    App^ thisApp = (App^)Application::Current;
    thisApp->currentWall = thisApp->currentRoom->wall3();
    thisApp->currentWallIndex = 3;

    Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::FloorButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
    WriteRoomToStorage();
    App^ thisApp = (App^)Application::Current;
    thisApp->currentWall = thisApp->currentRoom->floor();
    thisApp->currentWallIndex = 6;
    Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::CeilingButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
    WriteRoomToStorage();
    App^ thisApp = (App^)Application::Current;
    thisApp->currentWall = thisApp->currentRoom->ceiling();
    thisApp->currentWallIndex = 5;
    Frame->Navigate(AddWallView::typeid);
}


void WoMU_Lab3::AddRoomView::Wall4ButtonTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
    WriteRoomToStorage();
    App^ thisApp = (App^)Application::Current;
    thisApp->currentWall = thisApp->currentRoom->wall4();
    thisApp->currentWallIndex = 4;

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
    auto thisApp = ((App^)Application::Current);
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
            Output_Latitude->Text = "No data";
            Output_Longitude->Text = "No data";
        }
        else
        {
            auto thisApp = ((App^)Application::Current);
            auto currentRoom = thisApp->currentRoom;

            Output_Latitude->Text = position->Coordinate->Point->Position.Latitude.ToString();
            currentRoom->latitude(position->Coordinate->Point->Position.Latitude.ToString());
            Output_Longitude->Text = position->Coordinate->Point->Position.Longitude.ToString();
            currentRoom->longitude(position->Coordinate->Point->Position.Longitude.ToString());
        }
}
