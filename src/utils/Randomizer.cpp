#include "wf/utils/Randomizer.hpp"

wf::utils::Randomizer::Randomizer(uint32_t seed):
    generator(seed)
{}

int32_t wf::utils::Randomizer::random(int32_t minBound, int32_t maxBound) {
    return std::uniform_int_distribution<int>(minBound, maxBound)(generator);
}

uint32_t wf::utils::Randomizer::randomSeed() {
    return std::random_device{}();
}
