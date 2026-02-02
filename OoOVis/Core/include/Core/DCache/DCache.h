#pragma  once
#include <Core/Types/Types.h>
namespace OoOVis
{
	namespace Core {
		class DCache {
		public:
			static void write(memory_addr_t addr,data_t data);
			static data_t read(memory_addr_t addr);
		private:
			static std::unordered_map<memory_addr_t, data_t> _memory;
		};
	}
}