#pragma once

#include <SFML/Graphics/Color.hpp>

#include "IShape.hpp"

namespace wf::space::shape {
    class Cube: public IShape {
    public:
        Cube(double size, sf::Color color);

        void generateLines(LineInserter const &insertLine) override;

    public:
        // Coplien
        Cube() = delete;
        ~Cube() noexcept override = default;
        Cube(Cube const &) = delete;
        Cube &operator=(Cube const &) = delete;
        Cube(Cube &&) noexcept = delete;
        Cube &operator=(Cube &&) noexcept = delete;

    private:
        double m_size;
        sf::Color m_color;
    };
}
