#pragma once
#include <Visualizer/Units/DrawElement.h>
namespace OoOVisual
{
	namespace Visualizer
	{

		struct Draw_Element_Register_Manager : Draw_Element {
			explicit Draw_Element_Register_Manager(DRAW_ELEMENT_ID id,ImVec2 position, ImVec2 dimension) : Draw_Element(id,position,dimension) {}
			void show_tooltip() override;
			void show_detailed() override;

		};
	}
}