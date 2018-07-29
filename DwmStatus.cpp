#include "DwmStatus.h"

#include <iostream>
#include <thread>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "Display.h"
#include "Exceptions.h"
#include "Cpu.h"

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

    const char * percent_bar(int p)
    {
        if(p == -1)
        {
            return "/";
        }
        const char *s[] = {
            "▁", "▂", "▃", "▄", "▅", "▆", "▇"
        };
        return s[(p * 6) / 100];
    }

    std::string getBatteryLevel() noexcept
    {
        const int battery = getBattery();
        const std::string batteryLevel = percent_bar(battery);
        return batteryLevel;
    }

    char openSeparator() noexcept
    {
        return '[';
    }

    char closeSeparator() noexcept
    {
        return ']';
    }

    std::string getCpuInfo(dwmst::DwnStatus::Cpus & cpus)
    {
        std::stringstream ss;
        ss << openSeparator();
        for(int i = 0; i < 4; ++i)
        {
            const dwmst::Cpu & cpu = cpus[i];
            const float freq = cpu.scalingFrequencyPercent();
            ss << std::fixed << std::setprecision(1) << freq << '%' << " ";
        }
        ss << closeSeparator();

        return ss.str();
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
        const std::string dateTime = openSeparator() + currentDateTime() + closeSeparator();
        const std::string batteryLevel = openSeparator() + getBatteryLevel() + closeSeparator();
        const std::string cpus = getCpuInfo(cpus_);
        const std::string status = cpus + batteryLevel + dateTime;
        display_.setStatus(status.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
}
