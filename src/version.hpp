#ifndef VERSION_H
#define VERSION_H

#include <cstdint>
#include <string>

namespace version
{

static const uint32_t MAJOR = UINT32_C(1);
static const uint32_t MINOR = UINT32_C(1);
static const uint32_t PATCH = UINT32_C(0);

static const std::string VERSION_STRING{std::to_string(MAJOR) + "." +
            std::to_string(MINOR) + "." +
            std::to_string(PATCH)};

} // version namespace

#endif // VERSION_H
