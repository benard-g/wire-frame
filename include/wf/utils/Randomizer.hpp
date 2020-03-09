#pragma once

#include <cstdint>
#include <random>

namespace wf::utils {
    class Randomizer {
    public:
        static uint32_t randomSeed();

    public:
        explicit Randomizer(uint32_t seed = Randomizer::randomSeed());

        int32_t random(int32_t minBound, int32_t maxBound);

    public:
        // Coplien
        ~Randomizer() noexcept = default;
        Randomizer(Randomizer const &) = delete;
        Randomizer &operator=(Randomizer const &) = delete;
        Randomizer(Randomizer &&) noexcept = delete;
        Randomizer &operator=(Randomizer &&) noexcept = delete;

    private:
        std::mt19937 generator;
    };
}
