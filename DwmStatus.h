#ifndef DWMSTATUS_H
#define DWMSTATUS_H

#include <string>
#include <limits>

namespace dwmst
{

class Display;

struct DwnStatus
{
    DwnStatus(Display & display);

    void run();

private:
    Display & display_;
    float max_;
    float min_;
};

}

#endif // DWMSTATUS_H
