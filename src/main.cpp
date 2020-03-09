#include <iostream>
#include "Config.hpp"

void run(wf::Config const &config) {
    // TODO
}

int main() {
    try {
        wf::Config config = wf::Config::loadDefault();
        run(config);
    }
    catch (std::exception &exception) {
        std::cerr << "Error: " << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
