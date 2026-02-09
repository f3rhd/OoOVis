#include <Core/ReservationStation/ReservationStation.h>
#include <algorithm>
#include <ranges>

namespace OoOVis
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
			std::ranges::sort(_buffer, [](const Reservation_Station_Entry& a, const Reservation_Station_Entry& b) {return a.instruction_id < b.instruction_id; });
            for (const auto& entry : _buffer) {
                if (entry.ready && entry.busy)
                    return &entry;
            }
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
