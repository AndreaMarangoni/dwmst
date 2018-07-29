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

    std::string separator() noexcept
    {
        return "·";
    }

    std::string getCpuInfo(float min, float max)
    {
        const int concurentThreadsSupported = std::thread::hardware_concurrency();
        const int pos = 27;
        std::string cpu{"/sys/devices/system/cpu/cpuN/cpufreq/scaling_cur_freq"};
        std::stringstream ss;


        ss << openSeparator();
        for(int i = 0; i < concurentThreadsSupported; ++i)
        {
            cpu.replace(pos, 1, std::to_string(i));
            int val = getValue(cpu);
            if(val < min)
            {
                min = val;
            }
            else if(val > max)
            {
                max = val;
            }
            float freq = static_cast<float>(val);
            freq = ((freq - min) * 100) / (max - min);
            ss << std::fixed << std::setprecision(1) << freq << '%' << " ";
        }
        ss << closeSeparator();

        return ss.str();
    }

}

dwmst::DwnStatus::DwnStatus(dwmst::Display &display) :
    display_(display)
{
    const std::string max{"/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"};
    const std::string min{"/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"};
    min_ = getValue(min);
    max_ = getValue(max);
}

void dwmst::DwnStatus::run()
{
    while(true)
    {
        const std::string dateTime = openSeparator() + currentDateTime() + closeSeparator();
        const std::string batteryLevel = openSeparator() + getBatteryLevel() + closeSeparator();
        const std::string cpus = getCpuInfo( min_, max_);
        const std::string status = cpus + batteryLevel + dateTime;
        display_.setStatus(status.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
