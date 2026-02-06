#include <Core/Execution/ExecutionUnits.h>
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/DCache/DCache.h>
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Fetch/Fetch.h>
#include <Core/ReservationStation/ReservationStationPool.h>
#include <Core/Constants/Constants.h>
#include <iostream>
#include <format>

namespace OoOVis
{
	namespace Core
	{


		Forwarding_Data Execution_Unit_Adder::execute(const Reservation_Station_Entry* source_entry)
		{
			if (!source_entry)
				return { FORWARDING_DATA_INVALID };
			Forwarding_Data data{};
			data.kind = FORWARDING_DATA_STATION_DEALLOCATE_AND_WAKEUP;
			data.producer_tag = source_entry->self_tag;
			switch (source_entry->mode) {
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_ADD:
				data.produced_data.signed_ = source_entry->src1.signed_ + source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_SUB:
				data.produced_data.signed_ = source_entry->src1.signed_ - source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_AUIPC: // immediate value lives in the source 2
				data.produced_data.unsigned_ = (source_entry->src2.unsigned_ << 20) & 0xFFFFF000 + Fetch_Unit::get_program_counter();
				break;
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_LOAD_UPPER:
				data.produced_data.unsigned_ = (source_entry->src2.unsigned_ << 20) & 0xFFFFF000;
				break;
			// shouldnt happen
			default:
				break;
			}
			Register_File::write(source_entry->destination_register_id, data.produced_data);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			return data;
		}

		Forwarding_Data Execution_Unit_Bitwise::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { FORWARDING_DATA_INVALID };
			Forwarding_Data result{};
			result.kind = FORWARDING_DATA_STATION_DEALLOCATE_AND_WAKEUP;
			result.producer_tag = source_entry->self_tag;
			switch (source_entry->mode) {
			case EXECUTION_UNIT_MODE::BITWISE_AND:
				result.produced_data.signed_ = source_entry->src1.signed_ & source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::BITWISE_XOR:
				result.produced_data.signed_ = source_entry->src1.signed_ | source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::LOGICAL_XOR:
				result.produced_data.signed_ = source_entry->src1.signed_ ^ source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::BITWISE_SHIFT_LEFT_LOGICAL:
				result.produced_data.unsigned_ = source_entry->src1.unsigned_ << source_entry->src2.unsigned_;
				break;
			case EXECUTION_UNIT_MODE::BITWISE_SHIFT_RIGHT_LOGICAL:
				result.produced_data.unsigned_ = source_entry->src1.unsigned_ >> source_entry->src2.unsigned_;
				break;
			case EXECUTION_UNIT_MODE::BITWISE_SHIFT_RIGHT_ARITHMETIC:
				result.produced_data.signed_ = source_entry->src1.signed_ >> source_entry->src2.unsigned_;
				break;
			// shouldnt happen
			default:
				break;
			}

			Register_File::write(source_entry->destination_register_id, result.produced_data);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			return result;
		}

		Forwarding_Data Execution_Unit_Set_Less_Than::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { FORWARDING_DATA_INVALID };
			Forwarding_Data result{};
			result.kind = FORWARDING_DATA_STATION_DEALLOCATE_AND_WAKEUP;
			result.producer_tag = source_entry->self_tag;
			result.produced_data.signed_ = source_entry->src1.signed_ < source_entry->src2.signed_ ? 1 : 0;
			Register_File::write(source_entry->destination_register_id, result.produced_data);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			return result;
		}

		Forwarding_Data Execution_Unit_Multiplier::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { FORWARDING_DATA_INVALID };
			Forwarding_Data result{};
			result.kind = FORWARDING_DATA_STATION_DEALLOCATE_AND_WAKEUP;
			result.producer_tag = source_entry->self_tag;
			switch (source_entry->mode) {
			case EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_SIGNED:
			case EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH:
				result.produced_data.signed_ = source_entry->src1.signed_ * source_entry->src2.signed_;
				break;
				case EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH_SIGNED_UNSIGNED: {
                // MULHSU: src1 is signed, src2 is unsigned
                // We cast to 64-bit to ensure we don't lose the overflow
					int64_t full_product{ static_cast<int64_t>(source_entry->src1.signed_) * static_cast<uint64_t>(source_entry->src2.unsigned_) };
                result.produced_data.signed_ = static_cast<int32_t>(full_product >> 32);
                break;
            }
            case EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH_UNSIGNED: {
                // MULHU: Both src1 and src2 are unsigned
				uint64_t full_product{ static_cast<uint64_t>(source_entry->src1.unsigned_) * static_cast<uint64_t>(source_entry->src2.unsigned_) };
                result.produced_data.unsigned_ = static_cast<uint32_t>(full_product >> 32);
                break;
            }
			// shouldnt happen
			default:
				break;

			}
			Register_File::write(source_entry->destination_register_id, result.produced_data);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			return result;
		}

		Forwarding_Data Execution_Unit_Divider::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { FORWARDING_DATA_INVALID };
			Forwarding_Data result{};
			result.kind = FORWARDING_DATA_STATION_DEALLOCATE_AND_WAKEUP;
			result.producer_tag = source_entry->self_tag;
			switch (source_entry->mode) {
			case EXECUTION_UNIT_MODE::DIVIDER_DIVIDE_SIGNED:
				result.produced_data.signed_ = source_entry->src1.signed_ / source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::DIVIDER_DIVIDE_UNSIGNED:
				result.produced_data.unsigned_ = source_entry->src1.unsigned_ / source_entry->src2.unsigned_;
				break;
			case EXECUTION_UNIT_MODE::DIVIDER_REMAINDER_SIGNED:
				result.produced_data.signed_ = source_entry->src1.signed_ % source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::DIVIDER_REMAINDER_UNSIGNED:
				result.produced_data.unsigned_ = source_entry->src1.unsigned_ % source_entry->src2.unsigned_;
				break;
			// shouldnt happen
			default:
				break;
			}
			Register_File::write(source_entry->destination_register_id, result.produced_data);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			return result;
		}
		std::vector<Execution_Unit_Load_Store::Buffer_Entry> Execution_Unit_Load_Store::_store_buffer{};
		std::vector<Execution_Unit_Load_Store::Buffer_Entry> Execution_Unit_Load_Store::_load_buffer{};
		Forwarding_Data Execution_Unit_Load_Store::buffer_allocation_phase(const Reservation_Station_Entry* source_entry) {

			if (!source_entry)
				return { FORWARDING_DATA_INVALID };
			memory_addr_t address{}; ;
			data_t        register_data{}; ;
			if (source_entry->store_source_register_id != INVALID_REGISTER_ID) {
				if (store_buffer_is_full())
					return { FORWARDING_DATA_INVALID };
				// allocate  the entry in the store buffer
				address = source_entry->src1.signed_ + static_cast<offset_t>(source_entry->destination_register_id);
				register_data = source_entry->src2 ;
				_store_buffer.emplace_back(
					source_entry->instruction_id,
					static_cast<u32>(source_entry->reorder_buffer_entry_index), 
					source_entry->store_source_register_id,
					register_data, 
					address,
					NO_PRODUCER_TAG
				);
#ifdef DEBUG_PRINTS
				std::cout << std::format("Created entry in the store buffer: id:{}, source_id:{}, address:{}\n", source_entry->instruction_id, source_entry->store_source_register_id, address);
#endif
				// tell the rob that address and the data is ready
				Reorder_Buffer::set_ready(static_cast<u32>(source_entry->reorder_buffer_entry_index));
				return {FORWARDING_DATA_STATION_DEALLOCATE_ONLY,0,source_entry->self_tag };
			}
			if (!load_buffer_is_full()) {
				address = source_entry->src1.signed_ + source_entry->src2.signed_;
				_load_buffer.emplace_back(
					source_entry->instruction_id,
					source_entry->destination_register_id,
					source_entry->reorder_buffer_entry_index,
					register_data,
					address,
					source_entry->self_tag
				);
#ifdef DEBUG_PRINTS
				std::cout << std::format("Created entry in the load buffer buffer: id:{}, destination_reg:{}, address:{}\n", source_entry->instruction_id, source_entry->destination_register_id, address);
#endif
				return {FORWARDING_DATA_STATION_DEALLOCATE_ONLY,0,source_entry->self_tag };
			}
			return { FORWARDING_DATA_INVALID };
		}

		std::pair<size_t,size_t> Execution_Unit_Load_Store::find_load_that_is_executable() {
			if (_load_buffer.empty()) return { EXECUTABLE_LOAD_DOES_NOT_EXIST,LOAD_DOES_NOT_USE_FORWARDING };
			// maybe store buffer is empty?
			if (_store_buffer.empty()) {
				return { 0,LOAD_DOES_NOT_USE_FORWARDING };
			}
			// maybe we can bypass?
			size_t bypassable_load_entry_index{};
			bool can_bypass{ false };
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
			if (can_bypass)
				return { bypassable_load_entry_index,LOAD_DOES_NOT_USE_FORWARDING };

			// maybe we can forward anything?
			size_t forwaradable_load_entry_index{EXECUTABLE_LOAD_DOES_NOT_EXIST};
			size_t store_buffer_entry_index_that_is_forwarded_from{LOAD_DOES_NOT_USE_FORWARDING};
			for (size_t i{}; i < _load_buffer.size(); i++) {
				for (size_t j{}; j < _store_buffer.size(); j++) {
					if (_load_buffer[i].calculated_address == _store_buffer[j].calculated_address) {
						store_buffer_entry_index_that_is_forwarded_from = j;
					}
				}
				if (store_buffer_entry_index_that_is_forwarded_from != LOAD_DOES_NOT_USE_FORWARDING) {
					return { i,store_buffer_entry_index_that_is_forwarded_from };
				}
			}

			return { EXECUTABLE_LOAD_DOES_NOT_EXIST,LOAD_DOES_NOT_USE_FORWARDING };
		}
		Forwarding_Data Execution_Unit_Load_Store::execute_load() {

			auto executable_load_index(find_load_that_is_executable());
			if (executable_load_index.first != EXECUTABLE_LOAD_DOES_NOT_EXIST) {

				if (executable_load_index.second == LOAD_DOES_NOT_USE_FORWARDING) { // it is a bypassable load
					const Buffer_Entry* bypassable_load_entry{ &_load_buffer.at(executable_load_index.first) };
					data_t write_data{DCache::read(bypassable_load_entry->calculated_address) };
					//write to the physical register file
					Register_File::write(bypassable_load_entry->register_id, write_data);
					// make the rob entry ready
					Reorder_Buffer::set_ready(bypassable_load_entry->reorder_buffer_entry_index);
					Forwarding_Data result{ 
						true,
						write_data, // will be needed in forwarding to reservation stations
						bypassable_load_entry->producer_tag, // will be needed in forwarding logic 
					};
#ifdef DEBUG_PRINTS
					std::cout << std::format("Load instruction {} was executed using bypasssing.\n", bypassable_load_entry->self_id);
#endif
					_load_buffer.erase(_load_buffer.begin() + executable_load_index.first);
					return result;
				}
				if (executable_load_index.second != LOAD_DOES_NOT_USE_FORWARDING) {

					const Buffer_Entry* forwaradable_load_entry{ &_load_buffer[executable_load_index.first] };
					const Buffer_Entry* store_buffer_entry_that_is_forwarded_from{ &_store_buffer[executable_load_index.second] };
					Register_File::write(forwaradable_load_entry->register_id, store_buffer_entry_that_is_forwarded_from->register_data);
					Reorder_Buffer::set_ready(forwaradable_load_entry->reorder_buffer_entry_index);
					Forwarding_Data result(
						true,
						store_buffer_entry_that_is_forwarded_from->register_data, // will be needed in forwarding to reservation stations
						forwaradable_load_entry->producer_tag // will be needed in forwarding logic 
					);
#ifdef DEBUG_PRINTS
					std::cout << std::format("Load instruction {} was executed using forwarding from store instruction {}.\n", forwaradable_load_entry->self_id,store_buffer_entry_that_is_forwarded_from->self_id);
#endif
					_load_buffer.erase(_load_buffer.begin() +  executable_load_index.first);
					return result;
				}
			}
			return { FORWARDING_DATA_INVALID }; // we cant do anything
		}

		void Execution_Unit_Load_Store::execute_store(memory_addr_t store_id) {
			std::vector<size_t> commited_stores{};
			for (size_t i{}; i < _store_buffer.size(); i++) {
				if (_store_buffer[i].self_id == store_id) {
					DCache::write(_store_buffer[i].calculated_address, _store_buffer[i].register_data);
					commited_stores.push_back(i);
				}
			}
			for (auto j : commited_stores) {
#ifdef DEBUG_PRINTS
				std::cout << std::format("Store instruction {} wrote its data to the memory.\n",_store_buffer[j].self_id),
#endif
				_store_buffer.erase(_store_buffer.begin() + j);
			}
		}

		Forwarding_Data Execution_Unit_Branch::execute(const Reservation_Station_Entry* source_entry) {

			if (!source_entry)
				return { false };
			memory_addr_t target_address{};
			bool actual_taken{ false };
			switch (source_entry->mode) {
			case EXECUTION_UNIT_MODE::BRANCH_UNCONDITIONAL:
				target_address = source_entry->src1.unsigned_ + source_entry->src2.unsigned_;
				if (!Fetch_Unit::has_btb_entry(source_entry->instruction_id)) {
					Fetch_Unit::create_btb_entry(source_entry->instruction_id, target_address);
					Fetch_Unit::set_program_counter(target_address);
				} 
				Register_File::write(source_entry->destination_register_id, { source_entry->instruction_id + 1 });
				Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
				return {
					true,
					source_entry->instruction_id + 1, // will be needed in forwarding to reservation stations
					source_entry->self_tag, // will be needed in forwarding logic 
				};
				break;
			case EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_EQUAL:
				actual_taken = source_entry->src1.signed_ == source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_NOT_EQUAL:
				actual_taken = source_entry->src1.signed_ != source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_LESS_THAN:
				actual_taken = source_entry->src1.signed_ < source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_GREATER_THAN:
				actual_taken = source_entry->src1.signed_ > source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_LESS_THAN_UNSIGNED:
				actual_taken = source_entry->src1.unsigned_ < source_entry->src2.unsigned_;
				break;
			case EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_GREATER_THAN_UNSIGNED:
				actual_taken = source_entry->src1.unsigned_ > source_entry->src2.unsigned_;
				break;
			// wont happen
			default:
				break;

			}
			// update pht
			Fetch_Unit::update_pattern_history_table(source_entry->instruction_id, actual_taken);

			target_address = source_entry->branch_target;
			bool prediction{ Fetch_Unit::get_prediction(source_entry->instruction_id) };
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			if (prediction == actual_taken) {
				Reorder_Buffer::set_branch_evaluation(source_entry->reorder_buffer_entry_index, false);
			}
			else { // misprediction recovery

				if (actual_taken == true && prediction == false) {
					Fetch_Unit::set_program_counter(target_address);
					Reservation_Station_Pool::flush(source_entry->instruction_id);
				}
				else {

					Fetch_Unit::set_program_counter(source_entry->instruction_id + 1);
					Reservation_Station_Pool::flush(source_entry->branch_target);
				}
				Reorder_Buffer::set_branch_evaluation(source_entry->reorder_buffer_entry_index, true);
			}
			return { FORWARDING_DATA_STATION_DEALLOCATE_ONLY };
		}

	} // namespace Core
} // namespace OoOVis