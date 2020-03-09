#pragma once

#include <cstdint>
#include <string>

namespace wf {
    struct Config {
    public:
        uint32_t const windowWidth;
        uint32_t const windowHeight;
        std::string const windowName;
        uint32_t const windowFrameRate;

    public:
        static Config loadDefault();
    };
}
