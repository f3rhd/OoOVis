#pragma  once
auto constexpr REORDER_BUFFER_SIZE{ 64 };
auto constexpr PHYSICAL_REGISTER_FILE_SIZE{ 128 };
auto constexpr REGISTER_ALIAS_TABLE_SIZE{ 32 };
auto constexpr INVALID_REGISTER_ID{ -1 };
auto constexpr RESERVATION_STATION_SIZE{ 6 };
auto constexpr NO_PRODUCER_TAG{ 0 };
auto constexpr FETCH_WIDTH{ 3 };
auto constexpr COMMIT_WIDTH{ 3 };
auto constexpr LOAD_BUFFER_SIZE{ 6 };
auto constexpr STORE_BUFFER_SIZE{ 6 };
auto constexpr BRANCH_SHIFT_REGISTER_SIZE{ 8 };
auto constexpr RESERVATION_STATION_AMOUNT{ 7 };
auto constexpr FORWARDING_DATA_INVALID{ 0 };
unsigned int constexpr FORWARDING_DATA_STATION_DEALLOCATE_ONLY{ 1 << 1 };
unsigned int constexpr FORWARDING_DATA_STATION_DEALLOCATE_AND_WAKEUP{ 1 << 2 };
unsigned int constexpr FORWADING_DATA_FORWARD_ONLY{ 1 << 3 };
unsigned int constexpr LOAD_DOES_NOT_USE_FORWARDING{ 0xFFFFFFFF };
unsigned int constexpr EXECUTABLE_LOAD_DOES_NOT_EXIST{ 0xFFFFFFFF};
