#include "wf/space/shape/Cube.hpp"

wf::space::shape::Cube::Cube(double size, sf::Color color):
    m_size(size),
    m_color(color)
{}

void wf::space::shape::Cube::generateLines(LineInserter const &insertLine) {
    double offset = m_size / 2;

    // Points
    wf::core::Position front1{ .x=-offset, .y=-offset, .z=-offset };
    wf::core::Position front2{ .x= offset, .y=-offset, .z=-offset };
    wf::core::Position front3{ .x= offset, .y= offset, .z=-offset };
    wf::core::Position front4{ .x=-offset, .y= offset, .z=-offset };

    wf::core::Position back1{ .x=-offset, .y=-offset, .z= offset };
    wf::core::Position back2{ .x= offset, .y=-offset, .z= offset };
    wf::core::Position back3{ .x= offset, .y= offset, .z= offset };
    wf::core::Position back4{ .x=-offset, .y= offset, .z= offset };

    // Front square
    insertLine(wf::core::Line{ .p1=front1, .p2=front2, .color=m_color });
    insertLine(wf::core::Line{ .p1=front2, .p2=front3, .color=m_color });
    insertLine(wf::core::Line{ .p1=front3, .p2=front4, .color=m_color });
    insertLine(wf::core::Line{ .p1=front4, .p2=front1, .color=m_color });

    // Back square
    insertLine(wf::core::Line{ .p1=back1, .p2=back2, .color=m_color });
    insertLine(wf::core::Line{ .p1=back2, .p2=back3, .color=m_color });
    insertLine(wf::core::Line{ .p1=back3, .p2=back4, .color=m_color });
    insertLine(wf::core::Line{ .p1=back4, .p2=back1, .color=m_color });

    // Link front and back
    insertLine(wf::core::Line{ .p1=front1, .p2=back1, .color=m_color });
    insertLine(wf::core::Line{ .p1=front2, .p2=back2, .color=m_color });
    insertLine(wf::core::Line{ .p1=front3, .p2=back3, .color=m_color });
    insertLine(wf::core::Line{ .p1=front4, .p2=back4, .color=m_color });
}
