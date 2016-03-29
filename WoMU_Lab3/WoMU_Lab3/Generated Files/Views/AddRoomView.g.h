﻿#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------


namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class ScrollViewer;
                ref class TextBox;
                ref class TextBlock;
                ref class Slider;
                ref class Button;
            }
        }
    }
}

namespace WoMU_Lab3
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class AddRoomView : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::ScrollViewer^ listView;
        private: ::Windows::UI::Xaml::Controls::TextBox^ detailsBox;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ lengthTextBlock;
        private: ::Windows::UI::Xaml::Controls::Slider^ lengthSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ widthTextBlock;
        private: ::Windows::UI::Xaml::Controls::Slider^ widthSlider;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ heightTextBlock;
        private: ::Windows::UI::Xaml::Controls::Slider^ heightSlider;
        private: ::Windows::UI::Xaml::Controls::Button^ GetGeolocationButton;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ LocationDisabledMessage;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ Output_Longitude;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ Output_Latitude;
        private: ::Windows::UI::Xaml::Controls::Button^ wall4Button;
        private: ::Windows::UI::Xaml::Controls::Button^ floorButton;
        private: ::Windows::UI::Xaml::Controls::Button^ ceilingButton;
        private: ::Windows::UI::Xaml::Controls::Button^ wallButton;
        private: ::Windows::UI::Xaml::Controls::Button^ wall2Button;
        private: ::Windows::UI::Xaml::Controls::Button^ wall3Button;
        private: ::Windows::UI::Xaml::Controls::TextBox^ titleBox;
    };
}
