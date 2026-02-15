#include <Visualizer/Units/DrawElement.h>
#include <Core/ReservationStation/ReservationStationEntry.h>
namespace OoOVisual
{
	namespace Visualizer
	{
		struct Draw_Element_Execution_Unit : Draw_Element {

			explicit Draw_Element_Execution_Unit(DRAW_ELEMENT_ID id, ImVec2 position, ImVec2 dimension)
				:Draw_Element(id, position, dimension) {}
			void show_tooltip() override;
			void show_detailed() override {};
		};
	}
}