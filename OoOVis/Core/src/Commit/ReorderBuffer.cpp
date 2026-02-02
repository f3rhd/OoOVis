#pragma once
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Constants/Constants.h>
#include <iostream>
namespace OoOVis
{
	namespace Core {

		std::vector<std::unique_ptr<Reorder_Buffer_Entry>> Reorder_Buffer::_buffer{};
		size_t Reorder_Buffer::_head{};
		size_t Reorder_Buffer::_tail{};
		size_t Reorder_Buffer::allocate(std::unique_ptr<Reorder_Buffer_Entry>&& entry) {
			_buffer.emplace_back(std::move(entry));
			_tail = (_tail + 1) % REORDER_BUFFER_SIZE;
			return _buffer.size() - 1;
		}

		void Reorder_Buffer::set_ready(u64 target_entry_index) {
			if (target_entry_index >= REORDER_BUFFER_SIZE) {
				std::cout << "Tried to access non-existing reorder buffer entry.\n";
				exit(EXIT_FAILURE);
			}
			_buffer[target_entry_index]->ready = true;
		}

		void Reorder_Buffer::set_branch_evaluation(u64 target_entry_index, bool mispredicted)
		{
			if (target_entry_index >= REORDER_BUFFER_SIZE) {
				std::cout << "Tried to access non-existing reorder buffer entry.\n";
				exit(EXIT_FAILURE);
			}
			_buffer[target_entry_index]->ready = true;
			if (auto* branch_entry = dynamic_cast<Branch_Conditional_Reorder_Buffer_Entry*>(_buffer[target_entry_index].get())) {
				branch_entry->mispredicted = mispredicted;
			}
			else {
				std::cout << "Tried to access branch reorder buffer with wrong index.\n";
				exit(EXIT_FAILURE);
			}

		}

		bool Reorder_Buffer::full() {
			return (_tail + 1) % REORDER_BUFFER_SIZE == _head;
		}

	}
}
