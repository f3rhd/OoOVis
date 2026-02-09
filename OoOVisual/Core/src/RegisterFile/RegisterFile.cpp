#pragma once
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/Constants/Constants.h>
#include <Frontend/Parser/Lookup.h>
#include <stdexcept>
#include <format>
#include <iomanip>
#include <string>
#include <iostream>
#include <format>
namespace OoOVisual
{
	namespace Core
	{
		std::map<reg_id_t, Physical_Register_File_Entry> Register_File::_physical_register_file{};
		std::map<reg_id_t, reg_id_t> Register_File::_frontend_register_alias_table{};
		std::map<reg_id_t, reg_id_t> Register_File::_retirement_alias_table{};

		void Register_File::init() {
			for (reg_id_t i = 0; i < Constants::PHYSICAL_REGISTER_FILE_SIZE; i++) {
				_physical_register_file.emplace(i, Physical_Register_File_Entry());
				if (i >= 0 && i <= 31) {
					_physical_register_file[i].allocated = true;
				}
			}
			for (reg_id_t j = 0; j < Constants::REGISTER_ALIAS_TABLE_SIZE; j++) {
				_frontend_register_alias_table.emplace(j, j);
			}
			_retirement_alias_table = _frontend_register_alias_table;
		}

		bool Register_File::full() {
			for (auto const& [key, entry] : _physical_register_file) {
				if (!entry.allocated) return false;
			}
			return true;
		}


		void Register_File::restore_alias_table() {
			//for (reg_id_t i{}; i < ARCHITECTURAL_REGISTER_AMOUNT; i++) {
			//	if (_snapshot_register_alias_table[i] != _register_alias_table[i]) {
			//		Register_File::deallocate(_register_alias_table[i]);
			//	}
			//	_register_alias_table[i] = _snapshot_register_alias_table[i];
			//}
			_frontend_register_alias_table = _retirement_alias_table;
#ifdef DEBUG_PRINTS
			std::cout << std::format("Register alias table was restored due to mis-prediction.\n");
#endif
		}

		void Register_File::update_retirement_alias_table_with(reg_id_t architectural, reg_id_t physical) {
			_retirement_alias_table.insert_or_assign(architectural, physical);
		}

		void Register_File::deallocate(reg_id_t physical_register_id) {
			if (physical_register_id == 0)
				return;
			if (physical_register_id == Constants::INVALID_PHYSICAL_REGISTER_ID) {
				std::cout << "Tried to deallocate invalid register.\n"; 
				exit(EXIT_FAILURE); // @VisitLater
			}
				
			if (!_physical_register_file[physical_register_id].allocated) {
				std::cout << "Tried to deallocate an already deallocated register.\n";
				exit(EXIT_FAILURE); // @VisitLater
			}
			_physical_register_file[physical_register_id].allocated = false;
			_physical_register_file[physical_register_id].producer_tag = Constants::NO_PRODUCER_TAG;
#ifdef DEBUG_PRINTS
			std::cout << std::format("Register File deallocated P{}.\n", physical_register_id);
#endif
		}

		void Register_File::write(reg_id_t physical_register_id, data_t data) {
			if (physical_register_id == Constants::INVALID_PHYSICAL_REGISTER_ID) {
				std::cout << "Tried to write to an invalid register.\n";
				exit(EXIT_FAILURE); // @VisitLater
			}
			if (physical_register_id == 0) {
				return;
			}
			_physical_register_file[physical_register_id].data = data;
			_physical_register_file[physical_register_id].producer_tag = Constants::NO_PRODUCER_TAG;
#ifdef DEBUG_PRINTS
			std::cout << std::format(
				"PhysicalRegisterFile[{}] <- {}\n",
				physical_register_id,
				data.signed_
			);
#endif
		}

		reg_id_t Register_File::allocate_physical_register_for(reg_id_t architectural_register_id, u32 producer_tag) {
			if (architectural_register_id == 0) {
	#ifdef DEBUG_PRINTS
				std::cout << std::format("Register alias table was updated with zero->P0 due to register allocation.\n");
	#endif
				return 0;
			}
			for (auto it{ _physical_register_file.rbegin() }; it != _physical_register_file.rend(); it++) {
				auto& entry = it->second;
				auto& key = it->first;
				if (!entry.allocated) { // zero->P0 is direct mapping no one can allocate for it except "zero" ofc
					_frontend_register_alias_table[architectural_register_id] = key;
					entry.allocated = true;
					entry.producer_tag = producer_tag;
#ifdef DEBUG_PRINTS
					std::cout << std::format("Register alias table was updated with {}->P{} due to register allocation.\n", FrontEnd::Lookup::reg_name(architectural_register_id), key);
#endif
					return key;
				}
			}
			return Constants::INVALID_PHYSICAL_REGISTER_ID;
		}

		reg_id_t Register_File::aliasof(reg_id_t architectural_register_id)
		{
			if (architectural_register_id == Constants::INVALID_PHYSICAL_REGISTER_ID) {
				std::cout << "Tried to read invalid register.";
				exit(EXIT_FAILURE); // @VisitLater
			}
			return _frontend_register_alias_table[architectural_register_id];
		}

		Physical_Register_File_Entry Register_File::read_with_alias(reg_id_t architectural_register_id) {
			if (architectural_register_id == 0) {
				return {};
			}
			return _physical_register_file[aliasof(architectural_register_id)];
		}
	}
}
