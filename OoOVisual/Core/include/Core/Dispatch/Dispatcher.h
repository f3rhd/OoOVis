#pragma once

#include <Frontend/Parser/Instruction.h>
#include <Core/ReservationStation/ReservationStation.h>
#include <Core/Fetch/Fetch_Elements.h>
#include <memory>
namespace OoOVisual
{
    namespace Core
    {
        enum class DISPATCH_FEEDBACK : u8 {
            NO_INSTRUCTION_TO_DISPATCH,
            RESERVATION_STATION_WAS_FULL,
            REGISTER_FILE_WAS_FULL,
            REORDER_BUFFER_WAS_FULL,
            SUCCESSFUL_DISPATCH
        };
        class Dispatcher {

        public:
            Dispatcher() = default;
            static std::vector<DISPATCH_FEEDBACK>                                      dispatch_fetch_group();
        private:
            static DISPATCH_FEEDBACK                                                   dispatch_fetch_element(const Fetch_Element& fetch_element);
            static DISPATCH_FEEDBACK                                                   dispatch_register_instruction(const Fetch_Element& element, Reservation_Station& station);
            static DISPATCH_FEEDBACK                                                   dispatch_load_instruction(const Fetch_Element& element, Reservation_Station& station);
            static DISPATCH_FEEDBACK                                                   dispatch_store_instruction(const Fetch_Element& element, Reservation_Station& station);
            static DISPATCH_FEEDBACK                                                   dispatch_branch_instruction(const Fetch_Element& element, Reservation_Station& station);
            static DISPATCH_FEEDBACK                                                   dispatch_jump_instruction(const Fetch_Element& element,  Reservation_Station& station);

        };
        
    } // namespace Core
    
} // namespace OoOVis
