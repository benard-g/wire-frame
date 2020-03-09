#include "wf/space/Mutator.hpp"

namespace {
    inline wf::core::Position applyRotation(wf::core::Position const &position, wf::core::Orientation const &orientationOffset) {
        // TODO
        return position;
    }

    inline wf::core::Position applyTranslation(wf::core::Position const &position, wf::core::Position const &positionOffset) {
        return {
            .x=position.x + positionOffset.x,
            .y=position.y + positionOffset.y,
            .z=position.z + positionOffset.z,
        };
    }
}

wf::space::Mutator::Mutator(wf::core::Position const &position, wf::core::Orientation const &orientation):
    m_position(position),
    m_orientation(orientation)
{}

wf::core::Position wf::space::Mutator::mutate(wf::core::Position const &position) const {
    return ::applyTranslation(::applyRotation(position, m_orientation), m_position);
}

void wf::space::Mutator::move(wf::core::Position const &positionOffset) {
    m_position.x += positionOffset.x;
    m_position.y += positionOffset.y;
    m_position.z += positionOffset.z;
}

void wf::space::Mutator::rotate(const wf::core::Orientation &orientationOffset) {
    m_orientation.x += orientationOffset.x;
    m_orientation.y += orientationOffset.y;
    m_orientation.z += orientationOffset.z;
}
