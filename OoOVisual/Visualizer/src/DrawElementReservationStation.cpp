#include <Visualizer/DrawElementReservationStation.h>
#include <Visualizer/Constants.h>
#include <Core/ReservationStation/ReservationStationPool.h>

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
			if (_core_station->full()) {
				ImGui::PushStyleColor(ImGuiCol_Text,Constants::RED);
				ImGui::Text("Station is full.");
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text,Constants::GREEN);
				ImGui::Text("Station is not full.");
			}

			ImGui::PopStyleColor();
			ImGui::EndTooltip();
		}

		void Draw_Element_Reservation_Station::show_detailed() { // @ToDo

		}

	}
}