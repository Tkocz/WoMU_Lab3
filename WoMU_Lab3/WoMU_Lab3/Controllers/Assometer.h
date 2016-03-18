#pragma once

using namespace Windows::Devices::Sensors;


class Assometer sealed
{
public:
    static void Enable();
    static void Callback(std::function<void (wchar_t*)> fn);
private:
    static std::function<void(wchar_t*)> sfn;
    static wchar_t* orient;
    static double ox;
    static double oy;
    static double oz;
    static Accelerometer^ acco;
    static void OnReadingChanged(Accelerometer ^sender, AccelerometerReadingChangedEventArgs ^args);
};
