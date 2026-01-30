#pragma once

#include <Core/Types/Types.h>
#include <Frontend/Parser/Instruction.h>
#include <memory>
namespace OoOVis
{
    enum class RESERVATION_STATION_ID {
        ADD_SUB,
        LOGICAL,
        SET_LESS,
        MULTIPLIER,
        DIVIDER,
        LOAD_STORE,
        BRANCH,
        UNKNOWN
    };
    enum class EXECUTION_UNIT_MODE {
        ADD_SUB_UNIT_ADD,
        ADD_SUB_UNIT_SUB,
        ADD_SUB_UNIT_LOAD_UPPER,
        ADD_SUB_UNIT_AUIPC,
        DIVIDER_DIVIDE_SIGNED,
        DIVIDER_DIVIDE_UNSIGNED,
        DIVIDER_REMAINDER,
        DIVIDER_REMAINDER_UNSIGNED,
        MULTIPLIER_MULTIPLY_SIGNED,
        MULTIPLIER_MULTIPLY_HIGH,
        MULTIPLIER_MULTIPLY_HIGH_SIGNED_UNSIGNED,
        MULTIPLIER_MULTIPLY_HIGH_UNSIGNED,
        LOGICAL_AND,
        LOGICAL_OR,
        LOGICAL_XOR,
        LOGICAL_SHIFT_LEFT_LOGICAL,
        LOGICAL_SHIFT_RIGHT_LOGICAL,
        LOGICAL_SHIFT_RIGHT_ARITHMETIC,
        SET_LESS_THAN
    };
    namespace Core
    {
        struct Reservation_Station_Entry {
            EXECUTION_UNIT_MODE mode;
            data_t src1;
            data_t src2;
            data_t dest;
            u32    tag1;
            u32    tag2;
            bool   ready;
            bool   busy;
        };
        class Dispatch {

        public:
            Dispatch() = default;
            RESERVATION_STATION_ID assign_reservation_station_id_to(const std::unique_ptr<FrontEnd::Instruction>& instruction);

        };
        
    } // namespace Core
    
    
} // namespace OoOVis
