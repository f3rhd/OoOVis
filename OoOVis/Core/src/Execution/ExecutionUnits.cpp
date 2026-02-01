#include <Core/Execution/ExecutionUnits.h>
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/Fetch/Fetch.h>

namespace OoOVis
{
	namespace Core
	{

		Execution_Result Execution_Unit_Adder::execute(const Reservation_Station_Entry* source_entry)
		{
			if (!source_entry)
				return { false };
			Execution_Result result{};
			result.valid = true;
			result.producer_tag = source_entry->self_tag;
			result.destination_physical_register = source_entry->destination_id;
			result.target_reorder_buffer_entry_index = source_entry->reorder_buffer_entry_index;
			switch (source_entry->mode) {
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_ADD:
				result.write_data.signed_ = source_entry->src1.signed_ + source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_SUB:
				result.write_data.signed_ = source_entry->src1.signed_ - source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_AUIPC: // immediate value lives in the source 2
				result.write_data.unsigned_ = (source_entry->src2.unsigned_ << 20) & 0xFFFFF000 + Fetch_Unit::get_program_counter();
				break;
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_LOAD_UPPER:
				result.write_data.unsigned_ = (source_entry->src2.unsigned_ << 20) & 0xFFFFF000;
			// shouldnt happen
			default:
				break;
			}
			return result;
		}

		Execution_Result Execution_Unit_Bitwise::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { false };
			Execution_Result result{};
			result.valid = true;
			result.producer_tag = source_entry->self_tag;
			result.destination_physical_register = source_entry->destination_id;
			result.target_reorder_buffer_entry_index = source_entry->reorder_buffer_entry_index;
			switch (source_entry->mode) {
			case EXECUTION_UNIT_MODE::BITWISE_AND:
				result.write_data.signed_ = source_entry->src1.signed_ & source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::BITWISE_XOR:
				result.write_data.signed_ = source_entry->src1.signed_ | source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::LOGICAL_XOR:
				result.write_data.signed_ = source_entry->src1.signed_ ^ source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::BITWISE_SHIFT_LEFT_LOGICAL:
				result.write_data.unsigned_ = source_entry->src1.unsigned_ << source_entry->src2.unsigned_;
				break;
			case EXECUTION_UNIT_MODE::BITWISE_SHIFT_RIGHT_LOGICAL:
				result.write_data.unsigned_ = source_entry->src1.unsigned_ >> source_entry->src2.unsigned_;
				break;
			case EXECUTION_UNIT_MODE::BITWISE_SHIFT_RIGHT_ARITHMETIC:
				result.write_data.signed_ = source_entry->src1.signed_ >> source_entry->src2.unsigned_;
			// shouldnt happen
			default:
				break;
			}

			return result;
		}

		Execution_Result Execution_Unit_Set_Less_Than::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { false };
			Execution_Result result{};
			result.valid = true;
			result.producer_tag = source_entry->self_tag;
			result.destination_physical_register = source_entry->destination_id;
			result.target_reorder_buffer_entry_index = source_entry->reorder_buffer_entry_index;
			result.write_data.signed_ = source_entry->src1.signed_ < source_entry->src2.signed_ ? 1 : 0;
			return result;
		}

		Execution_Result Execution_Unit_Multiplier::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { false };
			Execution_Result result{};
			result.valid = true;
			result.producer_tag = source_entry->self_tag;
			result.destination_physical_register = source_entry->destination_id;
			result.target_reorder_buffer_entry_index = source_entry->reorder_buffer_entry_index;
			switch (source_entry->mode) {
			case EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_SIGNED:
			case EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH:
				result.write_data.signed_ = source_entry->src1.signed_ * source_entry->src2.signed_;
				break;
				case EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH_SIGNED_UNSIGNED: {
                // MULHSU: src1 is signed, src2 is unsigned
                // We cast to 64-bit to ensure we don't lose the overflow
					int64_t full_product{ static_cast<int64_t>(source_entry->src1.signed_) * static_cast<uint64_t>(source_entry->src2.unsigned_) };
                result.write_data.signed_ = static_cast<int32_t>(full_product >> 32);
                break;
            }
            case EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH_UNSIGNED: {
                // MULHU: Both src1 and src2 are unsigned
				uint64_t full_product{ static_cast<uint64_t>(source_entry->src1.unsigned_) * static_cast<uint64_t>(source_entry->src2.unsigned_) };
                result.write_data.unsigned_ = static_cast<uint32_t>(full_product >> 32);
                break;
            }
			// shouldnt happen
			default:
				break;

			}
			return result;
		}

		Execution_Result Execution_Unit_Divider::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { false };
			Execution_Result result{};
			result.valid = true;
			result.producer_tag = source_entry->self_tag;
			result.destination_physical_register = source_entry->destination_id;
			result.target_reorder_buffer_entry_index = source_entry->reorder_buffer_entry_index;

			switch (source_entry->mode) {
			case EXECUTION_UNIT_MODE::DIVIDER_DIVIDE_SIGNED:
				result.write_data.signed_ = source_entry->src1.signed_ / source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::DIVIDER_DIVIDE_UNSIGNED:
				result.write_data.unsigned_ = source_entry->src1.unsigned_ / source_entry->src2.unsigned_;
				break;
			case EXECUTION_UNIT_MODE::DIVIDER_REMAINDER_SIGNED:
				result.write_data.signed_ = source_entry->src1.signed_ % source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::DIVIDER_REMAINDER_UNSIGNED:
				result.write_data.unsigned_ = source_entry->src1.unsigned_ % source_entry->src2.unsigned_;
				break;
			// shouldnt happen
			default:
				break;

			}
			return result;
		}

	} // namespace Core
} // namespace OoOVis