#pragma once

#include "wf/core/Position.hpp"
#include "wf/core/Orientation.hpp"

namespace wf::space {
    class Mutator {
    public:
        Mutator(wf::core::Position const &position, wf::core::Orientation const &orientation);

        [[nodiscard]] wf::core::Position mutate(wf::core::Position const &position) const;

        void move(wf::core::Position const &positionOffset);

        void rotate(wf::core::Orientation const &orientationOffset);

        [[nodiscard]] wf::core::Position const &getPosition() const noexcept { return m_position; }

    public:
        // Coplien
        Mutator() = delete;
        ~Mutator() noexcept = default;
        Mutator(Mutator const &) = delete;
        Mutator &operator=(Mutator const &) = delete;
        Mutator(Mutator &&) noexcept = default;
        Mutator &operator=(Mutator &&) noexcept = default;

    private:
        wf::core::Position m_position;
        wf::core::Orientation m_orientation;
    };
}
