#include <Core/DCache/DCache.h>
#include <iostream>
#include <format>
namespace OoOVis
{
	namespace Core
	{

		std::unordered_map<memory_addr_t, data_t> DCache::_memory{};
		void DCache::write(memory_addr_t addr,data_t data) {
#ifdef DEBUG_PRINTS
			std::cout << std::format("DCache[{}] <- {}\n", addr,data.signed_);
#endif
			_memory.insert_or_assign(addr, data);
		}

		data_t DCache::read(memory_addr_t addr) {
#ifdef DEBUG_PRINTS
			std::cout << std::format("Accessed DCache[{}]\n", addr);
#endif
			return _memory[addr];
		}

	}
}

