#include <algorithm>
#include "wf/gfx/event/WindowKeyPressed.hpp"
#include "wf/space/Scene.hpp"

wf::space::Scene::Scene(
    wf::gfx::Window &window,
    wf::utils::Randomizer &randomizer,
    wf::core::Position const &moveSpeed,
    wf::core::Orientation const &rotateSpeed
):
    m_window(window),
    m_randomizer(randomizer),
    m_moveSpeed(moveSpeed),
    m_rotateSpeed(rotateSpeed),
    m_sceneBoundaries({
        .xMin=-(static_cast<double>(window.getSize().x) / 2) + 1,
        .xMax=(static_cast<double>(window.getSize().x) / 2),
        .yMin=-(static_cast<double>(window.getSize().y) / 2) + 1,
        .yMax=(static_cast<double>(window.getSize().y) / 2),
        .zMin=-(Z_MAX / 2) + 1,
        .zMax=(Z_MAX / 2),
    }),
    m_shapesWithMutators()
{}

double const wf::space::Scene::Z_MAX = 2'500;

void wf::space::Scene::addShape(std::unique_ptr<wf::space::shape::IShape> &&shape, Mutator &&mutator) {
    m_shapesWithMutators.addItem(ShapeWithMutator{
        .shape=std::move(shape),
        .mutator=std::move(mutator),
        .isMoving=false,
        .moveSpeed=wf::core::Position{ .x=0, .y=0, .z=0 },
        .rotateSpeed=wf::core::Orientation{ .x=0, .y=0, .z=0 },
    });
}

namespace {
    inline double randomBound(wf::utils::Randomizer &randomizer, double value) {
        return randomizer.random(0, 1) == 0 ? value : -value;
    }
}

void wf::space::Scene::registerEvents(wf::core::EventManager &eventManager) {
    eventManager.subscribe<wf::gfx::event::WindowKeyPressed>([this](auto const &event) {
        if (event.key == sf::Keyboard::Space) {
            m_shapesWithMutators.iterItems([this](auto &shapeWithMutator) {
                shapeWithMutator.isMoving = !shapeWithMutator.isMoving;
                if (shapeWithMutator.isMoving) {
                    shapeWithMutator.moveSpeed = {
                        .x=::randomBound(m_randomizer, m_moveSpeed.x),
                        .y=::randomBound(m_randomizer, m_moveSpeed.y),
                        .z=::randomBound(m_randomizer, m_moveSpeed.z),
                    };
                    shapeWithMutator.rotateSpeed = {
                        .x=::randomBound(m_randomizer, m_rotateSpeed.x),
                        .y=::randomBound(m_randomizer, m_rotateSpeed.y),
                        .z=::randomBound(m_randomizer, m_rotateSpeed.z),
                    };
                }
            });
        }
    });
}

inline wf::core::Position wf::space::Scene::transposePositionInWindow(wf::core::Position const &position) const noexcept {
    double x = std::max(m_sceneBoundaries.xMin, std::min(m_sceneBoundaries.xMax, position.x));
    double y = std::max(m_sceneBoundaries.yMin, std::min(m_sceneBoundaries.yMax, position.y));
    double z = std::max(m_sceneBoundaries.zMin, std::min(m_sceneBoundaries.zMax, position.z)) + m_sceneBoundaries.zMax;

    double zRatio = z / Z_MAX;
    double zCorrection = zRatio - 0.25f * zRatio * zRatio;
    x -= x * zCorrection;
    y -= y * zCorrection;

    return wf::core::Position{
        .x=x + m_sceneBoundaries.xMax,
        .y=y + m_sceneBoundaries.yMax,
        .z=z
    };
}

void wf::space::Scene::update() {
    m_shapesWithMutators.resetCache();
    m_shapesWithMutators.iterItemsParallel([this](auto &shapeWithMutator, auto lineInserter) {
        if (shapeWithMutator.isMoving) {
            shapeWithMutator.mutator.move(shapeWithMutator.moveSpeed);
            shapeWithMutator.mutator.rotate(shapeWithMutator.rotateSpeed);

            // Make the shape change direction when it reaches a wall
            auto &position = shapeWithMutator.mutator.getPosition();
            if (position.x < m_sceneBoundaries.xMin) {
                shapeWithMutator.moveSpeed.x = m_moveSpeed.x;
            }
            else if (position.x > m_sceneBoundaries.xMax) {
                shapeWithMutator.moveSpeed.x = -m_moveSpeed.x;
            }
            if (position.y < m_sceneBoundaries.yMin) {
                shapeWithMutator.moveSpeed.y = m_moveSpeed.y;
            }
            else if (position.y > m_sceneBoundaries.yMax) {
                shapeWithMutator.moveSpeed.y = -m_moveSpeed.y;
            }
            if (position.z < m_sceneBoundaries.zMin) {
                shapeWithMutator.moveSpeed.z = m_moveSpeed.z;
            }
            else if (position.z > m_sceneBoundaries.zMax) {
                shapeWithMutator.moveSpeed.z = -m_moveSpeed.z;
            }
        }

        shapeWithMutator.shape->generateLines([this, &shapeWithMutator, &lineInserter](wf::core::Line const &line) {
            lineInserter = wf::core::Line{
                .p1=transposePositionInWindow(shapeWithMutator.mutator.mutate(line.p1)),
                .p2=transposePositionInWindow(shapeWithMutator.mutator.mutate(line.p2)),
                .color=line.color
            };
        });
    });
}

void wf::space::Scene::render() {
    m_shapesWithMutators.iterCached([this](auto const &line) {
        m_window.drawLine(line);
    });
}
