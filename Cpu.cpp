#include "Cpu.h"

#include "Utils.h"

namespace
{
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
    minScaling_ = getValueFromFile(getCpuFilename(number_, "scaling_min_freq"));
    maxScaling_ = getValueFromFile(getCpuFilename(number_, "scaling_max_freq"));
}

float dwmst::Cpu::scalingFrequencyPercent() const
{
    const int currentFrequency = getValueFromFile(getCpuFilename(number_, "scaling_cur_freq"));
    if(currentFrequency < minScaling_)
    {
        minScaling_ = currentFrequency;
    }
    float frequency = static_cast<float>(currentFrequency);
    frequency = ((frequency - minScaling_) * 100) / (maxScaling_ - minScaling_);
    return frequency;
}
