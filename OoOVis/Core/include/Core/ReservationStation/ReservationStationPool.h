#pragma once
#include <Core/Types/Types.h>
#include <Core/ReservationStation/ReservationStation.h>
#include <Frontend/Parser/Instruction.h>
#include <memory>
namespace OoOVis
{
	namespace Core
	{
		struct Reservation_Station_Pool {

			static bool flush_mispredicted(memory_addr_t id); 
			static void wakeup(u32 producer_tag, data_t produced_data);
			static void deallocate_entry(u32 tag);
			static Reservation_Station& get_reservation_station(RESERVATION_STATION_ID id);
		private:
			static std::vector<Core::Reservation_Station> _pool;
		};

	} // namespace Core
} // namespace OoOVis