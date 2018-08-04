#ifndef DWMSTATUS_H
#define DWMSTATUS_H

#include <string>
#include <limits>
#include <thread>
#include <vector>

#include "Cpu.h"

namespace dwmst
{

class Display;

struct DwnStatus
{
    using Cpus = std::vector<Cpu>;

    DwnStatus(Display & display);

    void run() noexcept;

private:
    Display & display_;
    const int cpusAvailable_ = std::thread::hardware_concurrency();
    Cpus cpus_;
};

}

#endif // DWMSTATUS_H
