#pragma once
#include <Core/ReservationStation/ReservationStationEntry.h>
#include <vector>
namespace OoOVisual
{
	namespace Core {

		class Reservation_Station {

		public:
			Reservation_Station(RESERVATION_STATION_ID id);
			Reservation_Station_Entry* allocate_entry();
			const Reservation_Station_Entry* issue();
			std::vector<Reservation_Station_Entry>& get();
			const std::vector<Reservation_Station_Entry>& getc() const; // 
			RESERVATION_STATION_ID id() const { return _id; }
			bool full() const;
		private:
			std::vector<Reservation_Station_Entry> _buffer; 
			RESERVATION_STATION_ID _id;
		};

	} // namespace Core

} // namespace OoOVis
