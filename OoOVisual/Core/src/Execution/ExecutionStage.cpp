#include <Core/Execution/ExecutionStage.h>
#include <Core/Execution/ExecutionUnits.h>
#include <Core/ReservationStation/ReservationStationPool.h>
#include <Core/Constants/Constants.h>

namespace OoOVisual
{
	namespace Core
	{
		std::vector<const Reservation_Station_Entry*> Execution_Stage::_last_issued_entries(Constants::RESERVATION_STATION_AMOUNT,nullptr);
		void Execution_Stage::issue_and_execute() {

			std::vector<const Reservation_Station_Entry*> issued_entries(Constants::RESERVATION_STATION_AMOUNT-1);
			std::vector<Forwarding_Data> forwarding_data;
			auto branch_execution_input = Reservation_Station_Pool::get_reservation_station(static_cast<RESERVATION_STATION_ID>(6)).issue();
			// upon misprediction execution unit may flush the instructions that are ready to execute.
			forwarding_data.push_back(Execution_Unit_Branch::execute(branch_execution_input));
			for (size_t i{}; i < Constants::RESERVATION_STATION_AMOUNT - 1; i++) {
				issued_entries[i] = Reservation_Station_Pool::get_reservation_station(static_cast<RESERVATION_STATION_ID>(i)).issue();
			}
			forwarding_data.push_back(Execution_Unit_Adder::execute(issued_entries[0]));
			forwarding_data.push_back(Execution_Unit_Bitwise::execute(issued_entries[1]));
			forwarding_data.push_back(Execution_Unit_Set_Less_Than::execute(issued_entries[2]));
			forwarding_data.push_back(Execution_Unit_Multiplier::execute(issued_entries[3]));
			forwarding_data.push_back(Execution_Unit_Divider::execute(issued_entries[4]));
			forwarding_data.push_back(Execution_Unit_Load_Store::execute_load());
			forwarding_data.push_back(Execution_Unit_Load_Store::buffer_allocation_phase(issued_entries[5]));
			// forward the data to reservation stations

			for (const auto& data : forwarding_data) {

				if (data.kind & Constants::FORWADING_DATA_FORWARD_ONLY) {
					Reservation_Station_Pool::wakeup(data.producer_tag, data.produced_data);
				}
				else if (data.kind & Constants::FORWARDING_DATA_STATION_DEALLOCATE_ONLY) {
					Reservation_Station_Pool::deallocate_entry(data.producer_tag);
				}
				else if (data.kind & Constants::FORWARDING_DATA_STATION_DEALLOCATE_AND_FORWARD) {
					Reservation_Station_Pool::wakeup(data.producer_tag, data.produced_data);
					Reservation_Station_Pool::deallocate_entry(data.producer_tag);
				}
			}
			// visualizer will use these to render stuff
			_last_issued_entries = issued_entries;
			_last_issued_entries.push_back(branch_execution_input);
		}

		void Execution_Stage::reset()
		{
			_last_issued_entries = std::vector<const Reservation_Station_Entry*>(Constants::RESERVATION_STATION_AMOUNT,nullptr);
		}

		const std::vector<const OoOVisual::Core::Reservation_Station_Entry*> Execution_Stage::issued()
		{
			return _last_issued_entries;
		}

	} // namespace Core
} // namespace OoOVis

