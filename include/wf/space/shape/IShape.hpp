#pragma once

#include <functional>

#include "wf/core/Line.hpp"

namespace wf::space::shape {
    class IShape {
    public:
        using LineInserter = std::function<void (wf::core::Line const &line)>;

        /*!
         * Make the Shape generate its lines.
         *
         * @param insertLine
         */
        virtual void generateLines(LineInserter const &insertLine) = 0;

    public:
        virtual ~IShape() noexcept = default;
    };
}
