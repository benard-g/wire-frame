#include <algorithm>
#include "wf/gfx/event/WindowKeyPressed.hpp"
#include "wf/space/Scene.hpp"

wf::space::Scene::Scene(
    wf::gfx::Window &window,
    wf::utils::Randomizer &randomizer,
    std::size_t batchSize,
    wf::core::Position const &moveSpeed,
    wf::core::Orientation const &rotateSpeed
) :
    m_window(window),
    m_randomizer(randomizer),
    m_moveSpeed(moveSpeed),
    m_rotateSpeed(rotateSpeed),
    m_sceneBoundaries(
        {
            .xMin=-(static_cast<double>(window.getSize().x) / 2) + 1,
            .xMax=(static_cast<double>(window.getSize().x) / 2),
            .yMin=-(static_cast<double>(window.getSize().y) / 2) + 1,
            .yMax=(static_cast<double>(window.getSize().y) / 2),
            .zMin=-(Z_MAX / 2) + 1,
            .zMax=(Z_MAX / 2),
        }
    ),
    m_shapesWithMutators(batchSize)
{}

double const wf::space::Scene::Z_MAX = 1'500;

sf::Color const wf::space::Scene::SCENE_BOUNDARIES_COLOR(0x111111FF);
double const wf::space::Scene::SCENE_BOUNDARIES_NB_LINES_WIDTH = 16;
double const wf::space::Scene::SCENE_BOUNDARIES_NB_LINES_HEIGHT = 9;
double const wf::space::Scene::SCENE_BOUNDARIES_NB_LINES_DEPTH = 19;

void wf::space::Scene::addShape(
    std::unique_ptr<wf::space::shape::IShape> &&shape,
    wf::core::Position const &position,
    wf::core::Orientation const &orientation
) {
    m_shapesWithMutators.addItem(
        {
            .shape=std::move(shape),
            .mutator=wf::space::Mutator(position, orientation),
            .isMoving=false,
            .moveSpeed=wf::core::Position{.x=0, .y=0, .z=0},
            .rotateSpeed=wf::core::Orientation{.x=0, .y=0, .z=0},
        }
    );
}

namespace {
    inline double randomBound(wf::utils::Randomizer &randomizer, double value) {
        return randomizer.random(0, 1) == 0 ? value : -value;
    }
}

void wf::space::Scene::registerEvents(wf::core::EventManager &eventManager) {
    eventManager.subscribe<wf::gfx::event::WindowKeyPressed>(
        [this](auto const &event)
        {
            if (event.key == sf::Keyboard::Space) {
                m_shapesWithMutators.iterItems(
                    [this](auto &shapeWithMutator)
                    {
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
                    }
                );
            }
        }
    );
}

inline wf::core::Position
wf::space::Scene::transposePositionInWindow(wf::core::Position const &position) const noexcept {
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

inline void wf::space::Scene::applyMovements(wf::space::Scene::ShapeWithMutator &shapeWithMutator) {
    shapeWithMutator.mutator.move(shapeWithMutator.moveSpeed);
    shapeWithMutator.mutator.rotate(shapeWithMutator.rotateSpeed);

    // Make the shape change direction when it reaches a wall
    auto &position = shapeWithMutator.mutator.getPosition();

    if (position.x < m_sceneBoundaries.xMin) {
        shapeWithMutator.moveSpeed.x = m_moveSpeed.x;
    } else if (position.x > m_sceneBoundaries.xMax) {
        shapeWithMutator.moveSpeed.x = -m_moveSpeed.x;
    }

    if (position.y < m_sceneBoundaries.yMin) {
        shapeWithMutator.moveSpeed.y = m_moveSpeed.y;
    } else if (position.y > m_sceneBoundaries.yMax) {
        shapeWithMutator.moveSpeed.y = -m_moveSpeed.y;
    }

    if (position.z < m_sceneBoundaries.zMin) {
        shapeWithMutator.moveSpeed.z = m_moveSpeed.z;
    } else if (position.z > m_sceneBoundaries.zMax) {
        shapeWithMutator.moveSpeed.z = -m_moveSpeed.z;
    }
}

inline wf::space::Scene::InputMutations wf::space::Scene::getInputMutations() const noexcept {
    double xPos = 0;
    double yPos = 0;
    double zPos = 0;

    double xRot = 0;
    double yRot = 0;
    double zRot = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        xPos -= m_moveSpeed.x;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        xPos += m_moveSpeed.x;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        yPos -= m_moveSpeed.y;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        yPos += m_moveSpeed.y;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Home)) {
        zPos -= m_moveSpeed.z;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::End)) {
        zPos += m_moveSpeed.z;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        xRot -= m_rotateSpeed.x;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        xRot += m_rotateSpeed.x;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        yRot -= m_rotateSpeed.y;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        yRot += m_rotateSpeed.y;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        zRot += m_rotateSpeed.z;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        zRot -= m_rotateSpeed.z;
    }

    return {
        .position={
            .x=xPos,
            .y=yPos,
            .z=zPos,
        },
        .orientation={
            .x=xRot,
            .y=yRot,
            .z=zRot,
        },
    };
}

inline void wf::space::Scene::applyInputMutations(
    wf::space::Scene::InputMutations const &inputMutations,
    wf::space::Scene::ShapeWithMutator &shapeWithMutator
) const noexcept {
    shapeWithMutator.mutator.move(inputMutations.position);
    shapeWithMutator.mutator.rotate(inputMutations.orientation);
}

void wf::space::Scene::update() {
    InputMutations const inputMutations = getInputMutations();

    m_shapesWithMutators.resetComputed();
    m_shapesWithMutators.iterItemsParallel(
        [this, &inputMutations](auto &shapeWithMutator, auto lineInserter)
        {
            if (shapeWithMutator.isMoving) {
                applyMovements(shapeWithMutator);
            }

            applyInputMutations(inputMutations, shapeWithMutator);

            shapeWithMutator.shape->generateLines(
                [this, &shapeWithMutator, &lineInserter](wf::core::Line const &line)
                {
                    lineInserter = wf::core::Line{
                        .p1=transposePositionInWindow(shapeWithMutator.mutator.mutate(line.p1)),
                        .p2=transposePositionInWindow(shapeWithMutator.mutator.mutate(line.p2)),
                        .color=line.color
                    };
                }
            );
        }
    );
}

void wf::space::Scene::render() {
    drawSceneBoundaries();
    m_shapesWithMutators.iterComputed(
        [this](auto const &line)
        {
            m_window.drawLine(line);
        }
    );
}

void wf::space::Scene::drawSceneBoundaries() {
    double const stepWidth = (m_sceneBoundaries.xMax - m_sceneBoundaries.xMin) / SCENE_BOUNDARIES_NB_LINES_WIDTH;
    double const stepHeight = (m_sceneBoundaries.yMax - m_sceneBoundaries.yMin) / SCENE_BOUNDARIES_NB_LINES_HEIGHT;
    double const stepDepth = (m_sceneBoundaries.zMax - m_sceneBoundaries.zMin) / SCENE_BOUNDARIES_NB_LINES_DEPTH;

    for (double i = m_sceneBoundaries.xMin; i < m_sceneBoundaries.xMax; i += stepWidth) {
        auto topFront = transposePositionInWindow({.x=i, .y=m_sceneBoundaries.yMin, .z=m_sceneBoundaries.zMin});
        auto bottomFront = transposePositionInWindow({.x=i, .y=m_sceneBoundaries.yMax, .z=m_sceneBoundaries.zMin});
        auto topBack = transposePositionInWindow({.x=i, .y=m_sceneBoundaries.yMin, .z=m_sceneBoundaries.zMax});
        auto bottomBack = transposePositionInWindow({.x=i, .y=m_sceneBoundaries.yMax, .z=m_sceneBoundaries.zMax});
        m_window.drawLine({.p1=topFront, .p2=topBack, .color=SCENE_BOUNDARIES_COLOR});
        m_window.drawLine({.p1=topBack, .p2=bottomBack, .color=SCENE_BOUNDARIES_COLOR});
        m_window.drawLine({.p1=bottomBack, .p2=bottomFront, .color=SCENE_BOUNDARIES_COLOR});
    }

    for (double i = m_sceneBoundaries.yMin; i <= m_sceneBoundaries.yMax; i += stepHeight) {
        auto leftFront = transposePositionInWindow({.x=m_sceneBoundaries.xMin, .y=i, .z=m_sceneBoundaries.zMin});
        auto leftBack = transposePositionInWindow({.x=m_sceneBoundaries.xMin, .y=i, .z=m_sceneBoundaries.zMax});
        auto rightFront = transposePositionInWindow({.x=m_sceneBoundaries.xMax, .y=i, .z=m_sceneBoundaries.zMin});
        auto rightBack = transposePositionInWindow({.x=m_sceneBoundaries.xMax, .y=i, .z=m_sceneBoundaries.zMax});
        m_window.drawLine({.p1=leftFront, .p2=leftBack, .color=SCENE_BOUNDARIES_COLOR});
        m_window.drawLine({.p1=leftBack, .p2=rightBack, .color=SCENE_BOUNDARIES_COLOR});
        m_window.drawLine({.p1=rightBack, .p2=rightFront, .color=SCENE_BOUNDARIES_COLOR});
    }

    for (double i = m_sceneBoundaries.zMax; i > m_sceneBoundaries.zMin; i -= stepDepth) {
        auto topLeft = transposePositionInWindow({.x=m_sceneBoundaries.xMin, .y=m_sceneBoundaries.yMin, .z=i});
        auto bottomLeft = transposePositionInWindow({.x=m_sceneBoundaries.xMin, .y=m_sceneBoundaries.yMax, .z=i});
        auto topRight = transposePositionInWindow({.x=m_sceneBoundaries.xMax, .y=m_sceneBoundaries.yMin, .z=i});
        auto bottomRight = transposePositionInWindow({.x=m_sceneBoundaries.xMax, .y=m_sceneBoundaries.yMax, .z=i});
        m_window.drawLine({.p1=topLeft, .p2=topRight, .color=SCENE_BOUNDARIES_COLOR});
        m_window.drawLine({.p1=topRight, .p2=bottomRight, .color=SCENE_BOUNDARIES_COLOR});
        m_window.drawLine({.p1=bottomRight, .p2=bottomLeft, .color=SCENE_BOUNDARIES_COLOR});
        m_window.drawLine({.p1=bottomLeft, .p2=topLeft, .color=SCENE_BOUNDARIES_COLOR});
    }
}
