#pragma once

#include "wf/gfx/Window.hpp"
#include "wf/space/shape/IShape.hpp"
#include "wf/space/Mutator.hpp"
#include "wf/utils/DistributedContainer.hpp"
#include "wf/utils/Randomizer.hpp"

namespace wf::space {
    class Scene {
    public:
        Scene(
            wf::gfx::Window &window,
            wf::utils::Randomizer &randomizer,
            std::size_t batchSize,
            wf::core::Position const &moveSpeed,
            wf::core::Orientation const &rotateSpeed
        );

        void addShape(std::unique_ptr<wf::space::shape::IShape> &&shape, Mutator &&mutator);

        void update();

        void render();

        void registerEvents(wf::core::EventManager &eventManager);

    public:
        // Coplien
        Scene() = delete;
        ~Scene() noexcept = default;
        Scene(Scene const &) = delete;
        Scene &operator=(Scene const &) = delete;
        Scene(Scene &&) noexcept = delete;
        Scene &operator=(Scene &&) noexcept = delete;

    private:
        wf::gfx::Window &m_window;
        wf::utils::Randomizer &m_randomizer;

        wf::core::Position m_moveSpeed;
        wf::core::Orientation m_rotateSpeed;

    private:
        struct SceneBoundaries {
            double xMin;
            double xMax;
            double yMin;
            double yMax;
            double zMin;
            double zMax;
        };

        SceneBoundaries m_sceneBoundaries;

        static double const Z_MAX;

        [[nodiscard]] wf::core::Position transposePositionInWindow(wf::core::Position const &position) const noexcept;

    private:
        struct ShapeWithMutator {
            std::unique_ptr<wf::space::shape::IShape> shape;
            wf::space::Mutator mutator;
            bool isMoving;
            wf::core::Position moveSpeed;
            wf::core::Orientation rotateSpeed;
        };

        void applyMovements(ShapeWithMutator &shapeWithMutator);

        wf::utils::DistributedContainer<ShapeWithMutator, wf::core::Line> m_shapesWithMutators;

    private:
        struct InputMutations {
            wf::core::Position position;
            wf::core::Orientation orientation;
        };

        [[nodiscard]] InputMutations getInputMutations() const noexcept;
        void applyInputMutations(InputMutations const &inputMutations, ShapeWithMutator &shapeWithMutator) const noexcept;
    };
}
