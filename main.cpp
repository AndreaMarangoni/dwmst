#include <iostream>
#include <thread>
#include <sstream>
#include <iomanip>
#include <fstream>

extern "C"
{
#include <X11/Xlib.h>
}

namespace dwmst
{

struct Display
{

    Display() :
        display_(nullptr)
    {
        display_ = XOpenDisplay(nullptr);
    }

    bool isDisplayOpen() const noexcept
    {
        return static_cast<bool>(display_);
    }

    ~Display()
    {
        if(display_)
        {
            XCloseDisplay(display_);
        }
    }

    void setStatus(const char * status) noexcept
    {
        XStoreName(display_, DefaultRootWindow(display_), status);
        XSync(display_, False);
    }

private:
    ::Display * display_;
};



struct DwnStatus
{
    DwnStatus(Display & display) :
        display_(display)
    {
    }

    void run()
    {
        while(true)
        {
            const std::string dateTime = currentDateTime();
            const std::string batteryLevel = "batt:" + std::to_string(getBattery());
            const std::string status = batteryLevel + " | " + dateTime;
            display_.setStatus(status.c_str());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    std::string currentDateTime() const noexcept
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%a %d %b %T %Z %Y");
        return ss.str();
    }

    int getValue(const std::string & filename) const noexcept
    {
        int value = 0;
        std::ifstream file{filename, std::ios::in};
        if(!file.is_open())
        {
            std::cerr << "Error opening charge_now.\n";
            return -1;
        }
        file >> value;
        return value;
    }

    int getBattery() const noexcept
    {
        static const std::string chargeNowFilename{"/sys/class/power_supply/BAT1/charge_now"};
        static const std::string chargeFullFilename{"/sys/class/power_supply/BAT1/charge_full"};
        static const std::string voltageFilename{"/sys/class/power_supply/BAT1/voltage_now"};

        int charge_now = getValue(chargeNowFilename);
        int charge_full = getValue(chargeFullFilename);
        int voltage_now = getValue(voltageFilename);

        return ((float)charge_now * 1000 / (float)voltage_now) * 100 / ((float)charge_full * 1000 / (float)voltage_now);
    }

private:
    Display & display_;
};

}


int main()
{
    dwmst::Display display;

    if(!display.isDisplayOpen())
    {
        std::cerr << "dwmstatus: cannot open display.\n";
        return EXIT_FAILURE;
    }

    dwmst::DwnStatus status(display);

    status.run();

    return EXIT_SUCCESS;
}
