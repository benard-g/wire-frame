#pragma once

#include <SFML/Graphics/Color.hpp>

#include "./Position.hpp"

namespace wf::core {
    struct Line {
        Position p1;
        Position p2;
        sf::Color color;
    };
}
