
#include "pch.h"
#include "Views/AddRoomView.xaml.h"
#include "RoomsList.h"

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


concurrency::task<std::vector<RoomModel^>> GetRooms()
{
	return concurrency::create_task(ApplicationData::Current->LocalFolder->GetFilesAsync()).then([=](IVectorView<StorageFile^>^ filesInFolder) {

        std::vector<RoomModel^> rooms{};
		
		for (auto it = filesInFolder->First(); it->HasCurrent; it->MoveNext())
		{
			StorageFile^ file = it->Current;
			auto s = file->Name->Data();

			int n = file->Name->Length();
			if (s[n - 3] != 't' || s[n - 2] != 'x' || s[n - 1] != 't')
				continue;

			LoadRoom(file->Name).then([&rooms, filename = file->Name](RoomModel^ room) {
                rooms.push_back(room);
			});
		}

        return rooms;
	});
}

concurrency::task<RoomModel^> LoadRoom(String^ filename)
{
	concurrency::task<StorageFile^> getFileOperation(ApplicationData::Current->LocalFolder->GetFileAsync(filename));


	return getFileOperation.then([](StorageFile^ file)
	{
		return FileIO::ReadTextAsync(file);
	}).then([filename](concurrency::task<String^> previousOperation) {
		String^ roomAsText;

		auto room = ref new RoomModel;

		room->FileName = filename;

		try
		{
			roomAsText = previousOperation.get();

			const wchar_t* data = roomAsText->Data();
			auto len = wcslen(data);

			int i = 0;
			int j = 0;
			Platform::String^ rows[99] = { nullptr };
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
			w1->ImageFile = rows[9];

			w2->title(rows[10]);
			w2->description(rows[11]);
			w2->ImageFile = rows[12];

			w3->title(rows[13]);
			w3->description(rows[14]);
			w3->ImageFile = rows[15];

			w4->title(rows[16]);
			w4->description(rows[17]);
			w4->ImageFile = rows[18];

			wceil->title(rows[19]);
			wceil->description(rows[20]);
			wceil->ImageFile = rows[21];

			wfloor->title(rows[22]);
			wfloor->description(rows[23]);
			wfloor->ImageFile = rows[24];

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
			OutputDebugStringA("something is afuck!\n");
			//			OutputDebugString(L"Not Good");
		}

		return (RoomModel^)nullptr;
	});
}