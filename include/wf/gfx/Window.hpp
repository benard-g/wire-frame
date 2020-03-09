#pragma once

#include <cstdint>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

#include "wf/core/Line.hpp"
#include "wf/core/EventManager.hpp"

namespace wf::gfx {
    class Window {
    public:
        Window(uint32_t width, uint32_t height, std::string const &title, uint32_t frameRate);

        void clear(sf::Color color = sf::Color::Black);

        void drawLine(wf::core::Line const &line);

        void pollEvents(wf::core::EventManager &eventManager);

        void display();

        void close();

        bool isOpen() const noexcept;

        sf::Vector2u getSize() const noexcept;

    public:
        // Coplien
        Window() = delete;
        ~Window() noexcept = default;
        Window(Window const &) = delete;
        Window &operator=(Window const &) = delete;
        Window(Window &&) noexcept = delete;
        Window &operator=(Window &&) noexcept = delete;

    private:
        sf::RenderWindow m_window;
    };
}
