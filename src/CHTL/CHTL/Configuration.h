#pragma once

#include <string>
#include <map>

namespace CHTL {

class Configuration {
public:
    bool debugMode = false;
    bool disableStyleAutoAddClass = false;
    bool disableStyleAutoAddId = false;
    int indexInitialCount = 0;
};

} // namespace CHTL
