#include <iostream>
#include <thread>

extern "C"
{
#include <X11/Xlib.h>
}

namespace dwmst
{

struct Display
{

    bool open() noexcept
    {
        display_ = XOpenDisplay(nullptr);
        if (!display_)
        {
            std::cerr << "dwmstatus: cannot open display.\n";
            return false;
        }
        return true;
    }

    ~Display()
    {
        if(display_)
        {
            XCloseDisplay(display_);
        }
    }

    void setStatus(char * status) noexcept
    {
        XStoreName(display_, DefaultRootWindow(display_), status);
        XSync(display_, False);
    }

private:
    ::Display * display_;
};

struct DwnStatus
{

    void run()
    {
        while(true)
        {
            // do some job

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    }
};

}


int main()
{
    return 0;
}
