#include <iostream>

#include "wf/gfx/Window.hpp"
#include "wf/gfx/event/WindowClosed.hpp"
#include "Config.hpp"

void run(wf::Config const &config) {
    wf::gfx::Window window(config.windowWidth, config.windowHeight, config.windowName, config.windowFrameRate);
    wf::core::EventManager eventManager;

    eventManager.subscribe<wf::gfx::event::WindowClosed>([&window](auto) {
        window.close();
    });

    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        window.pollEvents(eventManager);

        // TODO render scene

        window.display();
    }
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
