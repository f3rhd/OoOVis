#pragma once
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Constants/Constants.h>
namespace OoOVis
{
	namespace Core {

		std::queue<Reorder_Buffer_Entry> Reorder_Buffer::_buffer;
		size_t Reorder_Buffer::allocate(const Reorder_Buffer_Entry& entry) {
			_buffer.emplace(entry);
			return _buffer.size() - 1;
		}

		bool Reorder_Buffer::full() {
			return _buffer.size() >= REORDER_BUFFER_SIZE;
		}

	}
}
