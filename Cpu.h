
#pragma once

namespace dwmst
{

struct Cpu
{
    Cpu(int number);
    float scalingFrequencyPercent() const;

private:
    int number_;
    int maxScaling_;
    mutable int minScaling_;
};

}
