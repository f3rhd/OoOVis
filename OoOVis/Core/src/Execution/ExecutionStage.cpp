#include <Core/Execution/ExecutionStage.h>
#include <Core/Execution/ExecutionUnits.h>
#include <Core/ReservationStation/ReservationStationPool.h>
#include <Core/Constants/Constants.h>

namespace OoOVis
{
	namespace Core
	{

		void Execution_Stage::issue_and_execute() {

			std::vector<const Reservation_Station_Entry*> issued_entries(RESERVATION_STATION_AMOUNT);
			std::vector<Forwarding_Data> forwarding_data;
			for (size_t i{}; i < RESERVATION_STATION_AMOUNT; i++) {
				issued_entries[i] = Reservation_Station_Pool::get_reservation_station(static_cast<RESERVATION_STATION_ID>(i)).issue();
			}
			//ADD_SUB
			//BITWISE
			//SET_LESS
			//MULTIPLIER
			//DIVIDER
			//LOAD_STORE
			//BRANCH
			forwarding_data.push_back(Execution_Unit_Adder::execute(issued_entries[0]));
			forwarding_data.push_back(Execution_Unit_Bitwise::execute(issued_entries[1]));
			forwarding_data.push_back(Execution_Unit_Set_Less_Than::execute(issued_entries[2]));
			forwarding_data.push_back(Execution_Unit_Multiplier::execute(issued_entries[3]));
			forwarding_data.push_back(Execution_Unit_Divider::execute(issued_entries[4]));
			forwarding_data.push_back(Execution_Unit_Load_Store::execute_load());
			forwarding_data.push_back(Execution_Unit_Load_Store::buffer_allocation_phase(issued_entries[5]));
			forwarding_data.push_back(Execution_Unit_Branch::execute(issued_entries[6]));
			// forward the data to reservation stations

			for (const auto& data : forwarding_data) {

				if (data.kind & FORWADING_DATA_FORWARD_ONLY) {
					Reservation_Station_Pool::wakeup(data.producer_tag, data.produced_data);
				}
				else if (data.kind & FORWARDING_DATA_STATION_DEALLOCATE_ONLY) {
					Reservation_Station_Pool::deallocate_entry(data.producer_tag);
				}
				else if (data.kind & FORWARDING_DATA_STATION_DEALLOCATE_AND_WAKEUP) {
					Reservation_Station_Pool::wakeup(data.producer_tag, data.produced_data);
					Reservation_Station_Pool::deallocate_entry(data.producer_tag);
				}
			}
		}
	} // namespace Core
} // namespace OoOVis

