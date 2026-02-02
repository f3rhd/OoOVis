#pragma once
#include <Core/Types/Types.h>
#include <Core/Constants/Constants.h>
namespace OoOVis 
{
    namespace Core
    {
		enum class RESERVATION_STATION_ID : u32 {
			ADD_SUB,
			BITWISE,
			SET_LESS,
			MULTIPLIER,
			DIVIDER,
			LOAD_STORE,
			BRANCH,
			UNKNOWN
		};
		enum class EXECUTION_UNIT_MODE {
			BRANCH_CONDITIONAL,
			BRANCH_UNCONDITIONAL,
			BRANCH_CONDITIONAL_EQUAL,
			BRANCH_CONDITIONAL_NOT_EQUAL,
			BRANCH_CONDITIONAL_LESS_THAN,
			BRANCH_CONDITIONAL_GREATER_THAN,
			BRANCH_CONDITIONAL_LESS_THAN_UNSIGNED,
			BRANCH_CONDITIONAL_GREATER_THAN_UNSIGNED,
			LOAD_STORE_LOAD_WORD,
			LOAD_STORE_LOAD_BYTE,
			LOAD_STORE_LOAD_HALF,
			LOAD_STORE_LOAD_BYTE_UNSIGNED,
			LOAD_STORE_LOAD_HALF_UNSIGNED,
			LOAD_STORE_STORE_WORD,
			LOAD_STORE_STORE_BYTE,
			LOAD_STORE_STORE_HALF,
			ADD_SUB_UNIT_ADD,
			ADD_SUB_UNIT_SUB,
			ADD_SUB_UNIT_LOAD_UPPER,
			ADD_SUB_UNIT_AUIPC,
			DIVIDER_DIVIDE_SIGNED,
			DIVIDER_DIVIDE_UNSIGNED,
			DIVIDER_REMAINDER_SIGNED,
			DIVIDER_REMAINDER_UNSIGNED,
			MULTIPLIER_MULTIPLY_SIGNED,
			MULTIPLIER_MULTIPLY_HIGH,
			MULTIPLIER_MULTIPLY_HIGH_SIGNED_UNSIGNED,
			MULTIPLIER_MULTIPLY_HIGH_UNSIGNED,
			BITWISE_AND,
			BITWISE_XOR,
			LOGICAL_XOR,
			BITWISE_SHIFT_LEFT_LOGICAL,
			BITWISE_SHIFT_RIGHT_LOGICAL,
			BITWISE_SHIFT_RIGHT_ARITHMETIC,
			SET_LESS_THAN,
			UNKNOWN
		};

		struct Reservation_Station_Entry {
			EXECUTION_UNIT_MODE mode = EXECUTION_UNIT_MODE::UNKNOWN;
			data_t src1 = {};
			data_t src2 = {};
			u32    producer_tag1 = NO_PRODUCER_TAG;
			u32    producer_tag2 = NO_PRODUCER_TAG;
			u32    self_tag = NO_PRODUCER_TAG;
			reg_id_t destination_register_id = INVALID_REGISTER_ID;
			u64	   reorder_buffer_entry_index = 0;
			bool   ready = false;
			bool   busy = false;
			u32    instruction_id = NO_STORE;
			reg_id_t store_source_register_id = INVALID_REGISTER_ID; // will need this in load forwarding and bypassing
			memory_addr_t branch_target = 0;
			//branch_instruction_id_t branch_id = 0;
		};

    }
}
