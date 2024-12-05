#pragma once
#include <vector>
#include <array>
#include "icVector.h"

struct color_choices {
	std::vector<icVector3> all_colors;
	icVector3 base_color = icVector3(1.0, 1.0, 1.0);
	int multi_hue_count;
	float log_lab = 0.0;
};

void single_hue_window(color_choices* colors);

void divergent_window(color_choices* colors);

void multi_hue_window(color_choices* colors);

void rainbow_window(color_choices* colors);

