#include <Visualizer/DrawElementFetchUnit.h>
#include <Visualizer/Constants.h>
#include <Core/Fetch/Fetch.h>
#include <iostream>
namespace OoOVisual
{
	namespace Visualizer
	{


		void Draw_Element_Fetch_Unit::show_detailed() { // @ToDo

		}

		void Draw_Element_Fetch_Unit::show_tooltip() {
			auto current_pc{ Core::Fetch_Unit::get_program_counter() };
			ImGui::SetTooltip(
				"Fetch Unit\nPc: %u"
				,current_pc
			);
		}
	}
}

