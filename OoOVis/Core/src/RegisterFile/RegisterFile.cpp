#pragma once
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/Constants/Constants.h>
#include <stdexcept>
#include <iostream>
namespace OoOVis
{
	namespace Core
	{
		std::unordered_map<reg_id_t, Physical_Register_File_Entry> Register_File::_physical_register_file{};
		std::unordered_map<reg_id_t, reg_id_t> Register_File::_register_alias_table{};

		void Register_File::init() {
			for (reg_id_t i = 0; i < PHYSICAL_REGISTER_FILE_SIZE; i++) {
				_physical_register_file.emplace(i, Physical_Register_File_Entry());
			}
			for (reg_id_t j = 0; j < REGISTER_ALIAS_TABLE_SIZE; j++) {
				_register_alias_table.emplace(j, j);
			}
		}

		bool Register_File::full() {
			for (auto const& [key, entry] : _physical_register_file) {
				if (!entry.allocated) return true;
			}
			return false;
		}

		void Register_File::deallocate(reg_id_t physical_register_id) {
			if (physical_register_id == INVALID_REGISTER_ID) {
				std::cout << "Tried to deallocate invalid register.\n"; 
				exit(EXIT_FAILURE); // @VisitLater
			}
				
			if (!_physical_register_file[physical_register_id].allocated) {
				std::cout << "Tried to deallocate an already deallocated register.\n";
				exit(EXIT_FAILURE); // @VisitLater
			}
			_physical_register_file[physical_register_id].allocated = false;
		}

		void Register_File::write(reg_id_t physical_register_id, data_t data) {
			if (physical_register_id == INVALID_REGISTER_ID) {
				std::cout << "Tried to write to an invalid register.\n";
				exit(EXIT_FAILURE); // @VisitLater
			}
			_physical_register_file[physical_register_id].data = data;
			_physical_register_file[physical_register_id].producer_tag = NO_PRODUCER_TAG;
		}

		reg_id_t Register_File::allocate_physical_register_for(reg_id_t architectural_register_id, u32 producer_tag) {
			for (auto& [key, entry] : _physical_register_file) {
				if (!entry.allocated) {
					_register_alias_table[architectural_register_id] = key;
					entry.allocated = true;
					entry.producer_tag = producer_tag;
					return key;
				}
			}
			return INVALID_REGISTER_ID;
		}

		reg_id_t Register_File::aliasof(reg_id_t architectural_register_id)
		{
			if (architectural_register_id == INVALID_REGISTER_ID) {
				std::cout << "Tried to read invalid register.";
				exit(EXIT_FAILURE); // @VisitLater
			}
			return _register_alias_table[architectural_register_id];
		}

		Physical_Register_File_Entry Register_File::read_with_alias(reg_id_t architectural_register_id) {
			if (architectural_register_id == 0) {
				return {};
			}
			return _physical_register_file[aliasof(architectural_register_id)];
		}
	}
}
