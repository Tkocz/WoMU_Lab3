﻿//
// AddWallView.xaml.cpp
// Implementation of the AddWallView class
//

#include "pch.h"
#include "AddWallView.xaml.h"

#include "Controllers\Assometer.h"

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

    App^ thisApp = (App^)Application::Current;

    InitializeComponent();

    //currentWall = thisApp->currentRoom->wall1();

    /*if (!currentWall) {
        currentWall = ref new WallModel;
        thisApp->currentRoom->wall1(thisApp->currentRoom->wall1());
    }
    */
    titleBox->Text = thisApp->currentWall->title();
    detailsBox->Text = thisApp->currentWall->description();
    OutputDebugString(thisApp->currentWall->title()->Data());
    switch (thisApp->currentWallIndex) {
    case 1: { this->RelevantWall->Text = ref new String(L"Vägg 1"); break; }
    case 2: { this->RelevantWall->Text = ref new String(L"Vägg 2"); break; }
    case 3: { this->RelevantWall->Text = ref new String(L"Vägg 3"); break; }
    case 4: { this->RelevantWall->Text = ref new String(L"Vägg 4"); break; }
    case 5: { this->RelevantWall->Text = ref new String(L"Taket"); break; }
    case 6: { this->RelevantWall->Text = ref new String(L"Golvet"); break; }
    }

    Assometer::Callback([this](wchar_t* dir) {
        this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this, dir]() {
            
            App^ thisApp = (App^)Application::Current;
            if (dir == L"up") {
                thisApp->currentWallIndex = 5;
                thisApp->currentWall = thisApp->currentRoom->ceiling();
                this->RelevantWall->Text = ref new Platform::String(L"Taket");
            }
            else if (dir == L"down") {
                thisApp->currentWallIndex = 6;
                thisApp->currentWall = thisApp->currentRoom->floor();
                this->RelevantWall->Text = ref new Platform::String(L"Golvet");
            }
            else if (dir == L"left") {
                thisApp->currentWallIndex--;
                if (thisApp->currentWallIndex < 1)
                    thisApp->currentWallIndex = 4;

                switch (thisApp->currentWallIndex) {
                case 1: { this->RelevantWall->Text = ref new String(L"Vägg 1"); thisApp->currentWall = thisApp->currentRoom->wall1(); break;  }
                case 2: { this->RelevantWall->Text = ref new String(L"Vägg 2"); thisApp->currentWall = thisApp->currentRoom->wall2(); break; }
                case 3: { this->RelevantWall->Text = ref new String(L"Vägg 3"); thisApp->currentWall = thisApp->currentRoom->wall3(); break; }
                case 4: { this->RelevantWall->Text = ref new String(L"Vägg 4"); thisApp->currentWall = thisApp->currentRoom->wall4(); break; }
                }

            }
            else if (dir == L"right") {
                thisApp->currentWallIndex++;
                if (thisApp->currentWallIndex > 4)
                    thisApp->currentWallIndex = 1;

                switch (thisApp->currentWallIndex) {
                case 1: { this->RelevantWall->Text = ref new String(L"Vägg 1"); thisApp->currentWall = thisApp->currentRoom->wall1(); break; }
                case 2: { this->RelevantWall->Text = ref new String(L"Vägg 2"); thisApp->currentWall = thisApp->currentRoom->wall2(); break; }
                case 3: { this->RelevantWall->Text = ref new String(L"Vägg 3"); thisApp->currentWall = thisApp->currentRoom->wall3(); break; }
                case 4: { this->RelevantWall->Text = ref new String(L"Vägg 4"); thisApp->currentWall = thisApp->currentRoom->wall4(); break; }
                }

            }
        }));
    });
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

    
    concurrency::task<StorageFile^>(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
    {
        if (nullptr == file)
            return;
        
        //OutputTextBlock->Text = "Picked photo: " + file->Name;
        concurrency::task<Streams::IRandomAccessStream^>(file->OpenAsync(FileAccessMode::Read)).then([this](Streams::IRandomAccessStream^ stream)
        {
            BitmapImage^ bitmapImage = ref new BitmapImage();
            bitmapImage->SetSource(stream);
            auto img = ref new Image();

            img->Source = bitmapImage;

            this->CameraButtonButton->Content = img;

        });
    });
    //this->CameraButtonButton->Content = file;
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
    App^ thisApp = (App^)Application::Current;

    thisApp->currentWall->title(titleBox->Text);
    OutputDebugString(L"title set to ");
    OutputDebugString(thisApp->currentWall->title()->Data());
    OutputDebugString(L"\n");
    thisApp->currentWall->description(detailsBox->Text);

    //thisApp->currentRoom->wall1(currentWall);
    
    Assometer::Callback(nullptr);
    Frame->GoBack();
}

void WoMU_Lab3::AddWallView::GoToPreviousPage_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Assometer::Callback(nullptr);
    Frame->GoBack();
}
