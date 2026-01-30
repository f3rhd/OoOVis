#pragma once
#include <Core/Commit/ReorderBuffer.h>
namespace OoOVis
{
	namespace Core {

		std::queue<Reorder_Buffer_Entry> Reorder_Buffer::_buffer;
		void Reorder_Buffer::allocate(const Reorder_Buffer_Entry& entry) {
			_buffer.emplace(entry);
		}
	}
}
