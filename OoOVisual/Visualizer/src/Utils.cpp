#include <Visualizer/Utils.h>

namespace OoOVisual
{
	namespace Visualizer
	{
		namespace Utils
		{
			void draw_glow_rec(ImVec2 pos, ImVec2 size, ImU32 col) {
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				
				for (int i = 1; i <= 5; i++) {
					float padding{ (float)i * 2.0f } ;
					
					ImU32 alpha_col{ (col & 0x00FFFFFF) | (0x22000000 / i) };

					draw_list->AddRectFilled(
						ImVec2(pos.x - padding, pos.y - padding),
						ImVec2(pos.x + size.x + padding, pos.y + size.y + padding),
						alpha_col,
						10.0f + padding 
					);
				}
			}

			void draw_glow_line(ImVec2 p1, ImVec2 p2, ImU32 col) {
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				draw_list->AddLine(p1, p2, (col & 0x00FFFFFF) | 0x44000000, 8.0f);
				
				draw_list->AddLine(p1, p2, col, 2.0f);

			}

			const char* flowtype_to_str(FrontEnd::FLOW_TYPE type) {
				switch (type)
				{
				case OoOVisual::FrontEnd::FLOW_TYPE::REGISTER:
					return "REGISTER";
				case OoOVisual::FrontEnd::FLOW_TYPE::LOAD:
					return "LOAD";
					break;
				case OoOVisual::FrontEnd::FLOW_TYPE::STORE:
					return "STORE";
					break;
				case OoOVisual::FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL:
					return "BRANCH";
				case OoOVisual::FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL:
					return "JUMP";
				case OoOVisual::FrontEnd::FLOW_TYPE::UNKNOWN:
				default:
					return "UNKNOWN";
				}
			}

		}
	}
}