#pragma once

#include <SFML/Window/Keyboard.hpp>

namespace wf::gfx::event {
    struct WindowKeyPressed {
        sf::Keyboard::Key key;
    };
}
