#pragma once
#include <Core/Types/Types.h>
#include <unordered_map>
namespace OoOVis 
{
	namespace Core
	{
		auto constexpr PHYSICAL_REGISTER_FILE_SIZE = 128;
		auto constexpr REGISTER_ALIAS_TABLE_SIZE = 32;
		auto constexpr INVALID_REGISTER_ID = 255;
		auto constexpr NO_PRODUCER_TAG = 0;
		struct Physical_Register_File_Entry {
			data_t data = { 0 };
			u32 producer_tag = NO_PRODUCER_TAG;
			bool allocated = false;
			Physical_Register_File_Entry() = default;
		};
		struct Register_File {
			static void							init();
			static bool							full();
			static void							dealloacte(reg_id_t physical_register_id);
			static void                         write(reg_id_t physical_register_id,data_t data);
			static reg_id_t						allocate_physical_register_for(reg_id_t architectural_register_id,u32 producer_tag); 
			static reg_id_t						aliasof(reg_id_t architectural_register_id);
			static Physical_Register_File_Entry read(reg_id_t architectural_register_id);
		private:
			static std::unordered_map<reg_id_t, Physical_Register_File_Entry> _physical_register_file;
			static std::unordered_map<reg_id_t, reg_id_t>					  _latest_register_alias_table;
		};

	} // namespace Core
} // namespace OoOVis
