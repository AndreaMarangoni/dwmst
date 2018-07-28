#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

namespace dwmst
{

struct UnableToOpenFile : std::runtime_error
{
    UnableToOpenFile(const std::string & filename) :
        std::runtime_error("UnableToOpenFile:" + filename)
    {}
};

}

#endif // EXCEPTIONS_H
