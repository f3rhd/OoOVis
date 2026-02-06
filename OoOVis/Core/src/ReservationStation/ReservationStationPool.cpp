#include <Core/ReservationStation/ReservationStationPool.h>
#include <Core/Constants/Constants.h>
#include <iostream>
#include <format>
namespace OoOVis
{
	namespace Core 
	{
		std::vector<Reservation_Station> Reservation_Station_Pool::_pool{
				{RESERVATION_STATION_ID::ADD_SUB},
				{RESERVATION_STATION_ID::BITWISE},
				{RESERVATION_STATION_ID::SET_LESS},
				{RESERVATION_STATION_ID::MULTIPLIER},
				{RESERVATION_STATION_ID::DIVIDER},
				{RESERVATION_STATION_ID::LOAD_STORE},
				{RESERVATION_STATION_ID::BRANCH}
		};

		void Reservation_Station_Pool::flush(memory_addr_t id) {
			for (auto& station : _pool) {
				for (auto& entry : station.get()) {
					if (entry.instruction_id > id) {
						entry.busy = false;
						entry.ready = false;
					}
				}
			}
		}

		void Reservation_Station_Pool::wakeup(u32 producer_tag, data_t produced_data) {
			for (auto&  station : _pool) {
				for (auto& entry : station.get()) {
					if (entry.producer_tag1 == producer_tag) {
						entry.producer_tag1 = NO_PRODUCER_TAG;
						entry.src1 = produced_data;
						entry.ready = entry.producer_tag1 == NO_PRODUCER_TAG && entry.producer_tag2 == NO_PRODUCER_TAG;
#ifdef DEBUG_PRINTS
						std::cout << std::format("Forwared data:{} to Instructions[{}] .\n", produced_data.signed_, entry.instruction_id);
#endif

					}
					if (entry.producer_tag2 == producer_tag) {
						entry.producer_tag2 = NO_PRODUCER_TAG;
						entry.src2 = produced_data;
						entry.ready = entry.producer_tag1 == NO_PRODUCER_TAG && entry.producer_tag2 == NO_PRODUCER_TAG;
#ifdef DEBUG_PRINTS
						std::cout << std::format("Forwared data:{} to Instructions[{}].\n", produced_data.signed_,entry.instruction_id);
#endif
					}
				}
			}
		}
		void Reservation_Station_Pool::deallocate_entry(u32 tag) {
			for (auto& station : _pool) {
				for (auto& entry : station.get()) {
					if (entry.busy && entry.ready && entry.self_tag == tag) {
#ifdef DEBUG_PRINTS
						std::cout << std::format("Deallocated Instructions[{}] from ReservationStation.\n",entry.instruction_id);
#endif
						entry.busy = false;
						entry.ready = false;
						break;
					}
				}
			}
		}

		Reservation_Station& Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID id)
		{
			switch (id)
			{
			case OoOVis::Core::RESERVATION_STATION_ID::ADD_SUB:
				return _pool[0];
				break;
			case OoOVis::Core::RESERVATION_STATION_ID::BITWISE:
				return _pool[1];
				break;
			case OoOVis::Core::RESERVATION_STATION_ID::SET_LESS:
				return _pool[2];
				break;
			case OoOVis::Core::RESERVATION_STATION_ID::MULTIPLIER:
				return _pool[3];
				break;
			case OoOVis::Core::RESERVATION_STATION_ID::DIVIDER:
				return _pool[4];
				break;
			case OoOVis::Core::RESERVATION_STATION_ID::LOAD_STORE:
				return _pool[5];
				break;
			case OoOVis::Core::RESERVATION_STATION_ID::BRANCH:
				return _pool[6];
				break;
			case OoOVis::Core::RESERVATION_STATION_ID::UNKNOWN:
				break;
			default:
				break;
			}
			return _pool[0];
		}

	} // namespace Core
} // namespace OoOVis
