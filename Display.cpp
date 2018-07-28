#include "Display.h"

dwmst::Display::Display() :
    display_(nullptr)
{
    display_ = XOpenDisplay(nullptr);
}

bool dwmst::Display::isDisplayOpen() const noexcept
{
    return static_cast<bool>(display_);
}

dwmst::Display::~Display()
{
    if(display_)
    {
        XCloseDisplay(display_);
    }
}

void dwmst::Display::setStatus(const char *status) const noexcept
{
    XStoreName(display_, DefaultRootWindow(display_), status);
    XSync(display_, False);
}
