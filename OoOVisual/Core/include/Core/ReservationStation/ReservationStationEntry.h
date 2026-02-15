#pragma once
#include <Core/Types/Types.h>
#include <Core/Constants/Constants.h>
#include <core/Execution/ExecutionUnitModes.h>
namespace OoOVisual 
{
    namespace Core
    {
		enum class RESERVATION_STATION_ID : u8 {
			ADD_SUB,
			BITWISE,
			SET_LESS,
			MULTIPLIER,
			DIVIDER,
			LOAD_STORE,
			BRANCH,
			UNKNOWN
		};
		struct Reservation_Station_Entry {
			u64	   reorder_buffer_entry_index = 0;
			data_t src1 = {};
			data_t src2 = {};
			u32    producer_tag1 = Constants::NO_PRODUCER_TAG;
			u32    producer_tag2 = Constants::NO_PRODUCER_TAG;
			u32    self_tag = Constants::NO_PRODUCER_TAG;
			u32    instruction_address = 0;
			reg_id_t destination_register_id = Constants::INVALID_PHYSICAL_REGISTER_ID;
			reg_id_t store_source_register_id = Constants::INVALID_PHYSICAL_REGISTER_ID; // will need this in load forwarding and bypassing
			memory_addr_t branch_target = 0;
			u32 timestamp = Constants::TIME_ZERO;
			u8 fetch_unit_prediction = Constants::NOT_BRANCH_INSTRUCTION;
			bool   ready = false;
			bool   busy = false;
			bool   destination_register_id_as_ofsset = false;
			EXECUTION_UNIT_MODE mode = EXECUTION_UNIT_MODE::UNKNOWN;
		};

    } // namespace Core
} // namespace OoOVis
