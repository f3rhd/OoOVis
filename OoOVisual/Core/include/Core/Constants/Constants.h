#pragma  once
namespace OoOVisual {
	namespace Core {
		namespace Constants {
				auto constexpr REORDER_BUFFER_SIZE{ 64 };
				auto constexpr PHYSICAL_REGISTER_FILE_SIZE{ 128 };
				auto constexpr ARCHITECTURAL_REGISTER_AMOUNT{ 32 };
				auto constexpr REGISTER_ALIAS_TABLE_SIZE{ 32 };
				auto constexpr INVALID_PHYSICAL_REGISTER_ID{ PHYSICAL_REGISTER_FILE_SIZE };
				auto constexpr RESERVATION_STATION_SIZE{ 6 };
				auto constexpr NO_PRODUCER_TAG{ 0 };
				auto constexpr FETCH_WIDTH{ 3 };
				auto constexpr COMMIT_WIDTH{ 3 };
				auto constexpr LOAD_BUFFER_SIZE{ 6 };
				auto constexpr STORE_BUFFER_SIZE{ 6 };
				auto constexpr BRANCH_SHIFT_REGISTER_SIZE{ 8 };
				auto constexpr RESERVATION_STATION_AMOUNT{ 7 };
				auto constexpr FORWARDING_DATA_INVALID{ 0 };
				auto constexpr UNIT_TIME{ 10ull };
				auto constexpr RESET{ "\033[0m" };
				auto constexpr RED{ "\033[31m" };
				auto constexpr GREEN{ "\033[32m" };
				auto constexpr YELLOW{ "\033[33m" };
				auto constexpr BLUE{ "\033[34m" };
				auto constexpr MAGENTA{ "\033[35m" };
				auto constexpr CYAN{ "\033[36m" };
				unsigned int constexpr FORWARDING_DATA_STATION_DEALLOCATE_ONLY{ 1 << 1 };
				unsigned int constexpr FORWARDING_DATA_STATION_DEALLOCATE_AND_FORWARD{ 1 << 2 };
				unsigned int constexpr FORWADING_DATA_FORWARD_ONLY{ 1 << 3 };
				unsigned int constexpr NOT_BRANCH_INSTRUCTION{ 0 };
				unsigned int constexpr PREDICTED_TAKEN{ 1 << 1 };
				unsigned int constexpr PREDICTED_NOT_TAKEN{ 1 << 2 };
				unsigned int constexpr LOAD_DOES_NOT_USE_FORWARD_FROM_STORE{ 0xFFFFFFFF };
				unsigned int constexpr EXECUTABLE_LOAD_DOES_NOT_EXIST{ 0xFFFFFFFF};
				unsigned int constexpr TIME_ZERO{ 0 }; // remember that no instruction will ever have a timestamp 0
		} // namespace Constants
	} // namespace Core
} // namespace OoOVis
