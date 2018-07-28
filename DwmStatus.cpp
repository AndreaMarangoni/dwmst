#include "DwmStatus.h"

#include <iostream>
#include <thread>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "Display.h"
#include "Exceptions.h"

namespace
{
    std::string currentDateTime() noexcept
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%a %d %b %T %Z %Y");
        return ss.str();
    }

    int getValue(const std::string & filename)
    {
        int value = 0;
        std::ifstream file{filename, std::ios::in};
        if(!file.is_open())
        {
            throw dwmst::UnableToOpenFile(filename);
        }
        file >> value;
        return value;
    }

    int getBattery()
    {
        static const std::string chargeNowFilename{"/sys/class/power_supply/BAT1/charge_now"};
        static const std::string chargeFullFilename{"/sys/class/power_supply/BAT1/charge_full"};
        static const std::string voltageFilename{"/sys/class/power_supply/BAT1/voltage_now"};

        try
        {
            int charge_now = getValue(chargeNowFilename);
            int charge_full = getValue(chargeFullFilename);
            int voltage_now = getValue(voltageFilename);
            return ((float)charge_now * 1000 / (float)voltage_now) * 100 / ((float)charge_full * 1000 / (float)voltage_now);
        }
        catch(dwmst::UnableToOpenFile & ex)
        {
            std::cerr << ex.what() << '\n';
        }
        return -1;
    }


    std::string getBatteryLevel() noexcept
    {
        const int battery = getBattery();
        const std::string batteryLevel = "batt:" + std::to_string(battery);
        return batteryLevel;
    }
}

dwmst::DwnStatus::DwnStatus(dwmst::Display &display) :
    display_(display)
{
}

void dwmst::DwnStatus::run()
{
    while(true)
    {
        const std::string dateTime = currentDateTime();
        const std::string batteryLevel = getBatteryLevel();
        const std::string status = batteryLevel + " | " + dateTime;
        display_.setStatus(status.c_str());
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
