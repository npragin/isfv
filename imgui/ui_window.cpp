#include "ui_window.h"
#include "imgui.h"

#include<iostream>


void create_and_push_color(color_choices* colors, int n)
{	
	for (int i = 0; i < n; i++)
	{
		if(i >= colors->all_colors.size())
			colors->all_colors.push_back(colors->base_color);
		ImGui::PushID(i);
		float* color = colors->all_colors[i].data();
		ImGui::ColorEdit3("Color", color);
		ImGui::PopID();
	}
}

void single_hue_window(color_choices* colors)
{
	create_and_push_color(colors, 1);
}

void divergent_window(color_choices* colors)
{
	create_and_push_color(colors, 2);
}

void multi_hue_window(color_choices* colors)
{
	if (colors->multi_hue_count == 0) {  // Initialize if not set
        colors->multi_hue_count = 2;
    }
	
	if (ImGui::Button("Add Hue") && colors->multi_hue_count < 9) // Buttons return true when clicked (most widgets return true when edited/activated)
		colors->multi_hue_count++;
	ImGui::SameLine();
	if (ImGui::Button("Remove Hue") && colors->multi_hue_count > 2)                            
		colors->multi_hue_count--;

	create_and_push_color(colors, colors->multi_hue_count);
}

void rainbow_window(color_choices* colors)
{
	const char* items[] = { "Turbo", "Jet", "RGB Rainbow" };
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
