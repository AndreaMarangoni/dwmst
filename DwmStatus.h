#ifndef DWMSTATUS_H
#define DWMSTATUS_H

#include <string>
#include <limits>
#include <thread>

#include "Cpu.h"

namespace dwmst
{

class Display;

struct DwnStatus
{
    using Cpus = std::array<Cpu, 4>;

    DwnStatus(Display & display);

    void run();

private:
    Display & display_;
    const int cpusAvailable_ = std::thread::hardware_concurrency();
    Cpus cpus_ = { Cpu(0), Cpu(1), Cpu(2), Cpu(3)};
};

}

#endif // DWMSTATUS_H
