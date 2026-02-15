#include <Visualizer/Units/DrawElementFetchUnit.h>
#include <Visualizer/Constants.h>
#include <Core/Fetch/Fetch.h>
#include <iostream>
namespace OoOVisual
{
	namespace Visualizer
	{

		void Draw_Element_Fetch_Unit::show_detailed() {
			ImGuiViewport* viewport{ ImGui::GetMainViewport() };
			float padding{ 20.0f };
			
			ImVec2 initial_pos{ viewport->WorkPos.x + viewport->WorkSize.x - padding, 
								viewport->WorkPos.y + padding };
			ImVec2 window_pivot{ 1.0f, 0.0f }; 

			ImGui::SetNextWindowPos(initial_pos, ImGuiCond_FirstUseEver, window_pivot);
			ImGui::SetNextWindowBgAlpha(0.9f);

			ImGuiWindowFlags window_flags{ ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings };

			if (ImGui::Begin("Fetch Unit Details", nullptr, window_flags)) {
				
				ImGui::TextColored(ImVec4{ 0.4f, 0.7f, 1.0f, 1.0f }, "FETCH UNIT INTERNALS");
				ImGui::Separator();

				if (ImGui::CollapsingHeader("Instruction Stream", ImGuiTreeNodeFlags_DefaultOpen)) {
					if (ImGui::BeginChild("InstructionScroll", ImVec2{ 0.0f, 150.0f }, true)) {
						const auto& stream{ Core::Fetch_Unit::instruction_stream() };
						for (size_t i{ 0 }; i < stream.size(); ++i) {
							ImGui::TextDisabled("[%04zu]", i);
							ImGui::SameLine();
							ImGui::Text("%s", stream[i].c_str());
						}
					}
					ImGui::EndChild();
				}

				if (ImGui::CollapsingHeader("Branch Target Buffer (BTB)")) {
					ImGuiTableFlags btb_flags{ ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY };
					if (ImGui::BeginTable("##BTBTable", 2, btb_flags, ImVec2{ 0.0f, 120.0f })) {
						ImGui::TableSetupScrollFreeze(0, 1);
						ImGui::TableSetupColumn("PC");
						ImGui::TableSetupColumn("Target");
						ImGui::TableHeadersRow();

						for (const auto& pair : Core::Fetch_Unit::branch_target_buffer()) {
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0); ImGui::Text("0x%08llX", pair.first);
							ImGui::TableSetColumnIndex(1); ImGui::TextColored(ImVec4{ 0.4f, 0.8f, 1.0f, 1.0f }, "0x%08llX", pair.second);
						}
						ImGui::EndTable();
					}
				}
				
				if (ImGui::CollapsingHeader("Pattern History Table (PHT)")) {
					ImGuiTableFlags pht_flags{ ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY };
					if (ImGui::BeginTable("##PHTTable", 2, pht_flags, ImVec2{ 0.0f, 120.0f })) {
						ImGui::TableSetupScrollFreeze(0, 1);
						ImGui::TableSetupColumn("Hash");
						ImGui::TableSetupColumn("State");
						ImGui::TableHeadersRow();

						for (const auto& pair : Core::Fetch_Unit::pattern_history_table()) {
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0); ImGui::Text("0x%04X", pair.first);
							ImGui::TableSetColumnIndex(1);
							u32 counter{ pair.second };
							if (counter >= 2) {
								ImGui::TextColored(ImVec4{ 0.2f, 1.0f, 0.2f, 1.0f }, "T (%u)", counter);
							} else {
								ImGui::TextColored(ImVec4{ 1.0f, 0.4f, 0.4f, 1.0f }, "NT (%u)", counter);
							}
						}
						ImGui::EndTable();
					}
				}
			}
			ImGui::End();
		}
		void Draw_Element_Fetch_Unit::show_tooltip() {
			auto current_pc{ Core::Fetch_Unit::program_counter() };
			ImGui::SetTooltip(
				"Fetch Unit\nPc: %u"
				,current_pc
			);
		}
	}
}

