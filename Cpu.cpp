#include "Cpu.h"

#include <string>
#include <fstream>

#include "Exceptions.h"

namespace
{
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

    std::string getCpuFilename(int cpu, const std::string & file) noexcept
    {
        std::string filename = std::string{"/sys/devices/system/cpu/cpuN/cpufreq/"} + file;
        filename.replace(27, 1, std::to_string(cpu));
        return filename;
    }

}

dwmst::Cpu::Cpu(int number):
    number_(number),
    maxScaling_(),
    minScaling_()
{
    minScaling_ = getValue(getCpuFilename(number_, "scaling_min_freq"));
    maxScaling_ = getValue(getCpuFilename(number_, "scaling_max_freq"));
}

float dwmst::Cpu::scalingFrequencyPercent() const
{
    const int currentFrequency = getValue(getCpuFilename(number_, "scaling_cur_freq"));
    if(currentFrequency < minScaling_)
    {
        minScaling_ = currentFrequency;
    }
    float frequency = static_cast<float>(currentFrequency);
    frequency = ((frequency - minScaling_) * 100) / (maxScaling_ - minScaling_);
    return frequency;
}
