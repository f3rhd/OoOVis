#include <Core/DCache/DCache.h>
#include <iostream>
#include <format>
namespace OoOVisual
{
	namespace Core
	{

		std::unordered_map<memory_addr_t, u8> DCache::_memory{};
		void DCache::write(EXECUTION_UNIT_MODE mode, memory_addr_t addr, data_t data) {
#ifdef DEBUG_PRINTS
			std::cout << std::format("DCache[{}] <- {}\n", addr, data.signed_);
#endif
			u8 first{ static_cast<u8>(data.unsigned_ & 0xFF) };
			u8 second{ static_cast<u8>((data.unsigned_ >> 8) & 0xFF) };
			u8 third{ static_cast<u8>((data.unsigned_ >> 16) & 0xFF) };
			u8 forth{ static_cast<u8>((data.unsigned_ >> 24) & 0xFF) };
			// we work with little endian
			switch (mode) {
			case EXECUTION_UNIT_MODE::LOAD_STORE_STORE_WORD:
				_memory.insert_or_assign(addr, first);
				_memory.insert_or_assign(addr + 1, second);
				_memory.insert_or_assign(addr + 2, third);
				_memory.insert_or_assign(addr + 3, forth);
				break;
			case EXECUTION_UNIT_MODE::LOAD_STORE_STORE_HALF:
				_memory.insert_or_assign(addr, first);
				_memory.insert_or_assign(addr + 1, second);
				break;
			case EXECUTION_UNIT_MODE::LOAD_STORE_STORE_BYTE:
				_memory.insert_or_assign(addr, first);
				break;
			default:
				break;

			}
		}

		data_t DCache::read(EXECUTION_UNIT_MODE mode, memory_addr_t addr) {
#ifdef DEBUG_PRINTS
			std::cout << std::format("Accessed DCache[{}]\n", addr);
#endif

			data_t result{};

			switch (mode) {
			case EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_WORD:
				result.unsigned_ = (static_cast<u32>(_memory[addr])) |
					(static_cast<u32>(_memory[addr + 1]) << 8) |
					(static_cast<u32>(_memory[addr + 2]) << 16) |
					(static_cast<u32>(_memory[addr + 3]) << 24);
				break;

			case EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_HALF:
			{
				i16 half((static_cast<u16>(_memory[addr])) |
					(static_cast<u16>(_memory[addr + 1]) << 8));
				result.signed_ = static_cast<i32>(half);
			}
			break;

			case EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_BYTE:
			{
				i8 byte(static_cast<i8>(_memory[addr]));
				result.signed_ = static_cast<i32>(byte);
			}
			break;

			case EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_BYTE_UNSIGNED:
				result.unsigned_ = static_cast<u32>(_memory[addr]);
				break;

			case EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_HALF_UNSIGNED:
				result.unsigned_ = (static_cast<u32>(_memory[addr])) |
					(static_cast<u32>(_memory[addr + 1]) << 8);
				break;

			default:
				break;
			}

			return result;

		}
		void DCache::reset()
		{
			_memory = {};
		}
	} // namespace Core
} // namespace OoOVis

