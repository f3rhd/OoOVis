#include <Visualizer/DrawElement.h>
#include <Visualizer/Constants.h>
#include <Visualizer/Utils.h>
namespace OoOVisual
{
	namespace Visualizer
	{


		
			void Draw_Element::show_architectural() {
			ImDrawList* draw_list{ ImGui::GetWindowDrawList() };

			if (is_hovered())
				Utils::draw_glow_rec(_position, _dimension, Constants::BLACK);
			draw_list->AddRectFilled(
				_position, 
				ImVec2(_position.x + _dimension.x, _position.y + _dimension.y), 
				Constants::BLACK, 
				10.0f
			);
			draw_list->AddRect(
				_position,
				ImVec2(_position.x + _dimension.x, _position.y + _dimension.y),
				Constants::WHITE,
				10.0f
			);

			const char* text{};
			switch (_id) {
			case DRAW_ELEMENT_ID::FETCH_UNIT:
				text = "FETCH";
				break;
			case DRAW_ELEMENT_ID::FETCH_GROUP:
				text = "FETCH GROUP";
				break;
			case DRAW_ELEMENT_ID::DISPATCHER:
				text = "DISPATCHER";
				break;
			case DRAW_ELEMENT_ID::STATION_0:
				text = "STATION 0";
				break;
			case DRAW_ELEMENT_ID::STATION_1:
				text = "STATION 1";
				break;
			case DRAW_ELEMENT_ID::STATION_2:
				text = "STATION 2";
				break;
			case DRAW_ELEMENT_ID::STATION_3:
				text = "STATION 3";
				break; 
			case DRAW_ELEMENT_ID::STATION_4:
				text = "STATION 4";
				break;
			case DRAW_ELEMENT_ID::STATION_5:
				text = "STATION 5";
				break;
			case DRAW_ELEMENT_ID::STATION_6:
				text = "STATION 6";
				break;
			case DRAW_ELEMENT_ID::ADDER_SUBTRACTOR_UNIT:
				text = "ADDER-SUBTRACTOR";
				break;
			case DRAW_ELEMENT_ID::BITWISE_UNIT:
				text = "BITWISE";
				break;
			case DRAW_ELEMENT_ID::LESS_THAN_SETTER_UNIT:
				text = "LESS-THAN-SETTER";
				break;
			case DRAW_ELEMENT_ID::MULTIPLIER_UNIT:
				text = "MULTIPLIER";
				break;
			case DRAW_ELEMENT_ID::DIVIDER_UNIT:
				text = "DIVIDER";
				break;
			case DRAW_ELEMENT_ID::LOAD_STORE_UNIT:
				text = "LOAD-STORE";
				break;
			case DRAW_ELEMENT_ID::BRANCH_UNIT:
				text = "BRANCH";
				break;
			case DRAW_ELEMENT_ID::REGISTER_FILE:
				text = "PHYSICAL REGISTER FILE";
				break;
			case DRAW_ELEMENT_ID::REORDER_BUFFER:
				text = "REORDER BUFFER";
				break;
			default:
				break;

			}
			float fontSize(ImGui::GetFontSize() * Constants::FETCH_UNIT_LABEL_FONT_SCALE);
			
			ImVec2 textSize ( ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, text));

			ImVec2 boxCenter  { 
				_position.x + _dimension.x / 2.0f, 
				_position.y + _dimension.y / 2.0f 
			};
			ImVec2 textPos { 
				boxCenter.x - (textSize.x / 2.0f), 
				boxCenter.y - (textSize.y / 2.0f) 
			};

			draw_list->AddText(
				ImGui::GetFont(), 
				fontSize,
				textPos,
				Constants::WHITE, 
				text
			);
		}

		bool Draw_Element::is_hovered() {
			return ImGui::IsMouseHoveringRect(
				_position,
				{ _position.x + _dimension.x,_position.y + _dimension.y }
			);
		}

		bool Draw_Element::is_double_clicked() {
			return is_hovered() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) == 2;
		}
	}
}