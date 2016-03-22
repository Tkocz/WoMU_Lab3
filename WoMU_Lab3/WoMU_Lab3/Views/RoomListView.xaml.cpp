﻿//
// RoomListView.xaml.cpp
// Implementation of the RoomListView class
//

#include "pch.h"
#include "RoomListView.xaml.h"
#include "AddRoomView.xaml.h"

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

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

RoomListView::RoomListView()
{
	InitializeComponent();

    concurrency::create_task(localFolder->GetFilesAsync()).then([=](IVectorView<StorageFile^>^ filesInFolder) {
        //Iterate over the results and print the list of files
        // to the visual studio output window
        for (auto it = filesInFolder->First(); it->HasCurrent; it->MoveNext())
        {
            StorageFile^ file = it->Current;

            LoadRoom(file->Name).then([this, filename=file->Name](RoomModel^ room) {
                auto o = this->RoomsList;
                auto btn = ref new Button();

                btn->Content = room->title();
                btn->Click += ref new Windows::UI::Xaml::RoutedEventHandler(this, &WoMU_Lab3::RoomListView::OnRoomClick);
                btn->Tag = room;

                o->Items->Append(btn);
            });
        }
    });

}

concurrency::task<RoomModel^> RoomListView::LoadRoom(String^ filename)
{
    concurrency::task<StorageFile^> getFileOperation(localFolder->GetFileAsync(filename));


    return getFileOperation.then([this](StorageFile^ file)
    {
        return FileIO::ReadTextAsync(file);
    }).then([this, filename](concurrency::task<String^> previousOperation) {
        String^ roomAsText;

        auto room = ref new RoomModel;

        room->FileName = filename;

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
            room->title(rows[0]);
            room->description(rows[1]);
            room->lengthCm(std::stoi(rows[2]->Data()));
            room->widthCm(std::stoi(rows[3]->Data()));
            room->heightCm(std::stoi(rows[4]->Data()));

            room->latitude(rows[5]);
            room->longitude(rows[6]);

            auto w1 = ref new WallModel;
            auto w2 = ref new WallModel;
            auto w3 = ref new WallModel;
            auto w4 = ref new WallModel;
            auto wceil = ref new WallModel;
            auto wfloor = ref new WallModel;

            w1->title(rows[7]);
            w1->description(rows[8]);

            w2->title(rows[9]);
            w2->description(rows[10]);

            w3->title(rows[11]);
            w3->description(rows[12]);

            w4->title(rows[13]);
            w4->description(rows[14]);

            wceil->title(rows[15]);
            wceil->description(rows[16]);

            wfloor->title(rows[17]);
            wfloor->description(rows[18]);

            room->wall1(w1);
            room->wall2(w2);
            room->wall3(w3);
            room->wall4(w4);
            room->floor(wfloor);
            room->ceiling(wceil);

            return room;

            //            OutputDebugString(L"view model updated\n");

        }
        catch (...) {
            //			OutputDebugString(L"Not Good");
        }

        return (RoomModel^)nullptr;
    });


}

void WoMU_Lab3::RoomListView::GoToPreviousPage_OnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Frame->GoBack();
}

void WoMU_Lab3::RoomListView::OnRoomClick(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e)
{
    auto thisApp = ((App^)Application::Current);
    thisApp->currentWall = nullptr;
    thisApp->currentRoom = (RoomModel^)(((Button^)sender)->Tag);

    Frame->Navigate(AddRoomView::typeid);
}
