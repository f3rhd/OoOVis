#include <Core/ReservationStation/ReservationStation.h>

namespace OoOVis
{
    namespace Core
    {
        Reservation_Station::Reservation_Station(RESERVATION_STATION_ID id, u32 size) {
            for (u32 i(0);i < size;i++) {
                _station.emplace_back();
                _station.back().self_tag = i * static_cast<u32>(id) +1;
            }
        }
        Reservation_Station_Entry* Reservation_Station::allocate_entry(const Reservation_Station_Entry& entry) {
            for (auto& entry : _station) {
                if (!entry.busy) {
                    entry.busy = true;
                    return &entry;
                }
            }
            // before dispatching the instructions we shall check whether station is full or not
            return nullptr;
        }
        
		bool Reservation_Station::full() {
            for (auto& entry : _station) {
                if (!entry.busy) {
                    return false;
                }
            }
            return true;

		}

	} // namespace Core
    
    
} // namespace OoOVis
