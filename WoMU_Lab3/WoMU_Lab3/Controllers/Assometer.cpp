#include "pch.h"

#include "Assometer.h"

using namespace Windows::Foundation;

wchar_t* Assometer::orient = L"";
std::function<void(wchar_t*)> Assometer::sfn = nullptr;

double Assometer::ox = 0.0;
double Assometer::oy = 0.0;
double Assometer::oz = 0.0;

Accelerometer^ Assometer::acco = nullptr;


void Assometer::Enable() {
    try {

        acco = Accelerometer::GetDefault();

        // No ass-o-meter available.
        if (!acco)
            return;

        acco->ReportLatency = acco->MinimumReportInterval;

        acco->ReadingChanged += ref new TypedEventHandler<Accelerometer^, AccelerometerReadingChangedEventArgs ^>(&Assometer::OnReadingChanged);
    }
    catch {
        acco = nullptr;
    }
}


void Assometer::OnReadingChanged(Accelerometer^ sender, AccelerometerReadingChangedEventArgs^ e)
{
    auto r = e->Reading;

    auto x = r->AccelerationX - ox;
    auto y = r->AccelerationY;
    auto z = r->AccelerationZ;

    wchar_t* o = nullptr;

    if (x > 0.7) {
        o = L"left";
        ox += 1.0;
    }
    else if (x < -0.7) {
        o = L"right";
        ox += -1.0;
    }
    else if (y > 0.7) {
        o = L"down";
    }
    else if (y < -0.7) {
        o = L"up";
    }

    if (o != orient) {
        orient = o;

        if (o != nullptr) {
            OutputDebugString(L"orientation change: ");
            OutputDebugString(orient);
            OutputDebugString(L"\n");

            if (sfn != nullptr) {
                sfn(o);
            }
        }

    }

}

void Assometer::Callback(std::function<void(wchar_t*)> fn) {
    sfn = fn;
}

/*
{
    // Establish the report interval
    accelerometerOriginal->ReportInterval = accelerometerReadingTransform->MinimumReportInterval;
    accelerometerReadingTransform->ReportInterval = accelerometerReadingTransform->MinimumReportInterval;

    // Establish the ReadingTransform to align with the current display orientation, so 
    // that the accelerometer data from 'accelerometerReadingTransform' will align with the 
    // current display orientation
    accelerometerReadingTransform->ReadingTransform = displayInformation->CurrentOrientation;

    visibilityToken = Window::Current->VisibilityChanged += ref new WindowVisibilityChangedEventHandler(this, &Scenario4_OrientationChanged::VisibilityChanged);

    // Register for ReadingChanged event
    readingTokenOriginal = accelerometerOriginal->ReadingChanged += ref new TypedEventHandler<Accelerometer^, AccelerometerReadingChangedEventArgs^>(this, &Scenario4_OrientationChanged::ReadingChangedOriginal);
    readingTokenReadingTransform = accelerometerReadingTransform->ReadingChanged += ref new TypedEventHandler<Accelerometer^, AccelerometerReadingChangedEventArgs^>(this, &Scenario4_OrientationChanged::ReadingChangedReadingTransform);

    ScenarioEnableButton->IsEnabled = false;
    ScenarioDisableButton->IsEnabled = true;
    */