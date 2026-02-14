#pragma once
#include <Visualizer/DrawElement.h>
namespace OoOVisual
{
	namespace Visualizer
	{
		struct Draw_Element_Fetch_Unit : Draw_Element {
			explicit Draw_Element_Fetch_Unit(DRAW_ELEMENT_ID id,ImVec2 position, ImVec2 dimension) : Draw_Element(id,position,dimension) {}
			void show_tooltip() override;
			void show_detailed() override;
		};
	}
}