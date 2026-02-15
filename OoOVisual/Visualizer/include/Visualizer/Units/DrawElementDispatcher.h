#pragma once
#include <Visualizer/Units/DrawElement.h>
namespace OoOVisual
{
	namespace Visualizer
	{
		struct Draw_Element_Dispatcher : Draw_Element {
			explicit Draw_Element_Dispatcher(DRAW_ELEMENT_ID id, ImVec2 position, ImVec2 dimension) : Draw_Element(id,position,dimension) {}
			void show_tooltip() override;
			// in hardware dispatcher itself is usually just combinational logic -- consisting of logic gates so we wont show it
			// what data it sends to other units etc. will be shown on arrows aka busses
			void show_detailed() override {};

		};

	}
}