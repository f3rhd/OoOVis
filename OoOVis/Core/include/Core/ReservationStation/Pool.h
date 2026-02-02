#pragma once
#include <Core/Types/Types.h>
#include <Core/ReservationStation/ReservationStation.h>
namespace OoOVis
{
	namespace Core
	{
		struct Reservation_Station_Pool {

			static void flush(memory_addr_t id); // if larger flush the entries with larger ids else less
		private:
			static std::unordered_map<u32, Core::Reservation_Station> _pool;
		};
	}
}