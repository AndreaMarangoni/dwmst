#ifndef DWMSTATUS_H
#define DWMSTATUS_H

#include <string>

namespace dwmst
{

class Display;

struct DwnStatus
{
    DwnStatus(Display & display);

    void run();

private:
    Display & display_;
};

}

#endif // DWMSTATUS_H
