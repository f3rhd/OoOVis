#include <Core/ReservationStation/Pool.h>
std::unordered_map<OoOVis::u32,OoOVis::Core::Reservation_Station> OoOVis::Core::Reservation_Station_Pool::_pool{
		{0,{Core::RESERVATION_STATION_ID::ADD_SUB}},
		{1,{Core::RESERVATION_STATION_ID::BITWISE}},
		{2,{Core::RESERVATION_STATION_ID::SET_LESS}},
		{3,{Core::RESERVATION_STATION_ID::MULTIPLIER}},
		{4,{Core::RESERVATION_STATION_ID::DIVIDER}},
		{5,{Core::RESERVATION_STATION_ID::LOAD_STORE}},
		{6,{Core::RESERVATION_STATION_ID::BRANCH}}
};

void OoOVis::Core::Reservation_Station_Pool::flush(memory_addr_t id) {
	for (auto& [key, station] : _pool) {
		for (auto& entry : station.get()) {
			if (entry.instruction_id > id) {
				entry.busy = false;
				entry.ready = false;
			}
		}
	}
}
