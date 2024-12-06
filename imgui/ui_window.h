#pragma once
#include <vector>
#include <array>
#include "../learnply/icVector.h"

struct color_choices {
	std::vector<icVector3> all_colors;
	icVector3 base_color = icVector3(1.0, 1.0, 1.0);
	int multi_hue_count;
	float log_lab = 0.0;
};

struct DefaultColors {
    std::vector<icVector3> singleHue = {icVector3(0.0, 0.0, 1.0)};
    std::vector<icVector3> divergent = {
        icVector3(0.1765, 0.0, 0.294),
        icVector3(0.498, 0.231, 0.0314)
    };
    std::vector<icVector3> multiHue = {
        icVector3(0.94, 0.98, 0.13),
        icVector3(0.87, 0.11, 0.32),
        icVector3(0.38, 0.0, 0.851),
        icVector3(0.05, 0.03, 0.53)
    };
};

void single_hue_window(color_choices* colors);

void divergent_window(color_choices* colors);

void multi_hue_window(color_choices* colors);

void rainbow_window(color_choices* colors);

