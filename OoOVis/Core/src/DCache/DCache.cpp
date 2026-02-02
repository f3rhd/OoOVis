#include <Core/DCache/DCache.h>
namespace OoOVis
{
	namespace Core
	{

		std::unordered_map<memory_addr_t, data_t> DCache::_memory{};
		void DCache::write(memory_addr_t addr,data_t data) {
			_memory.insert_or_assign(addr, data);
		}

		data_t DCache::read(memory_addr_t addr) {
			return _memory[addr];
		}

	}
}

