#pragma once
#include <Core/ReservationStation/ReservationStationEntry.h>
#include <unordered_map>
#include <vector>
namespace OoOVis
{
	namespace Core {

		class Reservation_Station {

		public:
			Reservation_Station(RESERVATION_STATION_ID id, u32 size);
			Reservation_Station_Entry* allocate_entry();
			bool full();
		private:
			std::vector<Reservation_Station_Entry> _station; // u32 is the tag of the entry
		};

	}

}
