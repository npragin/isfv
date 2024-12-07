#include "ui_window.h"
#include "imgui.h"
#include <iostream>

static Colors savedColors;

void create_and_push_color(color_choices* colors, int n)
{	
	for (int i = 0; i < n; i++)
	{
		// Ensure a color exists in the struct to be modified with the color editor
		if(i >= colors->all_colors.size())
			colors->all_colors.push_back(colors->base_color);
		
		// Determine label of color editor
		static char label[10];
		if (i == 0)
			sprintf(label, "Max");
		else if (i == n - 1)
			sprintf(label, "Min");
		else
			sprintf(label, "Color %d", i);
		
		// Create color editor
		ImGui::PushID(i);
		float* color = new float[3]{(float)colors->all_colors[i].x, (float)colors->all_colors[i].y, (float)colors->all_colors[i].z};
		ImGui::ColorEdit3(label, color);
		colors->all_colors[i].x = color[0];
        colors->all_colors[i].y = color[1];
        colors->all_colors[i].z = color[2];
		ImGui::PopID();
	}

	for (int i = 0; i < colors->all_colors.size() - n; i++)
		colors->all_colors.pop_back();

	if (colors->map_type == SINGLE)
		savedColors.singleHue = colors->all_colors;
	else if (colors->map_type == DIVERGENT)
		savedColors.divergent = colors->all_colors;
	else if (colors->map_type == MULTI)
		savedColors.multiHue = colors->all_colors;
}

void single_hue_window(color_choices* colors)
{
	colors->map_type = SINGLE;
	colors->all_colors = savedColors.singleHue;
	create_and_push_color(colors, 1);
}

void divergent_window(color_choices* colors)
{
	colors->map_type = DIVERGENT;
	colors->all_colors = savedColors.divergent;
	create_and_push_color(colors, 2);
}

void multi_hue_window(color_choices* colors)
{
	colors->map_type = MULTI;
	colors->all_colors = savedColors.multiHue;
	
	if (ImGui::Button("Add Hue") && colors->multi_hue_count < 9) // Buttons return true when clicked (most widgets return true when edited/activated)
		colors->multi_hue_count++;
	ImGui::SameLine();
	if (ImGui::Button("Remove Hue") && colors->multi_hue_count > 2)                            
		colors->multi_hue_count--;

	create_and_push_color(colors, colors->multi_hue_count);
}

void rainbow_window(color_choices* colors)
{
	const char* items[] = { "RGB Rainbow", "Jet", "Turbo" };
	static int current_index = 0;

	if (ImGui::BeginCombo("Options", items[current_index])) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_index == n); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(items[n], is_selected))
			{
				//std::cout << n << std::endl;
				current_index = n;
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
		}
		ImGui::EndCombo();
	}
}
