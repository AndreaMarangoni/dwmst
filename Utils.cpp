#include "Utils.h"

namespace dwmst
{

int getValueFromFile(const std::string &filename)
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

}
