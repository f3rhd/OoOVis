#include <Core/Execution/ExecutionUnits.h>
#include <Core/RegisterManager/RegisterManager.h>
#include <Core/DCache/DCache.h>
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Fetch/Fetch.h>
#include <Core/ReservationStation/ReservationStationPool.h>
#include <Core/Constants/Constants.h>
#include <iostream>
#include <format>
#include <ranges>

namespace OoOVisual
{
	namespace Core
	{


		Forwarding_Data Execution_Unit_Adder::execute(const Reservation_Station_Entry* source_entry)
		{
			if (!source_entry)
				return { Constants::FORWARDING_DATA_INVALID };
			Forwarding_Data data{};
			data.kind = Constants::FORWARDING_DATA_STATION_DEALLOCATE_AND_FORWARD;
			data.producer_tag = source_entry->self_tag;
			switch (source_entry->mode) {
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_ADD:
				data.produced_data.signed_ = source_entry->src1.signed_ + source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_SUB:
				data.produced_data.signed_ = source_entry->src1.signed_ - source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_AUIPC: // immediate value lives in the source 2
				data.produced_data.unsigned_ = (source_entry->src2.unsigned_ << 20) & 0xFFFFF000 + Fetch_Unit::program_counter();
				break;
			case EXECUTION_UNIT_MODE::ADD_SUB_UNIT_LOAD_UPPER:
				data.produced_data.unsigned_ = (source_entry->src2.unsigned_ << 20) & 0xFFFFF000;
				break;
			// shouldnt happen
			default:
				break;
			}
#ifdef DEBUG_PRINTS
			std::cout << Constants::MAGENTA << std::format("Instructions[{}] timestamp:{} is executing.", source_entry->instruction_address, source_entry->timestamp);
#endif
			Register_Manager::write(source_entry->destination_register_id, data.produced_data);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			return data;
		}

		Forwarding_Data Execution_Unit_Bitwise::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { Constants::FORWARDING_DATA_INVALID };
			Forwarding_Data result{};
			result.kind = Constants::FORWARDING_DATA_STATION_DEALLOCATE_AND_FORWARD;
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

			Register_Manager::write(source_entry->destination_register_id, result.produced_data);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			return result;
		}

		Forwarding_Data Execution_Unit_Set_Less_Than::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { Constants::FORWARDING_DATA_INVALID };
			Forwarding_Data result{};
			result.kind = Constants::FORWARDING_DATA_STATION_DEALLOCATE_AND_FORWARD;
			result.producer_tag = source_entry->self_tag;
			result.produced_data.signed_ = source_entry->src1.signed_ < source_entry->src2.signed_ ? 1 : 0;
			Register_Manager::write(source_entry->destination_register_id, result.produced_data);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			return result;
		}

		Forwarding_Data Execution_Unit_Multiplier::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { Constants::FORWARDING_DATA_INVALID };
			Forwarding_Data result{};
			result.kind = Constants::FORWARDING_DATA_STATION_DEALLOCATE_AND_FORWARD;
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
			Register_Manager::write(source_entry->destination_register_id, result.produced_data);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			return result;
		}

		Forwarding_Data Execution_Unit_Divider::execute(const Reservation_Station_Entry* source_entry) {
			if (!source_entry)
				return { Constants::FORWARDING_DATA_INVALID };
			Forwarding_Data result{};
			result.kind = Constants::FORWARDING_DATA_STATION_DEALLOCATE_AND_FORWARD;
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
			Register_Manager::write(source_entry->destination_register_id, result.produced_data);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			return result;
		}
		std::vector<Execution_Unit_Load_Store::Buffer_Entry> Execution_Unit_Load_Store::_store_buffer{};
		std::vector<Execution_Unit_Load_Store::Buffer_Entry> Execution_Unit_Load_Store::_load_buffer{};
		Forwarding_Data Execution_Unit_Load_Store::buffer_allocation_phase(const Reservation_Station_Entry* source_entry) {

			if (!source_entry)
				return { Constants::FORWARDING_DATA_INVALID };
			memory_addr_t address{}; ;
			data_t        register_data{}; ;
			if (source_entry->store_source_register_id != Constants::INVALID_PHYSICAL_REGISTER_ID) {
				if (store_buffer_is_full())
					return { Constants::FORWARDING_DATA_INVALID };
				// allocate  the entry in the store buffer
				address = source_entry->src1.signed_ + static_cast<offset_t>(source_entry->destination_register_id);
				register_data = source_entry->src2 ;
				_store_buffer.emplace_back(
					source_entry->mode,
					source_entry->timestamp,
					static_cast<u32>(source_entry->reorder_buffer_entry_index), 
					source_entry->store_source_register_id,
					register_data, 
					address,
					Constants::NO_PRODUCER_TAG
				);
#ifdef DEBUG_PRINTS
				std::cout << std::format("Created entry in the store buffer: timestamp:{}, source_id:{}, address:{}\n", source_entry->timestamp, source_entry->store_source_register_id, address);
#endif
				// tell the rob that address and the data is ready
				Reorder_Buffer::set_ready(static_cast<u32>(source_entry->reorder_buffer_entry_index));
				return {Constants::FORWARDING_DATA_STATION_DEALLOCATE_ONLY,0,source_entry->self_tag };
			}
			if (!load_buffer_is_full()) {
				address = source_entry->src1.signed_ + source_entry->src2.signed_;
				_load_buffer.emplace_back(
					source_entry->mode,
					source_entry->timestamp,
					source_entry->destination_register_id,
					source_entry->reorder_buffer_entry_index,
					register_data,
					address,
					source_entry->self_tag
				);
#ifdef DEBUG_PRINTS
				std::cout << std::format("Created entry in the load buffer buffer: timestamp:{}, destination_reg:{}, address:{}\n", source_entry->timestamp, source_entry->destination_register_id, address);
#endif
				return {Constants::FORWARDING_DATA_STATION_DEALLOCATE_ONLY,0,source_entry->self_tag };
			}
			return { Constants::FORWARDING_DATA_INVALID };
		}

		std::pair<size_t,size_t> Execution_Unit_Load_Store::find_load_that_is_executable() {
			if (_load_buffer.empty()) return { Constants::EXECUTABLE_LOAD_DOES_NOT_EXIST,Constants::LOAD_DOES_NOT_USE_FORWARD_FROM_STORE };
			// maybe store buffer is empty?
			if (_store_buffer.empty()) {
				return { 0,Constants::LOAD_DOES_NOT_USE_FORWARD_FROM_STORE }; // select the first entry in the load buffer ( implied by 0 ) 
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
				return { bypassable_load_entry_index,Constants::LOAD_DOES_NOT_USE_FORWARD_FROM_STORE };

			// maybe we can forward anything?
			size_t forwaradable_load_entry_index{Constants::EXECUTABLE_LOAD_DOES_NOT_EXIST};
			size_t store_buffer_entry_index_that_is_forwarded_from{Constants::LOAD_DOES_NOT_USE_FORWARD_FROM_STORE};
			// forward from the latest store instruction that writes to the same address
			for (size_t i{}; i < _load_buffer.size(); i++) {
				time_t max_store_timestamp{};
				for (size_t j{}; j < _store_buffer.size(); j++) {
					if (_load_buffer[i].calculated_address == _store_buffer[j].calculated_address && _store_buffer[j].timestamp >= max_store_timestamp) { 
						// we cant use store_buffer_entry_index_that_is_forwarded_from for comparison since its initial value is max(u32) which will never execute the loop
						store_buffer_entry_index_that_is_forwarded_from = j;
						max_store_timestamp = static_cast<time_t>(j);
					}
				}
				if (store_buffer_entry_index_that_is_forwarded_from != Constants::LOAD_DOES_NOT_USE_FORWARD_FROM_STORE) {
					return { i,store_buffer_entry_index_that_is_forwarded_from };
				}
			}

			return { Constants::EXECUTABLE_LOAD_DOES_NOT_EXIST,Constants::LOAD_DOES_NOT_USE_FORWARD_FROM_STORE };
		}
		Forwarding_Data Execution_Unit_Load_Store::execute_load() {

			auto executable_load_index(find_load_that_is_executable());
			if (executable_load_index.first == Constants::EXECUTABLE_LOAD_DOES_NOT_EXIST)
				return { Constants::FORWARDING_DATA_INVALID };

			if (executable_load_index.second == Constants::LOAD_DOES_NOT_USE_FORWARD_FROM_STORE) { // it is a bypassable load
				const Buffer_Entry* bypassable_load_entry{ &_load_buffer.at(executable_load_index.first) };
				data_t write_data{DCache::read(bypassable_load_entry->mode,bypassable_load_entry->calculated_address) };
				//write to the physical register file
				Register_Manager::write(bypassable_load_entry->register_id, write_data);
				// make the rob entry ready
				Reorder_Buffer::set_ready(bypassable_load_entry->reorder_buffer_entry_index);
				Forwarding_Data result{ 
					Constants::FORWADING_DATA_FORWARD_ONLY, // if we are in the load buffer we are already deallocated from the reservation station 
					write_data, // will be needed in forwarding to reservation stations
					bypassable_load_entry->producer_tag, // will be needed in forwarding logic 
				};
#ifdef DEBUG_PRINTS
				std::cout << std::format("Load instruction {} was executed using bypasssing.\n", bypassable_load_entry->timestamp);
#endif
				_load_buffer.erase(_load_buffer.begin() + executable_load_index.first);
				return result;
			}

			const Buffer_Entry* forwaradable_load_entry{ &_load_buffer[executable_load_index.first] };
			const Buffer_Entry* store_buffer_entry_that_is_forwarded_from{ &_store_buffer[executable_load_index.second] };
			Register_Manager::write(forwaradable_load_entry->register_id, store_buffer_entry_that_is_forwarded_from->register_data);
			Reorder_Buffer::set_ready(forwaradable_load_entry->reorder_buffer_entry_index);
			Forwarding_Data result(
				Constants::FORWADING_DATA_FORWARD_ONLY,
				store_buffer_entry_that_is_forwarded_from->register_data, // will be needed in forwarding to reservation stations
				forwaradable_load_entry->producer_tag // will be needed in forwarding logic 
			);
#ifdef DEBUG_PRINTS
			std::cout << std::format("Load instruction {} was executed using forwarding from store instruction {}.\n", forwaradable_load_entry->timestamp,store_buffer_entry_that_is_forwarded_from->timestamp);
#endif
			_load_buffer.erase(_load_buffer.begin() +  executable_load_index.first);
			return result;
		}

		void Execution_Unit_Load_Store::execute_store(memory_addr_t store_timestamp) {
#ifdef DEBUG_PRINTS
			std::vector<size_t> commited_stores{};
#endif
			for (size_t i{}; i < _store_buffer.size(); i++) {
				if (_store_buffer[i].timestamp == store_timestamp) {
					DCache::write(_store_buffer[i].mode,_store_buffer[i].calculated_address, _store_buffer[i].register_data);
#ifdef DEBUG_PRINTS
					commited_stores.push_back(i);
#endif
				}
			}
#ifdef DEBUG_PRINTS
			for (auto j : commited_stores) {
				std::cout << std::format("Store instruction timestamp : {} wrote its data to the memory.\n", _store_buffer[j].timestamp);
			}
#endif
			std::erase_if(_store_buffer, [&](const auto& a) {return a.timestamp == store_timestamp; });
		}

		time_t Execution_Unit_Load_Store::flush_mispredicted(time_t timestamp) {
			std::vector<time_t> erased_entry_timestamps{};
			std::erase_if(
				_load_buffer, 
				[&](const Execution_Unit_Load_Store::Buffer_Entry& a) { 
					if (a.timestamp > timestamp) {
						erased_entry_timestamps.push_back(a.timestamp);
						return true;
					}
					return false;
				}
			);
			std::erase_if(
				_store_buffer, 
				[&](const Execution_Unit_Load_Store::Buffer_Entry& a) { 
					if (a.timestamp > timestamp) {
						erased_entry_timestamps.push_back(a.timestamp);
						return true;
					}
					return false;
				}
			);
			auto it(std::ranges::max_element(erased_entry_timestamps));
			if (it != erased_entry_timestamps.end())
				return *it;
			return Constants::TIME_ZERO;
		}

		Forwarding_Data Execution_Unit_Branch::execute(const Reservation_Station_Entry* source_entry) {

			if (!source_entry)
				return { Constants::FORWARDING_DATA_INVALID };
			memory_addr_t target_address{};
			bool actual_taken{ false };
			switch (source_entry->mode) { 
			case EXECUTION_UNIT_MODE::BRANCH_UNCONDITIONAL_JALR:// Fetch unit keeps fetching when it sees jalr instruction so we gotta recover
			{
				target_address = source_entry->src1.unsigned_ + source_entry->src2.unsigned_;
				Fetch_Unit::set_program_counter(target_address);
				Fetch_Unit::set_program_counter_flags();
				Register_Manager::write(source_entry->destination_register_id, { source_entry->instruction_address + 1 });
				Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
				time_t latest_flushed_reservation_station_entry_timestamp{ Reservation_Station_Pool::flush_mispredicted(source_entry->self_tag,source_entry->timestamp) };
				time_t latest_flushed_load_store_buffer_entry_timestamp{ Execution_Unit_Load_Store::flush_mispredicted(source_entry->timestamp) };
				Reorder_Buffer::set_branch_evaluation(
					source_entry->reorder_buffer_entry_index,
					true,
					std::max(latest_flushed_reservation_station_entry_timestamp, latest_flushed_load_store_buffer_entry_timestamp)
				);
				Fetch_Group::group = std::vector<Fetch_Element>(Constants::FETCH_WIDTH);
				return {
					Constants::FORWARDING_DATA_STATION_DEALLOCATE_AND_FORWARD,
					source_entry->instruction_address + 1, // will be needed in forwarding to reservation stations
					source_entry->self_tag, // will be needed in forwarding logic 
				};
			}
			break;
			case EXECUTION_UNIT_MODE::BRANCH_UNCONDITIONAL_JAL: // Fetch unit already jumped so all we have to do is write the values to the registers
				Register_Manager::write(source_entry->destination_register_id, { source_entry->instruction_address + 1 });
				Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
				return {
					Constants::FORWARDING_DATA_STATION_DEALLOCATE_AND_FORWARD,
					source_entry->instruction_address + 1, // will be needed in forwarding to reservation stations
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
			case EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_GREATER_OR_EQUAL_THAN:
				actual_taken = source_entry->src1.signed_ >= source_entry->src2.signed_;
				break;
			case EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_GREATER_OR_EQUAL_THAN_UNSIGNED:
				actual_taken = source_entry->src1.unsigned_ >= source_entry->src2.unsigned_;
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
			bool prediction{ false };
			if (source_entry->fetch_unit_prediction & Constants::PREDICTED_TAKEN)
				prediction = true;
			Fetch_Unit::update_pattern_history_table(source_entry->instruction_address, actual_taken);
			target_address = source_entry->branch_target;
			Fetch_Unit::create_btb_entry(source_entry->instruction_address, target_address);
			Reorder_Buffer::set_ready(source_entry->reorder_buffer_entry_index);
			if (prediction == actual_taken) {
				Reorder_Buffer::set_branch_evaluation(source_entry->reorder_buffer_entry_index, false,Constants::TIME_ZERO);
#ifdef DEBUG_PRINTS
				std::cout << Constants::GREEN << "Instructions[" << source_entry->instruction_address << "] timestamp : " << source_entry->timestamp <<  " was predicted correctly\n" << Constants::RESET;
#endif
			}
			else { // misprediction recovery

#ifdef DEBUG_PRINTS
				std::cout << Constants::RED << "Instructions[ " << source_entry->instruction_address << "] timestamp : " << source_entry->timestamp <<  " was mispredicted\n" << Constants::RESET;
#endif
				Fetch_Group::group = std::vector<Fetch_Element>(Constants::FETCH_WIDTH);
				if (actual_taken == true && prediction == false) {
					Fetch_Unit::set_program_counter(target_address);
				}
				else 
					Fetch_Unit::set_program_counter(source_entry->instruction_address + 1);
				Fetch_Unit::set_program_counter_flags();
				time_t latest_flushed_reservation_station_entry_timestamp{ Reservation_Station_Pool::flush_mispredicted(source_entry->self_tag,source_entry->timestamp) };
				time_t latest_flushed_load_store_buffer_entry_timestamp{ Execution_Unit_Load_Store::flush_mispredicted(source_entry->timestamp)};
				Reorder_Buffer::set_branch_evaluation(
					source_entry->reorder_buffer_entry_index,
					true,
					std::max(latest_flushed_reservation_station_entry_timestamp, latest_flushed_load_store_buffer_entry_timestamp)
				);
			}
			return { Constants::FORWARDING_DATA_STATION_DEALLOCATE_ONLY,0,source_entry->self_tag };
		}

	} // namespace Core
} // namespace OoOVis