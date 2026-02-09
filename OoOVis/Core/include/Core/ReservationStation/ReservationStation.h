#pragma once
#include <Core/ReservationStation/ReservationStationEntry.h>
#include <vector>
namespace OoOVis
{
	namespace Core {

		class Reservation_Station {

		public:
			Reservation_Station(RESERVATION_STATION_ID id);
			Reservation_Station_Entry* allocate_entry();
			const Reservation_Station_Entry* issue();
			std::vector<Reservation_Station_Entry>& get();
			RESERVATION_STATION_ID id() const { return _id; }
			bool full();
		private:
			std::vector<Reservation_Station_Entry> _buffer; 
			RESERVATION_STATION_ID _id;
		};

	} // namespace Core

} // namespace OoOVis
