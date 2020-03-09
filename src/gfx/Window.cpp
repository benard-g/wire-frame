#include <SFML/Window/Event.hpp>

#include "wf/gfx/Window.hpp"
#include "wf/gfx/event/WindowClosed.hpp"
#include "wf/gfx/event/WindowKeyPressed.hpp"

wf::gfx::Window::Window(uint32_t width, uint32_t height, std::string const &title, uint32_t frameRate):
    m_window(sf::VideoMode(width, height), title)
{
    m_window.setFramerateLimit(frameRate);
}

void wf::gfx::Window::clear(sf::Color color) {
    m_window.clear(color);
}

void wf::gfx::Window::drawLine(wf::core::Line const &line) {
    // TODO: Handle 3D with z-buffer
    sf::Vertex vertexLine[] = {
        sf::Vertex(sf::Vector2f(static_cast<float>(line.p1.x), static_cast<float>(line.p1.y)), line.color),
        sf::Vertex(sf::Vector2f(static_cast<float>(line.p2.x), static_cast<float>(line.p2.y)), line.color)
    };
    m_window.draw(vertexLine, 2, sf::Lines);
}

void wf::gfx::Window::display() {
    m_window.display();
}

void wf::gfx::Window::close() {
    m_window.close();
}

bool wf::gfx::Window::isOpen() const noexcept {
    return m_window.isOpen();
}

sf::Vector2u wf::gfx::Window::getSize() const noexcept {
    return m_window.getSize();
}

void wf::gfx::Window::pollEvents(wf::core::EventManager &eventManager) {
    sf::Event event{};

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                eventManager.notify<wf::gfx::event::WindowClosed>({});
                break;
            case sf::Event::KeyPressed:
                eventManager.notify<wf::gfx::event::WindowKeyPressed>({ .key=event.key.code });
                break;
            default:
                break;
        }
    }
}
