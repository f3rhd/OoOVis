#include <Visualizer/Units/DrawElementExecutionUnit.h>
#include <Visualizer/Utils.h>
#include <Core/Execution/ExecutionStage.h>
namespace OoOVisual
{
	namespace Visualizer
	{

		void Draw_Element_Execution_Unit::show_tooltip() {
			const Core::Reservation_Station_Entry* issued_entry{};
			switch (_id)
			{
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::ADDER_SUBTRACTOR_UNIT:
				issued_entry = Core::Execution_Stage::issued()[0];
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::BITWISE_UNIT:
				issued_entry = Core::Execution_Stage::issued()[1];
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::LESS_THAN_SETTER_UNIT:
				issued_entry = Core::Execution_Stage::issued()[2];
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::MULTIPLIER_UNIT:
				issued_entry = Core::Execution_Stage::issued()[3];
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::DIVIDER_UNIT:
				issued_entry = Core::Execution_Stage::issued()[4];
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::LOAD_STORE_UNIT:
				issued_entry = Core::Execution_Stage::issued()[5];
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::BRANCH_UNIT:
				issued_entry = Core::Execution_Stage::issued()[6];
				break;
			default:
				break;
			}
			ImGui::BeginTooltip();
			
			bool is_busy{ issued_entry != nullptr };

			ImGui::Text("Execution Unit Status");
			ImGui::Separator();

			if (ImGui::BeginTable("##ExecUnitTable", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg)) {
				
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Status:");
				ImGui::TableSetColumnIndex(1);
				if (is_busy) {
					ImGui::TextColored(ImVec4{ 1.0f, 0.4f, 0.4f, 1.0f }, "BUSY");
				} else {
					ImGui::TextColored(ImVec4{ 0.4f, 1.0f, 0.4f, 1.0f }, "IDLE");
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Operation:");
				ImGui::TableSetColumnIndex(1);
				if (is_busy) {
					ImGui::Text("%s", Utils::execution_mode_to_str(issued_entry->mode));
				} else {
					ImGui::TextDisabled("N/A");
				}

				if (is_busy) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Fetched at:");
					ImGui::TableSetColumnIndex(1);
					ImGui::TextColored(ImVec4{ 0.7f, 0.9f, 1.0f, 1.0f }, "%llu", issued_entry->timestamp);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("ROB Index:");
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("#%llu", issued_entry->reorder_buffer_entry_index);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("PC:");
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("0x%08X", issued_entry->instruction_address);
				}

				ImGui::EndTable();
			}
			
			ImGui::EndTooltip();
		}
	}
}