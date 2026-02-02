#pragma  once
auto constexpr REORDER_BUFFER_SIZE{ 64 };
auto constexpr PHYSICAL_REGISTER_FILE_SIZE{ 128 };
auto constexpr REGISTER_ALIAS_TABLE_SIZE{ 32 };
auto constexpr INVALID_REGISTER_ID{ 255 };
auto constexpr RESERVATION_STATION_SIZE{ 6 };
auto constexpr NO_PRODUCER_TAG{ 0 };
auto constexpr FETCH_WIDTH{ 3 };
auto constexpr EXECUTION_WIDTH{ 3 };
auto constexpr COMMIT_WIDTH{ 3 };
auto constexpr NO_STORE{ 0 };
auto constexpr IS_STORE{ PHYSICAL_REGISTER_FILE_SIZE + 1 };
auto constexpr LOAD_BUFFER_SIZE{ 6 };
auto constexpr STORE_BUFFER_SIZE{ 6 };
auto constexpr BRANCH_SHIFT_REGISTER_SIZE{ 8 };
auto constexpr RESERVATION_STATION_AMOUNT{ 7 };
