using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LED_Strahler_GUI
{
    public enum LED_Strahler_Commands
    {
        PingRequest = 0x00,
        SetGroup = 0x01,
        SetTimebase = 0x02,
        SetRGB = 0x03,
        SetRGBW = 0x04,
        SetHSV = 0x05,
        StrobeRGB = 0x06,
        StrobeRGBW = 0x07,
        StrobeHSV = 0x08,
        FadeHue = 0x09,
        FadeSaturation = 0x0A,
        FadeValue = 0x0B,
        GetTemperature = 0x10,
        Ignore = 0xFF
    };
}
