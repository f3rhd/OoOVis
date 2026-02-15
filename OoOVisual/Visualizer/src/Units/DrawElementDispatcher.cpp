#include <Visualizer/Units/DrawElementDispatcher.h>
#include <Visualizer/Constants.h>
#include <Core/Dispatch/Dispatcher.h>
#include <Core/Constants/Constants.h>
namespace OoOVisual
{
	namespace Visualizer
	{
		void Draw_Element_Dispatcher::show_tooltip() {
			ImGui::BeginTooltip();
			ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "DISPATCHER FEEDBACK ");
			ImGui::Separator();

			if (ImGui::BeginTable("##FetchQueueTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
				
				ImGui::TableSetupColumn("Slot");
				ImGui::TableSetupColumn("FEEDBACK STATUS");
				ImGui::TableHeadersRow();

				for (unsigned int i{}; i < Core::Constants::FETCH_WIDTH; i++) {
					const auto& element = Core::Dispatcher::last_dispatch_feedback()[i];

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("%d", i);

					ImGui::TableSetColumnIndex(1);
					switch (element) {
					case Core::DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH:
						ImGui::PushStyleColor(ImGuiCol_Text, Constants::WHITE);
						ImGui::Text("NO_INSTRUCTION_TO_DISPATCH");
						ImGui::PopStyleColor();
						break;
					case Core::DISPATCH_FEEDBACK::RESERVATION_STATION_WAS_FULL:
						ImGui::PushStyleColor(ImGuiCol_Text, Constants::RED);
						ImGui::Text("RESERVATION_STATION_WAS_FULL");
						ImGui::PopStyleColor();
						break;
					case Core::DISPATCH_FEEDBACK::REGISTER_FILE_WAS_FULL:
						ImGui::PushStyleColor(ImGuiCol_Text, Constants::RED);
						ImGui::Text("REGISTER_FILE_WAS_FULL");
						ImGui::PopStyleColor();
						break;
					case Core::DISPATCH_FEEDBACK::REORDER_BUFFER_WAS_FULL:
						ImGui::PushStyleColor(ImGuiCol_Text, Constants::RED);
						ImGui::Text("REORDER_BUFFER_WAS_FULL");
						ImGui::PopStyleColor();
						break;
					case Core::DISPATCH_FEEDBACK::BLOCKED_BY_EARLIER_INSTRUCTION:
						ImGui::PushStyleColor(ImGuiCol_Text, Constants::RED);
						ImGui::Text("BLOCKED_BY_EARLIER_INSTRUCTION");
						ImGui::PopStyleColor();
						break;
					case Core::DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH:
						ImGui::PushStyleColor(ImGuiCol_Text, Constants::GREEN);
						ImGui::Text("SUCCESSFUL_DISPATCH");
						ImGui::PopStyleColor();
						break;
					default:
						break;
					}
				}
			}
			ImGui::EndTable();
			ImGui::EndTooltip();
		}
	}
}
