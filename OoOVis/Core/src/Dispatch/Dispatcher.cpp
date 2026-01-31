#include <Core/Dispatch/Dispatcher.h>
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Constants/Constants.h>
#include <iostream>
#include <unordered_map>
namespace OoOVis
{
    namespace Core
    {
		void Dispatcher::dispatch_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, std::unordered_map<u32,Reservation_Station>& station_pool) {
            switch (instruction->flow()) {
            case FrontEnd::FLOW_TYPE::REGISTER:
                dispatch_register_instruction(instruction, station_pool.at(static_cast<u32>(RESERVATION_STATION_ID::ADD_SUB)));
                break;
            case FrontEnd::FLOW_TYPE::LOAD:
                dispatch_load_instruction(instruction, station_pool.at(static_cast<u32>(RESERVATION_STATION_ID::LOAD_STORE)));
                break;
            case FrontEnd::FLOW_TYPE::STORE:
                dispatch_store_instruction(instruction, station_pool.at(static_cast<u32>(RESERVATION_STATION_ID::LOAD_STORE)));
                break;
            case FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL:
                dispatch_branch_instruction(instruction,station_pool.at(static_cast<u32>(RESERVATION_STATION_ID::BRANCH)));
                break;
            case FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL:
                dispatch_jump_instruction(instruction, station_pool.at(static_cast<u32>(RESERVATION_STATION_ID::BRANCH)));
                break;
            case FrontEnd::FLOW_TYPE::UNKNOWN: // @HandleThis
            default:
                std::cout << "Faced with unknown instruction flow type\n";
				exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
                return ; // @HandleThis
            }
        }
		void Dispatcher::dispatch_register_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {
            if (const auto* register_instruction = dynamic_cast<FrontEnd::Register_Instruction*>(instruction.get())) {
                auto info = get_reservation_station_id_and_execution_mode_for(instruction);
                Reservation_Station_Entry temp_reservation_station_entry;
                temp_reservation_station_entry.mode = info.second;
                Physical_Register_File_Entry entry;
                try {

					entry = Register_File::read(register_instruction->src1_reg());
                }
                catch (const std::runtime_error& err) {
                    std::cout << err.what();
					exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
                    return;
                }
                temp_reservation_station_entry.producer_tag1 = entry.producer_tag;
                temp_reservation_station_entry.producer_tag2 = NO_PRODUCER_TAG;
                temp_reservation_station_entry.src1 = entry.data;
                // allocate reorder buffer before renaming the destination register, this function call is invoked in the case where Reorder_Buffer is not full
                auto target_reorder_buffer_entry_index = Reorder_Buffer::allocate(
                    std::make_unique<Register_Reorder_Buffer_Entry>(
                        register_instruction->flow(),
                        Register_File::aliasof(register_instruction->dest_reg())
                    )
                );
                Reservation_Station_Entry* allocated_reservation_station_entry = station.allocate_entry(); // We do the fullness checking before calling this function
                // store the old alias before renaming
                Register_File::allocate_physical_register_for(register_instruction->dest_reg(), allocated_reservation_station_entry->self_tag); // we do the fulness checking before calling this, so we good
                if (register_instruction->uses_immval()) {
                    temp_reservation_station_entry.src2 ._signed = register_instruction->src2().imm_val;
                }
                else {
                    temp_reservation_station_entry.src2 = Register_File::read(register_instruction->src2().src2_reg).data;
                }
                temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == NO_PRODUCER_TAG && temp_reservation_station_entry.producer_tag2 == NO_PRODUCER_TAG;
                temp_reservation_station_entry.busy = true;
                temp_reservation_station_entry.self_tag = allocated_reservation_station_entry->self_tag;
                temp_reservation_station_entry.reorder_buffer_entry_index = target_reorder_buffer_entry_index;
                *allocated_reservation_station_entry = temp_reservation_station_entry;
            }
            // again shouldnt happen but...
            // @HandleThis
            else {
                std::cout << "Dynamic cast of Instruction -> Register_Instrution failed.\n";
				exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
            }
        }
        void Dispatcher::dispatch_load_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {
            if (const auto* load_instruction = dynamic_cast<FrontEnd::Load_Instruction*>(instruction.get())) {
                Reservation_Station_Entry temp_reservation_station_entry;
                switch (load_instruction->kind()) {
                case FrontEnd::Load_Instruction::LOAD_INSTRUCTION_TYPE::LB:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_BYTE;
                    break;
                case FrontEnd::Load_Instruction::LOAD_INSTRUCTION_TYPE::LH:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_HALF;
                    break;
                case FrontEnd::Load_Instruction::LOAD_INSTRUCTION_TYPE::LW:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_WORD;
                    break;
                case FrontEnd::Load_Instruction::LOAD_INSTRUCTION_TYPE::LBU:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_BYTE_UNSIGNED;
                    break;
                case FrontEnd::Load_Instruction::LOAD_INSTRUCTION_TYPE::LHU:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_HALF_UNSIGNED;
                    break;
                case FrontEnd::Load_Instruction::LOAD_INSTRUCTION_TYPE::UNKNOWN:
                default:
                    // shouldnt happen
                    std::cout << "Faced with unkown load instruction type while dispatching.\n";
					exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
                    break;

                }
                Physical_Register_File_Entry entry;
                try {
                    entry = Register_File::read(load_instruction->base_reg());
                }
                catch (const std::runtime_error& err) {
                    std::cout << err.what();
					exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
                }
                temp_reservation_station_entry.src1 = entry.data;
                temp_reservation_station_entry.producer_tag1 = entry.producer_tag;
                temp_reservation_station_entry.src2._signed = { load_instruction->offset() }; 
                temp_reservation_station_entry.producer_tag2 = NO_PRODUCER_TAG;
                temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == NO_PRODUCER_TAG && temp_reservation_station_entry.producer_tag2 == NO_PRODUCER_TAG;

                // allocate reorder buffer before renaming the destination register, this function call is invoked in the case where Reorder_Buffer is not full
                auto target_reorder_buffer_entry_index = Reorder_Buffer::allocate(
                    std::make_unique<Register_Reorder_Buffer_Entry>(
                        load_instruction->flow(),
                        Register_File::aliasof(load_instruction->dest_reg())
                    )
                );
                Reservation_Station_Entry* allocated_reservation_station_entry = station.allocate_entry();
			    Register_File::allocate_physical_register_for(load_instruction->dest_reg(),allocated_reservation_station_entry->self_tag);
                temp_reservation_station_entry.self_tag = allocated_reservation_station_entry->self_tag;
                temp_reservation_station_entry.busy = true;
                temp_reservation_station_entry.reorder_buffer_entry_index = target_reorder_buffer_entry_index;
                *allocated_reservation_station_entry = temp_reservation_station_entry;

            }
            // shouldnt happennnn but again..
            // @Handle this
            else {
                std::cout << "Dynamic cast of Instruction -> Load_Instruction failed\n";
                exit(EXIT_FAILURE);

            }

        }
        void Dispatcher::dispatch_store_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {
            if (const auto* store_instruction = dynamic_cast<FrontEnd::Store_Instruction*>(instruction.get())) {
                static u32 store_id = 0;
                Reservation_Station_Entry temp_reservation_station_entry;
                switch (store_instruction->kind()) {
                case FrontEnd::Store_Instruction::STORE_INSTRUCTION_TYPE::SB:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::LOAD_STORE_STORE_BYTE;
                    break;
                case FrontEnd::Store_Instruction::STORE_INSTRUCTION_TYPE::SH:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::LOAD_STORE_STORE_HALF;
                    break;
                case FrontEnd::Store_Instruction::STORE_INSTRUCTION_TYPE::SW:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::LOAD_STORE_STORE_WORD;
                    break;
				// shouldnt happen
                case FrontEnd::Store_Instruction::STORE_INSTRUCTION_TYPE::UNKNOWN:
                default:
                    std::cout << "Faced with an unknown store instruction type while dispatching.\n";
					exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
                    break;

                }
                Physical_Register_File_Entry src1_entry;
                Physical_Register_File_Entry src2_entry;
                try {
                    src1_entry = Register_File::read(store_instruction->src1());
                    src2_entry = Register_File::read(store_instruction->src2());
                }
                catch (std::runtime_error& err) {
                    std::cout << err.what();
					exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
                }
                Reservation_Station_Entry* allocated_reservation_station = station.allocate_entry();
                temp_reservation_station_entry.producer_tag1 = src1_entry.producer_tag;
                temp_reservation_station_entry.producer_tag2 = src2_entry.producer_tag;
                temp_reservation_station_entry.src1 = src1_entry.data;
                temp_reservation_station_entry.src2 = src2_entry.data;
                temp_reservation_station_entry.self_tag = allocated_reservation_station->self_tag;
                temp_reservation_station_entry.busy = true;
                temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == NO_PRODUCER_TAG && temp_reservation_station_entry.producer_tag2 == NO_PRODUCER_TAG;
                // when the store instruction is in the store buffer it will wait a signal from reorder buffer to finish its execution architecturally, that is why we need store_id
                temp_reservation_station_entry.reorder_buffer_entry_index = static_cast<u32>(Reorder_Buffer::allocate(
						std::make_unique<Store_Reorder_Buffer_Entry>(
							store_instruction->flow(),
							store_id
						)
					)
                );
                temp_reservation_station_entry.store_id = store_id; // this id will be passed down to the entry in the store buffer
                store_id++;
                *allocated_reservation_station = temp_reservation_station_entry;
            }
            else {
                std::cout << "Dynamic cast of Instruction -> Store_Instruction failed\n";
                exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
            }
        }
        void Dispatcher::dispatch_branch_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {
            if (const auto* branch_instruction = dynamic_cast<FrontEnd::Branch_Instruction*>(instruction.get())) {

                Reservation_Station_Entry* allocated_reservation_station_entry = station.allocate_entry();
                Reservation_Station_Entry temp_reservation_station_entry;
                temp_reservation_station_entry.busy = true;
                temp_reservation_station_entry.self_tag = allocated_reservation_station_entry->self_tag;
                temp_reservation_station_entry.reorder_buffer_entry_index = Reorder_Buffer::allocate(
                    std::make_unique<Branch_Conditional_Reorder_Buffer_Entry>(
                        branch_instruction->flow()
                    )
                );
                try {
					Physical_Register_File_Entry entry1(Register_File::read(branch_instruction->src1()));
					Physical_Register_File_Entry entry2(Register_File::read(branch_instruction->src2()));
					temp_reservation_station_entry.producer_tag1 = entry1.producer_tag;
                    temp_reservation_station_entry.producer_tag2 = entry2.producer_tag;
					temp_reservation_station_entry.src1 = entry1.data;
                    temp_reservation_station_entry.src2 = entry2.data;
					temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == NO_PRODUCER_TAG && temp_reservation_station_entry.producer_tag2 == NO_PRODUCER_TAG;
                    // execution stage will need these for branch instruction
                    temp_reservation_station_entry.branch_id = branch_instruction->id();
                    temp_reservation_station_entry.branch_target = branch_instruction->target_label();
                    temp_reservation_station_entry.fallthrough = branch_instruction->fallthrough();

                }
                catch (std::runtime_error& err) {
                    std::cout << err.what();
					exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
                }
            }
            else {
                std::cout << "Dynamic cast of Instruction -> Branch_Instruction failed\n";
                exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
            }

        }
        void Dispatcher::dispatch_jump_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {

            if (const auto* jump_instruction = dynamic_cast<FrontEnd::Jump_Instruction*>(instruction.get())) {

                Reservation_Station_Entry* allocated_reservation_station_entry = station.allocate_entry();
                Reservation_Station_Entry temp_reservation_station_entry;
                temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BRANCH_UNCONDITIONAL;
                temp_reservation_station_entry.self_tag = allocated_reservation_station_entry->self_tag;
                temp_reservation_station_entry.busy = true;
				Register_File::allocate_physical_register_for(jump_instruction->dest_reg(), allocated_reservation_station_entry->self_tag);
                Physical_Register_File_Entry entry;
                u64 target_reorder_buffer_entry_index{};
                switch (jump_instruction->kind()) {
                case FrontEnd::Jump_Instruction::JUMP_INSTRUCTION_TYPE::JALR:
                    try {
                        entry = Register_File::read(jump_instruction->src1());
                    }
                    catch (std::runtime_error& err) {
                        std::cout << err.what();
						exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
                    }
                    temp_reservation_station_entry.producer_tag1 = entry.producer_tag;
                    temp_reservation_station_entry.src1 = entry.data;
                    temp_reservation_station_entry.producer_tag2 = NO_PRODUCER_TAG;
                    temp_reservation_station_entry.src2._signed = jump_instruction->offset();
                    temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == NO_PRODUCER_TAG;
					target_reorder_buffer_entry_index = Reorder_Buffer::allocate(
						std::make_unique<Branch_Unconditional_Reorder_Buffer_Entry>(
							jump_instruction->flow(),
                            true
						)
					);
					temp_reservation_station_entry.reorder_buffer_entry_index = target_reorder_buffer_entry_index;
                    break;
                case FrontEnd::Jump_Instruction::JUMP_INSTRUCTION_TYPE::JAL:
                    temp_reservation_station_entry.ready = true;
					target_reorder_buffer_entry_index = Reorder_Buffer::allocate(
						std::make_unique<Branch_Unconditional_Reorder_Buffer_Entry>(
							jump_instruction->flow(),
                            false
						)
					);
                    temp_reservation_station_entry.reorder_buffer_entry_index = target_reorder_buffer_entry_index;
                    temp_reservation_station_entry.src1._unsigned = jump_instruction->target_label();
                    break;
                default:
					std::cout << "Faced with unknown jump instruction type while dispatching.\n";
					exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
                    break;
                }
				*allocated_reservation_station_entry = temp_reservation_station_entry;
            }
            else {
                std::cout << "Dynamic cast of Instruction -> Jump_Instruction failed\n";
                exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
            }

        }

		std::pair<RESERVATION_STATION_ID, EXECUTION_UNIT_MODE> Dispatcher::get_reservation_station_id_and_execution_mode_for(const std::unique_ptr<FrontEnd::Instruction>& instruction){
            switch (instruction->flow())
            {
            case FrontEnd::FLOW_TYPE::LOAD:
                return { RESERVATION_STATION_ID::LOAD_STORE,EXECUTION_UNIT_MODE::LOAD_STORE_LOAD_WORD }; // this is the default
            case FrontEnd::FLOW_TYPE::STORE:
                return { RESERVATION_STATION_ID::LOAD_STORE,EXECUTION_UNIT_MODE::LOAD_STORE_STORE_WORD }; // this is the default
            case FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL:
                return { RESERVATION_STATION_ID::BRANCH,EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL };
            case FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL:
                return { RESERVATION_STATION_ID::BRANCH,EXECUTION_UNIT_MODE::BRANCH_UNCONDITIONAL };
            case FrontEnd::FLOW_TYPE::REGISTER:
                if (const auto* register_instr = dynamic_cast<const FrontEnd::Register_Instruction*>(instruction.get())) {
                    switch (register_instr->instruction_type()) {
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::ADD:
                            return { RESERVATION_STATION_ID::ADD_SUB,EXECUTION_UNIT_MODE::ADD_SUB_UNIT_ADD };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SUB:
                            return { RESERVATION_STATION_ID::ADD_SUB,EXECUTION_UNIT_MODE::ADD_SUB_UNIT_SUB };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::LOAD_UPPER:
                            return { RESERVATION_STATION_ID::ADD_SUB,EXECUTION_UNIT_MODE::ADD_SUB_UNIT_LOAD_UPPER };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::AUIPC:
                            return { RESERVATION_STATION_ID::ADD_SUB,EXECUTION_UNIT_MODE::ADD_SUB_UNIT_AUIPC };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::DIV:
                            return { RESERVATION_STATION_ID::DIVIDER,EXECUTION_UNIT_MODE::DIVIDER_DIVIDE_SIGNED };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::DIVU:
                            return { RESERVATION_STATION_ID::DIVIDER,EXECUTION_UNIT_MODE::DIVIDER_DIVIDE_UNSIGNED };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::REM:
                            return { RESERVATION_STATION_ID::DIVIDER,EXECUTION_UNIT_MODE::DIVIDER_REMAINDER_SIGNED };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::REMU:
                            return { RESERVATION_STATION_ID::DIVIDER,EXECUTION_UNIT_MODE::DIVIDER_REMAINDER_UNSIGNED };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MUL:
                            return { RESERVATION_STATION_ID::DIVIDER,EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_SIGNED };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MULH:
                            return { RESERVATION_STATION_ID::DIVIDER,EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MULHSU:
                            return { RESERVATION_STATION_ID::DIVIDER,EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH_SIGNED_UNSIGNED };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MULHU:
                            return { RESERVATION_STATION_ID::DIVIDER,EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH_UNSIGNED };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::AND:
                            return { RESERVATION_STATION_ID::LOGICAL,EXECUTION_UNIT_MODE::LOGICAL_AND };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::OR:
                            return { RESERVATION_STATION_ID::LOGICAL,EXECUTION_UNIT_MODE::LOGICAL_OR };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::XOR:
                            return { RESERVATION_STATION_ID::LOGICAL,EXECUTION_UNIT_MODE::LOGICAL_XOR };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SLL:
                            return { RESERVATION_STATION_ID::LOGICAL,EXECUTION_UNIT_MODE::LOGICAL_SHIFT_LEFT_LOGICAL };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SRL:
                            return { RESERVATION_STATION_ID::LOGICAL,EXECUTION_UNIT_MODE::LOGICAL_SHIFT_RIGHT_LOGICAL };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SRA:
                            return { RESERVATION_STATION_ID::LOGICAL,EXECUTION_UNIT_MODE::LOGICAL_SHIFT_RIGHT_ARITHMETIC };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SLT:
                            return { RESERVATION_STATION_ID::SET_LESS,EXECUTION_UNIT_MODE::SET_LESS_THAN };
                        default: // shouldn't happen
                            break;
                    }
                }
                return { RESERVATION_STATION_ID::UNKNOWN,EXECUTION_UNIT_MODE::UNKNOWN };
                break;
            default:
                return { RESERVATION_STATION_ID::UNKNOWN,EXECUTION_UNIT_MODE::UNKNOWN };
                break;
            }
			}
     
	} // namespace Core
    
} // namespace OoOVis
