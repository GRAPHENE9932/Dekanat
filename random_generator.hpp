#pragma once

#include <span>

void seed_rendomizer_with_time();
void generate_random_bytes(std::span<char> output_data);