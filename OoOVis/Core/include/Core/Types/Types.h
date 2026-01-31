#pragma once

#include <cstdint>
#include <unordered_map>
namespace OoOVis
{
	union data_t {
		uint32_t _unsigned;
		int32_t  _signed;
	};
	using u64 = uint64_t;
	using u32 = uint32_t;
	using i32 = int32_t;
	using memory_addr_t = uint32_t;
	using offset_t = int32_t;
	using reg_id_t = uint32_t;
	using label_id_t = uint32_t;
	using branch_instruction_id_t = uint32_t;
}
