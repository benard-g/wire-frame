#include <iostream>

#include "wf/gfx/Window.hpp"
#include "wf/gfx/event/WindowClosed.hpp"
#include "wf/utils/Randomizer.hpp"
#include "wf/space/Scene.hpp"
#include "wf/space/shape/Cube.hpp"
#include "Config.hpp"

void run(wf::Config const &config) {
    wf::gfx::Window window(config.windowWidth, config.windowHeight, config.windowName, config.windowFrameRate);
    wf::core::EventManager eventManager;
    wf::utils::Randomizer randomizer(42);

    wf::space::Scene scene(
        window,
        randomizer,
        300,
        wf::core::Position{ .x=5, .y=5, .z=5 },
        wf::core::Orientation{ .x=0.1, .y=0.1, .z=0.1 }
    );

    eventManager.subscribe<wf::gfx::event::WindowClosed>([&window](auto) {
        window.close();
    });
    scene.registerEvents(eventManager);

    for (int i=0; i < 1000; ++i) {
        scene.addShape(
            std::make_unique<wf::space::shape::Cube>(
                50,
                sf::Color::Red
            ),
            wf::space::Mutator(
                wf::core::Position{ .x=0, .y=0, .z=0 },
                wf::core::Orientation{ .x=0, .y=0, .z=0 }
            )
        );
        scene.addShape(
            std::make_unique<wf::space::shape::Cube>(
                50,
                sf::Color::Green
            ),
            wf::space::Mutator(
                wf::core::Position{ .x=-100, .y=0, .z=0 },
                wf::core::Orientation{ .x=0, .y=0, .z=0 }
            )
        );
        scene.addShape(
            std::make_unique<wf::space::shape::Cube>(
                50,
                sf::Color::Blue
            ),
            wf::space::Mutator(
                wf::core::Position{ .x=100, .y=0, .z=0 },
                wf::core::Orientation{ .x=0, .y=0, .z=0 }
            )
        );
    }

    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        window.pollEvents(eventManager);

        scene.update();
        scene.render();

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
