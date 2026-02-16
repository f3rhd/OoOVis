#pragma once

#include <Core/ReservationStation/ReservationStation.h>
#include <Core/Fetch/FetchElements.h>
#include <map>
namespace OoOVisual
{
    namespace Core
    {
        enum class DISPATCH_FEEDBACK : u8 {
            NO_INSTRUCTION_TO_DISPATCH,
            RESERVATION_STATION_WAS_FULL,
            REGISTER_FILE_WAS_FULL,
            REORDER_BUFFER_WAS_FULL,
            BLOCKED_BY_EARLIER_INSTRUCTION,
            SUCCESSFUL_DISPATCH
        };
        class Dispatcher {

        public:
            Dispatcher() = default;
            static std::vector<DISPATCH_FEEDBACK>                                                                                    dispatch_fetch_group();
            static const std::vector<DISPATCH_FEEDBACK>&                                                                             last_dispatch_feedback();
            static DISPATCH_FEEDBACK                                                                                                 dispatch_feedback_of(RESERVATION_STATION_ID id); // used by visualizer
        private:
            static DISPATCH_FEEDBACK                                                   dispatch_fetch_element(const Fetch_Element& fetch_element);
            static DISPATCH_FEEDBACK                                                   dispatch_register_instruction(const Fetch_Element& element, Reservation_Station& station);
            static DISPATCH_FEEDBACK                                                   dispatch_load_instruction(const Fetch_Element& element, Reservation_Station& station);
            static DISPATCH_FEEDBACK                                                   dispatch_store_instruction(const Fetch_Element& element, Reservation_Station& station);
            static DISPATCH_FEEDBACK                                                   dispatch_branch_instruction(const Fetch_Element& element, Reservation_Station& station);
            static DISPATCH_FEEDBACK                                                   dispatch_jump_instruction(const Fetch_Element& element,  Reservation_Station& station);
        private:
            static std::vector<DISPATCH_FEEDBACK>                                      _last_dispatch_feedback;
            static std::map<Core::RESERVATION_STATION_ID, DISPATCH_FEEDBACK>            _station_dispatch_map; //used by visualizer

        };
        
    } // namespace Core
    
} // namespace OoOVis
