#include <Visualizer/Units/DrawElement.h>

namespace OoOVisual
{
	namespace Visualizer
	{
		struct Draw_Element_Fetch_Group : Draw_Element {
			explicit Draw_Element_Fetch_Group(DRAW_ELEMENT_ID id, ImVec2 position, ImVec2 dimension) : Draw_Element(id,position,dimension) {}
			void show_tooltip() override;
			void show_detailed() override {};
		};
	}
}
