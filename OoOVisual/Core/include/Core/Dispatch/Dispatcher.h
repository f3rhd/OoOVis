#pragma once

#include <Frontend/Parser/Instruction.h>
#include <Core/ReservationStation/ReservationStation.h>
#include <Core/Fetch/Fetch_Elements.h>
#include <memory>
namespace OoOVisual
{
    namespace Core
    {
        class Dispatcher {

        public:
            Dispatcher() = default;
            bool                                                   dispatch_instruction(const Fetch_Element& fetch_element);
            std::pair<RESERVATION_STATION_ID, EXECUTION_UNIT_MODE> get_reservation_station_id_and_execution_mode_for_register_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction);
        private:
            bool                                                   dispatch_register_instruction(const Fetch_Element& element, Reservation_Station& station);
            bool                                                   dispatch_load_instruction(const Fetch_Element& element, Reservation_Station& station);
            bool                                                   dispatch_store_instruction(const Fetch_Element& element, Reservation_Station& station);
            bool                                                   dispatch_branch_instruction(const Fetch_Element& element, Reservation_Station& station);
            bool                                                   dispatch_jump_instruction(const Fetch_Element& element,  Reservation_Station& station);

        };
        
    } // namespace Core
    
} // namespace OoOVis
