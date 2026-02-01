#pragma once

#include <cstdint>
#include <unordered_map>
namespace OoOVis
{
	namespace Core {
		class Reservation_Station;
	}
	union data_t {
		uint32_t unsigned_;
		int32_t  signed_;
	};
	using u64 = uint64_t;
	using u32 = uint32_t;
	using i32 = int32_t;
	using memory_addr_t = uint32_t;
	using offset_t = int32_t;
	using reg_id_t = uint32_t;
	using label_id_t = uint32_t;
	using branch_instruction_id_t = uint32_t;
	using reservation_station_pool_t = std::unordered_map<u32, Core::Reservation_Station>;
}
