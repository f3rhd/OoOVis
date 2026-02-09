#pragma once
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Constants/Constants.h>
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/Execution/ExecutionUnits.h>
#include <iostream>
#include <format>
namespace OoOVis
{
	namespace Core {

		std::vector<std::unique_ptr<Reorder_Buffer_Entry>> Reorder_Buffer::_buffer(Constants::REORDER_BUFFER_SIZE);
		size_t Reorder_Buffer::_head{};
		size_t Reorder_Buffer::_tail{};
		size_t Reorder_Buffer::allocate(std::unique_ptr<Reorder_Buffer_Entry>&& entry) {
			_buffer[_tail] = std::move(entry);
			auto allocated_entry_index = _tail;
			_tail = (_tail + 1) % Constants::REORDER_BUFFER_SIZE;
			return allocated_entry_index;
		}

		void Reorder_Buffer::set_ready(u64 target_entry_index) {
			if (target_entry_index >= Constants::REORDER_BUFFER_SIZE) {
				std::cout << "Tried to access non-existing reorder buffer entry.\n";
				exit(EXIT_FAILURE);
			}
			_buffer[target_entry_index]->ready = true;
		}

		void Reorder_Buffer::set_branch_evaluation(u64 target, bool was_misprediction)
		{
			if (target >= Constants::REORDER_BUFFER_SIZE) {
				std::cout << "Tried to access non-existing reorder buffer entry.\n";
				exit(EXIT_FAILURE);
			}
			_buffer[target]->ready = true;
			if (auto* branch_entry = dynamic_cast<Branch_Conditional_Reorder_Buffer_Entry*>(_buffer[target].get())) {
				branch_entry->mispredicted = was_misprediction;
			}
			else {
				std::cout << "Tried to access branch reorder buffer with wrong index.\n";
				exit(EXIT_FAILURE);
			}

		}

		void Reorder_Buffer::commit() {
			for (size_t i{}; i < Constants::COMMIT_WIDTH; i++) {
				auto* entry = _buffer[_head].get();
				if (entry == nullptr) return;
				switch (entry->flow()) {
				case FrontEnd::FLOW_TYPE::REGISTER:
				case FrontEnd::FLOW_TYPE::LOAD:
					if (entry->ready) {
						Register_File::deallocate(dynamic_cast<Register_Reorder_Buffer_Entry*>(entry)->old_alias);
						Register_File::update_retirement_alias_table_with(
							dynamic_cast<Register_Reorder_Buffer_Entry*>(entry)->architectural_register_id,
							dynamic_cast<Register_Reorder_Buffer_Entry*>(entry)->new_alias
						);
						_head++;
					}
					break;
				case FrontEnd::FLOW_TYPE::STORE:
					if (dynamic_cast<Store_Reorder_Buffer_Entry*>(entry)->ready) {
						Execution_Unit_Load_Store::execute_store(dynamic_cast<Store_Reorder_Buffer_Entry*>(entry)->id);
						_buffer[_head].reset();
						_head++;
					}
					break;
				case FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL:
					if (dynamic_cast<Branch_Conditional_Reorder_Buffer_Entry*>(entry)->ready) {
						if (dynamic_cast<Branch_Conditional_Reorder_Buffer_Entry*>(entry)->mispredicted) {
							while (_tail != _head) {
								_buffer[_tail].reset();
								if (_tail > _head)
									_tail--;
								else
									_tail++;
							}
							_buffer[_head].reset();
							_head++;
							_tail = _head;
							Register_File::restore_alias_table();
						}
						else {
							_buffer[_head].reset();
							_head++;
						}
					}
					break;
				default:
					break;

				}
			}
		}
		bool Reorder_Buffer::full() {
			return (_tail + 1)  % Constants::REORDER_BUFFER_SIZE == _head;
		}

	} // namespace Core
} // namespace OoOVis
