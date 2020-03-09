#include "Config.hpp"

wf::Config wf::Config::loadDefault() {
    return {
        .windowWidth = 1280,
        .windowHeight = 720,
        .windowName = "Wire-Frame",
        .windowFrameRate = 30
    };
}
