#include <cmath>
#include "wf/space/Mutator.hpp"

wf::space::Mutator::Mutator(wf::core::Position const &position, wf::core::Orientation const &orientation):
    m_position(position),
    m_orientation(orientation),
    m_rotationMatrix()
{
    updateRotationMatrix();
}

inline wf::core::Position wf::space::Mutator::applyTranslation(wf::core::Position const &position) const noexcept {
    return {
        .x=position.x + m_position.x,
        .y=position.y + m_position.y,
        .z=position.z + m_position.z,
    };
}

inline wf::core::Position wf::space::Mutator::applyRotation(wf::core::Position const &pos) const noexcept {
    return {
        .x=pos.x * m_rotationMatrix[0][0] + pos.y * m_rotationMatrix[0][1] + pos.z * m_rotationMatrix[0][2],
        .y=pos.x * m_rotationMatrix[1][0] + pos.y * m_rotationMatrix[1][1] + pos.z * m_rotationMatrix[1][2],
        .z=pos.x * m_rotationMatrix[2][0] + pos.y * m_rotationMatrix[2][1] + pos.z * m_rotationMatrix[2][2],
    };
}

wf::core::Position wf::space::Mutator::mutate(wf::core::Position const &position) const {
    return applyTranslation(applyRotation(position));
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
    updateRotationMatrix();
}

void wf::space::Mutator::updateRotationMatrix() {
    double cosX = std::cos(m_orientation.x);
    double cosY = std::cos(m_orientation.y);
    double cosZ = std::cos(m_orientation.z);
    double sinX = std::sin(m_orientation.x);
    double sinY = std::sin(m_orientation.y);
    double sinZ = std::sin(m_orientation.z);

    m_rotationMatrix[0][0] = cosY * cosZ;
    m_rotationMatrix[0][1] = -(cosY * sinZ);
    m_rotationMatrix[0][2] = sinY;

    m_rotationMatrix[1][0] = sinX * sinY * cosZ + cosX * sinZ;
    m_rotationMatrix[1][1] = cosX * cosZ - sinX * sinY * sinZ;
    m_rotationMatrix[1][2] = -(sinX * cosY);

    m_rotationMatrix[2][0] = sinX * sinZ - cosX * sinY * cosZ;
    m_rotationMatrix[2][1] = cosX * sinY * sinZ + sinX * cosZ;
    m_rotationMatrix[2][2] = cosX * cosY;
}
