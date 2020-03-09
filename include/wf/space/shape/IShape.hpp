#pragma once

#include <cstdint>
#include <vector>

#include "wf/core/Line.hpp"

namespace wf::space::shape {
    class IShape {
    public:
        /*!
         * Make the Shape generate its lines and insert them in the write_iterator.
         *
         * @param lineInserter
         */
        virtual void generateLines(std::back_insert_iterator<std::vector<wf::core::Line>> lineInserter) = 0;

    public:
        virtual ~IShape() noexcept = default;
    };
}
