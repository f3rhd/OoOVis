#pragma once

#include <Frontend/Parser/Instruction.h>
#include <Core/ReservationStation/ReservationStation.h>
#include <memory>
namespace OoOVis
{
    namespace Core
    {
        class Dispatcher {

        public:
            Dispatcher() = default;
            bool                                                   dispatch_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction,const memory_addr_t instruction_id);
            std::pair<RESERVATION_STATION_ID, EXECUTION_UNIT_MODE> get_reservation_station_id_and_execution_mode_for(const std::unique_ptr<FrontEnd::Instruction>& instruction);
        private:
            bool                                                   dispatch_register_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, const memory_addr_t instruction_id, Reservation_Station& station);
            bool                                                   dispatch_load_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, const memory_addr_t instruction_id, Reservation_Station& station);
            bool                                                   dispatch_store_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, const memory_addr_t instruction_id, Reservation_Station& station);
            bool                                                   dispatch_branch_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, const memory_addr_t instruction_id, Reservation_Station& station);
            bool                                                   dispatch_jump_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, const memory_addr_t instruction_id,  Reservation_Station& station);

        };
        
    } // namespace Core
    
} // namespace OoOVis
