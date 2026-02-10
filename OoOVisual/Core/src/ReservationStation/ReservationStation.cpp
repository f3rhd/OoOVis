#include <Core/ReservationStation/ReservationStation.h>
#include <algorithm>
#include <ranges>

namespace OoOVisual
{
    namespace Core
    {
        Reservation_Station::Reservation_Station(RESERVATION_STATION_ID id) {
            static u32 tag_counter{};
            _id = id;
            for (u32 i(0);i < Constants::RESERVATION_STATION_SIZE;i++) {
                _buffer.emplace_back();
                _buffer.back().self_tag = ++tag_counter;
            }
        }
        Reservation_Station_Entry* Reservation_Station::allocate_entry() {
            for (auto& entry : _buffer) {
                if (!entry.busy ) {
                    entry.busy = true;
                    return &entry;
                }
            }
            // before dispatching the instructions we shall check whether station is full or not
            return nullptr;
        }

        const Reservation_Station_Entry* Reservation_Station::issue() {
            const Reservation_Station_Entry* best = nullptr;
			for (auto& e : _buffer) {
                if (_id == RESERVATION_STATION_ID::BRANCH) { 
                    if (!e.busy) // for branches we have to issue in order 
                        continue;
                }
                else {
                    if (!e.busy && !e.ready)
                        continue;
                }
				if (!best || e.timestamp < best->timestamp)
					best = &e;
			}
            if(best && best->ready)
				return best;
            return nullptr;
        }
        
		std::vector<Reservation_Station_Entry>& Reservation_Station::get() {
            return _buffer;
		}

		bool Reservation_Station::full() {
            for (auto& entry : _buffer) {
                if (!entry.busy) {
                    return false;
                }
            }
            return true;

		}

	} // namespace Core
    
    
} // namespace OoOVis
