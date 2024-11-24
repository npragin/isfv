#pragma once
#include <vector>
#include <array>

struct color_choices {
	std::vector<std::array<float, 3>> all_colors;
	std::array<float, 3> base_color = { 1.0, 1.0, 1.0 };
	float log_lab = 0.0;
};

void single_hue_window(color_choices* colors);

void divergent_window(color_choices* colors);

void multi_hue_window(color_choices* colors);

void rainbow_window(color_choices* colors);

