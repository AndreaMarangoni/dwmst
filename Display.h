#ifndef DISPLAY_H
#define DISPLAY_H

extern "C"
{
    #include <X11/Xlib.h>
}

namespace dwmst
{

class Display
{
public:
    Display();
    ~Display();

    bool isDisplayOpen() const noexcept;
    void setStatus(const char * status) const noexcept;
private:
    ::Display * display_;
};

}

#endif // DISPLAY_H
