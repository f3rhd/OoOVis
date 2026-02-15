#include <Visualizer/Units/DrawElementReservationStation.h>
#include <Visualizer/Constants.h>
#include <Core/ReservationStation/ReservationStationPool.h>
#include <Core/ReservationStation/ReservationStationEntry.h>

namespace OoOVisual
{
	namespace Visualizer
	{


		Draw_Element_Reservation_Station::Draw_Element_Reservation_Station(DRAW_ELEMENT_ID id, ImVec2 position, ImVec2 dimension) : Draw_Element(id,position,dimension){
			switch (_id)
			{
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::STATION_0:
				_core_station = &Core::Reservation_Station_Pool::get_reservation_station(Core::RESERVATION_STATION_ID::ADD_SUB);
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::STATION_1:
				_core_station = &Core::Reservation_Station_Pool::get_reservation_station(Core::RESERVATION_STATION_ID::BITWISE);
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::STATION_2:
				_core_station = &Core::Reservation_Station_Pool::get_reservation_station(Core::RESERVATION_STATION_ID::SET_LESS);
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::STATION_3:
				_core_station = &Core::Reservation_Station_Pool::get_reservation_station(Core::RESERVATION_STATION_ID::MULTIPLIER);
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::STATION_4:
				_core_station = &Core::Reservation_Station_Pool::get_reservation_station(Core::RESERVATION_STATION_ID::DIVIDER);
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::STATION_5:
				_core_station = &Core::Reservation_Station_Pool::get_reservation_station(Core::RESERVATION_STATION_ID::LOAD_STORE);
				break;
			case OoOVisual::Visualizer::DRAW_ELEMENT_ID::STATION_6:
				_core_station = &Core::Reservation_Station_Pool::get_reservation_station(Core::RESERVATION_STATION_ID::BRANCH);
				break;
			default:
				break;
			}
		}

		void Draw_Element_Reservation_Station::show_tooltip() {
			ImGui::BeginTooltip();
			ImGui::Text("Reservation Station Entries");
			ImGui::Separator();

			// slot | busy | ready | dest | src1/tag1 | src2/tag2 | special | rob
			if (ImGui::BeginTable("##ReservationStation", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
				ImGui::TableSetupColumn("Slot");
				ImGui::TableSetupColumn("Busy"); 
				ImGui::TableSetupColumn("Ready");
				ImGui::TableSetupColumn("Dest");
				ImGui::TableSetupColumn("Src1/T1");
				ImGui::TableSetupColumn("Src2/T2");
				ImGui::TableSetupColumn("Specialized Info");
				ImGui::TableSetupColumn("ROB Idx");
				ImGui::TableHeadersRow();

				for (u32 i{0}; i < Core::Constants::RESERVATION_STATION_SIZE; ++i) {
					const auto& entry{_core_station->getc()[i]};
					
					if (entry.mode == Core::EXECUTION_UNIT_MODE::UNKNOWN) continue; 

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("%u", i);

					ImGui::TableSetColumnIndex(1);
					if (entry.busy) {
						ImGui::TextColored(ImVec4{1.0f, 0.4f, 0.4f, 1.0f}, "BUSY");
					} else {
						ImGui::TextDisabled("Idle");
					}

					// Ready Status
					ImGui::TableSetColumnIndex(2);
					if (entry.ready) {
						ImGui::TextColored(ImVec4{0.2f, 1.0f, 0.2f, 1.0f}, "READY");
					} else {
						ImGui::TextColored(ImVec4{1.0f, 0.8f, 0.2f, 1.0f}, "WAIT");
					}

					// Destination Register
					ImGui::TableSetColumnIndex(3);
					if (entry.destination_register_id != Core::Constants::INVALID_PHYSICAL_REGISTER_ID) {
						ImGui::Text("P%u", entry.destination_register_id);
					} else {
						ImGui::TextDisabled("-");
					}

					// Source 1 / Tag 1
					ImGui::TableSetColumnIndex(4);
					if (entry.producer_tag1 != Core::Constants::NO_PRODUCER_TAG) {
						ImGui::TextColored(ImVec4{1.0f, 0.6f, 0.6f, 1.0f}, "T:%u", entry.producer_tag1);
					} else {
						ImGui::Text("V:%llu", entry.src1);
					}

					// Source 2 / Tag 2
					ImGui::TableSetColumnIndex(5);
					if (entry.producer_tag2 != Core::Constants::NO_PRODUCER_TAG) {
						ImGui::TextColored(ImVec4{1.0f, 0.6f, 0.6f, 1.0f}, "T:%u", entry.producer_tag2);
					} else {
						ImGui::Text("V:%llu", entry.src2);
					}

					// Specialized Logic
					ImGui::TableSetColumnIndex(6);
					if (_core_station->id() == Core::RESERVATION_STATION_ID::BRANCH) {
						ImGui::Text("Tgt: 0x%08X", entry.branch_target);
						if (entry.fetch_unit_prediction != Core::Constants::NOT_BRANCH_INSTRUCTION) {
							ImGui::SameLine();
							ImGui::TextColored(ImVec4{0.4f, 0.8f, 1.0f, 1.0f}, "[Pred]");
						}
					}
					else if (_core_station->id() == Core::RESERVATION_STATION_ID::LOAD_STORE) {
						if (entry.store_source_register_id != Core::Constants::INVALID_PHYSICAL_REGISTER_ID) {
							ImGui::Text("StoreSrc: P%u", entry.store_source_register_id);
						} else {
							ImGui::Text("Mem Mode");
						}
					}
					else {
						ImGui::TextDisabled("Standard Reg");
					}

					// ROB Index
					ImGui::TableSetColumnIndex(7);
					ImGui::Text("#%llu", entry.reorder_buffer_entry_index);
				}
				ImGui::EndTable();
			}
			ImGui::EndTooltip();
		}
	}
}