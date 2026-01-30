#pragma once
#pragma once

#include <cstdint>

union data_t {
	uint64_t _unsigned;
	int64_t  _signed;
};
using u64 = uint64_t;
using i64 = int64_t;
using u32 = uint32_t;
using i32 = int32_t;
using memory_addr_t = uint64_t;
using offset_t = int64_t;
using reg_id_t = uint32_t;
using label_id_t = uint64_t;
using branch_instruction_id_t = uint64_t;
