#include <iostream>

#include "Display.h"
#include "DwmStatus.h"

namespace dwmst
{
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
