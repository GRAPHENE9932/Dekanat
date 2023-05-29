#pragma once

#include <span>

void seed_randomizer_with_time();
void generate_random_bytes(std::span<char> output_data);