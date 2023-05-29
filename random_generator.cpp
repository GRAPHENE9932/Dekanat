#include "random_generator.hpp"

#include <ctime>
#include <random>

std::mt19937_64 generator;

void seed_randomizer_with_time() {
    generator.seed(std::time(nullptr));
}

void generate_random_bytes(std::span<char> output_data) {
    for (auto& value : output_data) {
        value = generator();
    }
}