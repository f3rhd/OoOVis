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

		bool Reorder_Buffer::full() {
			return (_tail + 1) % REORDER_BUFFER_SIZE == _head;
		}

	}
}
