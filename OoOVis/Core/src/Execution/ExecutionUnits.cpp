#include <Core/Execution/ExecutionUnits.h>
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/DCache/DCache.h>
#include <Core/Commit/ReorderBuffer.h>
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
			result.destination_physical_register = source_entry->destination_register_id;
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
				break;
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
			result.destination_physical_register = source_entry->destination_register_id;
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
				break;
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
			result.destination_physical_register = source_entry->destination_register_id;
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
			result.destination_physical_register = source_entry->destination_register_id;
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
			result.destination_physical_register = source_entry->destination_register_id;
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
		std::vector<Execution_Unit_Load_Store::Buffer_Entry> Execution_Unit_Load_Store::_store_buffer{};
		std::vector<Execution_Unit_Load_Store::Buffer_Entry> Execution_Unit_Load_Store::_load_buffer{};
		void Execution_Unit_Load_Store::buffer_allocation_phase(const Reservation_Station_Entry* source_entry) {

			if (!source_entry)
				return;
			memory_addr_t address(source_entry->src1.signed_ + static_cast<i32>(source_entry->destination_register_id));
			data_t        register_data{ source_entry->src2 };
			if (source_entry->store_id != NO_STORE && !store_buffer_is_full()) {
				// allocate  the entry in the store buffer
				_store_buffer.emplace_back(
					source_entry->store_id,
					static_cast<u32>(source_entry->reorder_buffer_entry_index), 
					source_entry->store_source_register_id,
					register_data, 
					address,
					NO_PRODUCER_TAG
				);
				// tell the rob that address and the data is ready
				Reorder_Buffer::set_ready(static_cast<u32>(source_entry->reorder_buffer_entry_index));
				return;
			}
			if (!load_buffer_is_full()) {
				_load_buffer.emplace_back(
					NO_STORE,
					source_entry->destination_register_id,
					source_entry->reorder_buffer_entry_index,
					register_data,
					address,
					source_entry->self_tag
				);
				return;
			}
		}

		Execution_Result Execution_Unit_Load_Store::execute_load() {

			size_t bypassable_load_entry_index{};
			bool can_bypass{ false };
			// select a load instruction that is bypassable (if it exists)
			for (size_t i{ 0 }; i < _load_buffer.size(); i++) {
				for (const auto& entry : _store_buffer) {
					if (_load_buffer.at(bypassable_load_entry_index).calculated_address != entry.calculated_address)
						can_bypass = true;
					else
						can_bypass = false;
				}
				if (can_bypass) {
					bypassable_load_entry_index = i;
					break;
				}

			}
			if (can_bypass) {
				// execute it directly
				// read from memory
				const Buffer_Entry* bypassable_load_entry{ &_load_buffer.at(bypassable_load_entry_index) };
				data_t write_data{DCache::read(bypassable_load_entry->calculated_address) };
				//write to the physical register file
				Register_File::write(bypassable_load_entry->register_id, write_data);
				// make the rob entry ready
				Reorder_Buffer::set_ready(bypassable_load_entry->reorder_buffer_entry_index);
				// deallocate it from the load buffer
				Execution_Result result{ 
					true,
					write_data, // will be needed in forwarding to reservation stations
					bypassable_load_entry->reorder_buffer_entry_index, // wont be used by common data bus since we already set the readiness here
					bypassable_load_entry->producer_tag, // will be needed in forwarding logic 
					bypassable_load_entry->register_id  // this wont be needed as we write to the register file here
				};
				_load_buffer.erase(_load_buffer.begin() + bypassable_load_entry_index);
				return result;
			}
			size_t forwaradable_load_entry_index{};
			const Buffer_Entry* store_buffer_entry_that_is_forwarded_from{};
			for (size_t i{}; i < _load_buffer.size(); i++) {
				for (size_t j{}; j < _store_buffer.size(); j++) {
					if (_load_buffer[i].calculated_address == _store_buffer[j].calculated_address) {
						store_buffer_entry_that_is_forwarded_from = &_store_buffer[j];
					}
				}
				if (store_buffer_entry_that_is_forwarded_from) {
					forwaradable_load_entry_index = i;
					break;
				}
			}
			if (!store_buffer_entry_that_is_forwarded_from)
				return; // we cant forward anything
			const Buffer_Entry* forwaradable_load_entry{ &_load_buffer[forwaradable_load_entry_index] };
			Register_File::write(forwaradable_load_entry->register_id, store_buffer_entry_that_is_forwarded_from->register_data);
			Reorder_Buffer::set_ready(forwaradable_load_entry->reorder_buffer_entry_index);
			Execution_Result result(
				true,
				store_buffer_entry_that_is_forwarded_from->register_data, // will be needed in forwarding to reservation stations
				forwaradable_load_entry->reorder_buffer_entry_index, // wont be used by common data bus since we already set the readiness here
				forwaradable_load_entry->producer_tag, // will be needed in forwarding logic 
				forwaradable_load_entry->register_id  // this wont be needed as we write to the register file here
			);
			_load_buffer.erase(_load_buffer.begin() + forwaradable_load_entry_index);

			return result;
		}

	} // namespace Core
} // namespace OoOVis