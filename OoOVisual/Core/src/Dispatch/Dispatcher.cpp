#include <Core/Dispatch/Dispatcher.h>
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Constants/Constants.h>
#include <Core/ReservationStation/ReservationStationPool.h>
#include <iostream>
#include <unordered_map>
#include <format>
namespace OoOVisual
{
    namespace Core
    {
		DISPATCH_FEEDBACK Dispatcher::dispatch_fetch_element(const Fetch_Element& fetch_element) {
            if (Reorder_Buffer::full())
                return DISPATCH_FEEDBACK::REORDER_BUFFER_WAS_FULL;
            if (!fetch_element.instruction)
                return DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH;
            switch ((*fetch_element.instruction)->flow()) {
            case FrontEnd::FLOW_TYPE::REGISTER:
                return dispatch_register_instruction(fetch_element, Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID::ADD_SUB));
            case FrontEnd::FLOW_TYPE::LOAD:
                return dispatch_load_instruction(fetch_element, Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID::LOAD_STORE));
            case FrontEnd::FLOW_TYPE::STORE:
                return dispatch_store_instruction(fetch_element, Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID::LOAD_STORE));
            case FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL:
                return dispatch_branch_instruction(fetch_element, Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID::BRANCH));
            case FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL:
                return dispatch_jump_instruction(fetch_element, Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID::BRANCH));
            case FrontEnd::FLOW_TYPE::UNKNOWN: // @HandleThis
            default:
                std::cout << "Faced with unknown instruction flow type\n";
                exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
            }
            
        }
		DISPATCH_FEEDBACK Dispatcher::dispatch_register_instruction(const Fetch_Element& fetch_element, Reservation_Station& station) {
            const auto& instruction = *fetch_element.instruction;
            const auto& instruction_id = fetch_element.address;
            const auto& speculation_id = fetch_element.origin_branch_timestamp;
            if (const auto* register_instruction{ dynamic_cast<FrontEnd::Register_Instruction*>(instruction.get()) }) {
                if (station.full()) 
                    return DISPATCH_FEEDBACK::RESERVATION_STATION_WAS_FULL;
                if (Register_File::full()) return DISPATCH_FEEDBACK::REORDER_BUFFER_WAS_FULL;
                if (register_instruction->dest_reg() == 0)
                    return DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH;
                auto info = get_reservation_station_id_and_execution_mode_for_register_instruction(instruction);
                Reservation_Station_Entry temp_reservation_station_entry;
                temp_reservation_station_entry.mode = info.second;
                Physical_Register_File_Entry entry(Register_File::read_with_alias(register_instruction->src1_reg()));

				temp_reservation_station_entry.producer_tag1 = entry.producer_tag;
				temp_reservation_station_entry.src1 = entry.data;
                // store the old alias before renaming
                auto old_alias{ Register_File::aliasof(register_instruction->dest_reg()) };
                Reservation_Station_Entry* allocated_reservation_station_entry{ station.allocate_entry() }; 
                reg_id_t destination_physical_register_id{ Register_File::allocate_physical_register_for(register_instruction->dest_reg(), allocated_reservation_station_entry->self_tag) }; // we do the fulness checking before calling this, so we good
                // allocate reorder buffer before renaming the destination register, this function call is invoked in the case where Reorder_Buffer is not full
                auto target_reorder_buffer_entry_index{ Reorder_Buffer::allocate(
						std::make_unique<Register_Reorder_Buffer_Entry>(
							register_instruction->flow(),
							register_instruction->dest_reg(),
							old_alias,
							destination_physical_register_id,
                            speculation_id
						)
					) 
                };
                if (register_instruction->uses_immval()) {
                    temp_reservation_station_entry.src2.signed_ = register_instruction->src2().imm_val;
                    temp_reservation_station_entry.producer_tag2 = Constants::NO_PRODUCER_TAG;
                }
                else {
					temp_reservation_station_entry.src2 = Register_File::read_with_alias(register_instruction->src2().src2_reg).data;
                    temp_reservation_station_entry.producer_tag2 = Register_File::read_with_alias(register_instruction->src2().src2_reg).producer_tag;
                }
				temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == Constants::NO_PRODUCER_TAG && temp_reservation_station_entry.producer_tag2 == Constants::NO_PRODUCER_TAG;
                temp_reservation_station_entry.busy = true;
                temp_reservation_station_entry.self_tag = allocated_reservation_station_entry->self_tag;
                temp_reservation_station_entry.reorder_buffer_entry_index = target_reorder_buffer_entry_index;
                temp_reservation_station_entry.destination_register_id = destination_physical_register_id;
                temp_reservation_station_entry.instruction_address = instruction_id;
                temp_reservation_station_entry.timestamp = fetch_element.timestamp;
				#ifdef DEBUG_PRINTS
                std::cout << std::format("Dispatched Instructions[{}] timestamp : {} to ReservationStationPool[{}].\n", instruction_id,temp_reservation_station_entry.timestamp,temp_reservation_station_entry.self_tag);
				#endif
                *allocated_reservation_station_entry = temp_reservation_station_entry;
            }
            // again shouldnt happen but...
            // @HandleThis
            else {
                std::cout << "Dynamic cast of Instruction -> Register_Instruction failed.\n";
				exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
            }        
            return DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH;
        }
        DISPATCH_FEEDBACK Dispatcher::dispatch_load_instruction(const Fetch_Element& fetch_element, Reservation_Station& station) {
            const auto& instruction = *fetch_element.instruction;
            const auto& instruction_id = fetch_element.address;
            const auto& speculation_id = fetch_element.origin_branch_timestamp;
            if (const auto* load_instruction{ dynamic_cast<FrontEnd::Load_Instruction*>(instruction.get()) }) {
                if (station.full())
                    return DISPATCH_FEEDBACK::RESERVATION_STATION_WAS_FULL;
                if (Register_File::full())
                    return DISPATCH_FEEDBACK::REGISTER_FILE_WAS_FULL;
                if (load_instruction->dest_reg() == 0)
                    return DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH;
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
                Physical_Register_File_Entry entry(Register_File::read_with_alias(load_instruction->base_reg()));
                temp_reservation_station_entry.src1 = entry.data;
                temp_reservation_station_entry.producer_tag1 = entry.producer_tag;
                temp_reservation_station_entry.destination_register_id = load_instruction->dest_reg();
                temp_reservation_station_entry.src2.signed_ = (load_instruction->offset());
                temp_reservation_station_entry.producer_tag2 = Constants::NO_PRODUCER_TAG;
                temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == Constants::NO_PRODUCER_TAG && temp_reservation_station_entry.producer_tag2 == Constants::NO_PRODUCER_TAG;

                auto old_alias{ Register_File::aliasof(load_instruction->dest_reg()) };
                Reservation_Station_Entry* allocated_reservation_station_entry{ station.allocate_entry() };
			    reg_id_t destination_physical_register_id = Register_File::allocate_physical_register_for(load_instruction->dest_reg(),allocated_reservation_station_entry->self_tag);
                // allocate reorder buffer before renaming the destination register, this function call is invoked in the case where Reorder_Buffer is not full
                auto target_reorder_buffer_entry_index{ Reorder_Buffer::allocate(
                    std::make_unique<Register_Reorder_Buffer_Entry>(
                        load_instruction->flow(),
                        load_instruction->dest_reg(),
                        old_alias,
                        destination_physical_register_id,
                        speculation_id
                    )
                )
                };
                temp_reservation_station_entry.self_tag = allocated_reservation_station_entry->self_tag;
                temp_reservation_station_entry.busy = true;
                temp_reservation_station_entry.reorder_buffer_entry_index = target_reorder_buffer_entry_index;
                temp_reservation_station_entry.destination_register_id = destination_physical_register_id;
                temp_reservation_station_entry.instruction_address = instruction_id;
                temp_reservation_station_entry.timestamp = fetch_element.timestamp;
                *allocated_reservation_station_entry = temp_reservation_station_entry;
				#ifdef DEBUG_PRINTS
                std::cout << std::format("Dispatched Instructions[{}] timestamp : {} to ReservationStationPool[{}].\n", instruction_id,temp_reservation_station_entry.timestamp,temp_reservation_station_entry.self_tag);
				#endif

            }
            // shouldnt happennnn but again..
            else {
                std::cout << "Dynamic cast of Instruction -> Load_Instruction failed\n";
                exit(EXIT_FAILURE);

            }
            return DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH;
        }
        DISPATCH_FEEDBACK Dispatcher::dispatch_store_instruction(const Fetch_Element& fetch_element, Reservation_Station& station) {
            const auto& instruction = *fetch_element.instruction;
            const auto& instruction_id = fetch_element.address;
            const auto& speculation_id = fetch_element.origin_branch_timestamp;
            if (const auto* store_instruction = dynamic_cast<FrontEnd::Store_Instruction*>(instruction.get())) {
                if (station.full())
                    return DISPATCH_FEEDBACK::RESERVATION_STATION_WAS_FULL;
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
                Physical_Register_File_Entry src1_entry(Register_File::read_with_alias(store_instruction->src1()));
                Physical_Register_File_Entry src2_entry(Register_File::read_with_alias(store_instruction->src2()));
                Reservation_Station_Entry* allocated_reservation_station(station.allocate_entry());
                temp_reservation_station_entry.producer_tag1 = src1_entry.producer_tag;
                temp_reservation_station_entry.producer_tag2 = src2_entry.producer_tag;
                temp_reservation_station_entry.src1 = src1_entry.data;
                temp_reservation_station_entry.src2 = src2_entry.data;
                temp_reservation_station_entry.destination_register_id = static_cast<reg_id_t>(store_instruction->offset()); // the offset lives in destination id here
                temp_reservation_station_entry.destination_register_id_as_ofsset = true;
                temp_reservation_station_entry.store_source_register_id = Register_File::aliasof(store_instruction->src2());
                temp_reservation_station_entry.self_tag = allocated_reservation_station->self_tag;
                temp_reservation_station_entry.busy = true;
                temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == Constants::NO_PRODUCER_TAG && temp_reservation_station_entry.producer_tag2 == Constants::NO_PRODUCER_TAG;
                temp_reservation_station_entry.timestamp = fetch_element.timestamp;
                // when the store instruction is in the store buffer it will wait a signal from reorder buffer to finish its execution architecturally, that is why we need store_id
                temp_reservation_station_entry.reorder_buffer_entry_index = static_cast<u32>(Reorder_Buffer::allocate(
						std::make_unique<Store_Reorder_Buffer_Entry>(
							store_instruction->flow(),
                            instruction_id,
                            speculation_id
						)
					)
                );
                temp_reservation_station_entry.instruction_address = instruction_id; // this id will be passed down to the entry in the store buffer
                *allocated_reservation_station = temp_reservation_station_entry;
				#ifdef DEBUG_PRINTS
                std::cout << std::format("Dispatched Instructions[{}] timestamp : {} to ReservationStationPool[{}].\n", instruction_id,temp_reservation_station_entry.timestamp,temp_reservation_station_entry.self_tag);
				#endif
            }
            else {
                std::cout << "Dynamic cast of Instruction -> Store_Instruction failed\n";
                exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
            }        
            return DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH;
        }
        DISPATCH_FEEDBACK Dispatcher::dispatch_branch_instruction(const Fetch_Element& fetch_element, Reservation_Station& station) {
            const auto& instruction = *fetch_element.instruction;
            const auto& instruction_id = fetch_element.address;
            const auto& speculation_id = fetch_element.origin_branch_timestamp;
            if (const auto* branch_instruction{ dynamic_cast<FrontEnd::Branch_Instruction*>(instruction.get()) }) {
                if (station.full())
                    return DISPATCH_FEEDBACK::RESERVATION_STATION_WAS_FULL;
                Reservation_Station_Entry* allocated_reservation_station_entry ( station.allocate_entry());
                Reservation_Station_Entry temp_reservation_station_entry;
                temp_reservation_station_entry.busy = true;
                temp_reservation_station_entry.self_tag = allocated_reservation_station_entry->self_tag;
                temp_reservation_station_entry.reorder_buffer_entry_index = Reorder_Buffer::allocate(
                    std::make_unique<Branch_Conditional_Reorder_Buffer_Entry>(
                        branch_instruction->flow(),
                        speculation_id,
                        fetch_element.timestamp
                    )
                );
				Physical_Register_File_Entry entry1(Register_File::read_with_alias(branch_instruction->src1()));
				Physical_Register_File_Entry entry2(Register_File::read_with_alias(branch_instruction->src2()));
                switch (branch_instruction->kind()) {
                case FrontEnd::Branch_Instruction::BRANCH_INSTRUCTION_TYPE::BEQ:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_EQUAL;
                    break;
                case FrontEnd::Branch_Instruction::BRANCH_INSTRUCTION_TYPE::BNE:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_NOT_EQUAL;
                    break;
                case FrontEnd::Branch_Instruction::BRANCH_INSTRUCTION_TYPE::BGE:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_GREATER_OR_EQUAL_THAN;
                    break;
                case FrontEnd::Branch_Instruction::BRANCH_INSTRUCTION_TYPE::BGEU:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_GREATER_OR_EQUAL_THAN_UNSIGNED;
                    break;
                case FrontEnd::Branch_Instruction::BRANCH_INSTRUCTION_TYPE::BLT:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_LESS_THAN;
                    break;
                case FrontEnd::Branch_Instruction::BRANCH_INSTRUCTION_TYPE::BLTU:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL_LESS_THAN_UNSIGNED;
                    break;
                default:
                    break;
                }
				temp_reservation_station_entry.producer_tag1 = entry1.producer_tag;
				temp_reservation_station_entry.producer_tag2 = entry2.producer_tag;
				temp_reservation_station_entry.src1 = entry1.data;
				temp_reservation_station_entry.src2 = entry2.data;
				temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == Constants::NO_PRODUCER_TAG && temp_reservation_station_entry.producer_tag2 == Constants::NO_PRODUCER_TAG;
				// execution stage will need these for branch instruction
				temp_reservation_station_entry.branch_target = branch_instruction->target_addr();
				temp_reservation_station_entry.instruction_address = instruction_id;
                temp_reservation_station_entry.timestamp = fetch_element.timestamp;
                temp_reservation_station_entry.fetch_unit_prediction = fetch_element.branch_prediction;
				*allocated_reservation_station_entry = temp_reservation_station_entry;
				#ifdef DEBUG_PRINTS
                std::cout << Constants::YELLOW << std::format("Dispatched Instructions[{}] timestamp : {} to ReservationStationPool[{}] Fetch_Unit_Prediction : {}  .\n", instruction_id,temp_reservation_station_entry.timestamp,temp_reservation_station_entry.self_tag,temp_reservation_station_entry.fetch_unit_prediction) <<Constants::RESET;
				#endif
            }
            else {
                std::cout << "Dynamic cast of Instruction -> Branch_Instruction failed\n";
                exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
            }
            return DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH;
        }
        DISPATCH_FEEDBACK Dispatcher::dispatch_jump_instruction(const Fetch_Element& fetch_element, Reservation_Station& station) {

            const auto& instruction = *fetch_element.instruction;
            const auto& instruction_id = fetch_element.address;
            const auto& speculation_id = fetch_element.origin_branch_timestamp;
            if (const auto* jump_instruction = dynamic_cast<FrontEnd::Jump_Instruction*>(instruction.get())) {

                if (station.full())
                    return DISPATCH_FEEDBACK::REORDER_BUFFER_WAS_FULL;
                Reservation_Station_Entry* allocated_reservation_station_entry(station.allocate_entry());
                Reservation_Station_Entry temp_reservation_station_entry;
                switch (jump_instruction->kind()) {
                case FrontEnd::Jump_Instruction::JUMP_INSTRUCTION_TYPE::JALR:
					temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BRANCH_UNCONDITIONAL_JALR;
                    break;
                case FrontEnd::Jump_Instruction::JUMP_INSTRUCTION_TYPE::JAL:
					temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BRANCH_UNCONDITIONAL_JAL;
                    break;
                default: // wont happen
                    break;
                }
                temp_reservation_station_entry.self_tag = allocated_reservation_station_entry->self_tag;
                temp_reservation_station_entry.busy = true;
                temp_reservation_station_entry.timestamp = fetch_element.timestamp;
                reg_id_t old_alias = Register_File::aliasof(jump_instruction->dest_reg());
				reg_id_t destination_pysical_register_id = Register_File::allocate_physical_register_for(jump_instruction->dest_reg(), allocated_reservation_station_entry->self_tag);
				temp_reservation_station_entry.destination_register_id = destination_pysical_register_id;
                Physical_Register_File_Entry entry;
                u64 target_reorder_buffer_entry_index{};
                switch (jump_instruction->kind()) {
                case FrontEnd::Jump_Instruction::JUMP_INSTRUCTION_TYPE::JALR:
					entry = Register_File::read_with_alias(jump_instruction->src1());
                    temp_reservation_station_entry.producer_tag1 = entry.producer_tag;
                    temp_reservation_station_entry.src1 = entry.data;
                    temp_reservation_station_entry.producer_tag2 = Constants::NO_PRODUCER_TAG;
                    temp_reservation_station_entry.src2.signed_ = jump_instruction->offset();
                    temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == Constants::NO_PRODUCER_TAG;
					target_reorder_buffer_entry_index = Reorder_Buffer::allocate(
						std::make_unique<Branch_Unconditional_Reorder_Buffer_Entry>(
							jump_instruction->flow(),
                            jump_instruction->dest_reg(),
                            old_alias,
                            destination_pysical_register_id,
                            true, // always mispredicted
                            speculation_id,
							fetch_element.timestamp
						)
					);
					temp_reservation_station_entry.reorder_buffer_entry_index = target_reorder_buffer_entry_index;
                    break;
                case FrontEnd::Jump_Instruction::JUMP_INSTRUCTION_TYPE::JAL:
                    temp_reservation_station_entry.ready = true;
					target_reorder_buffer_entry_index = Reorder_Buffer::allocate(
						std::make_unique<Branch_Unconditional_Reorder_Buffer_Entry>(
							jump_instruction->flow(),
                            jump_instruction->dest_reg(),
                            old_alias,
                            destination_pysical_register_id,
                            false, // never mispredicted
                            speculation_id,
                            fetch_element.timestamp
						)
					);
                    temp_reservation_station_entry.reorder_buffer_entry_index = target_reorder_buffer_entry_index;
                    break;
                default:
					std::cout << "Faced with unknown jump instruction type while dispatching.\n";
					exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
                    break;
                }
                temp_reservation_station_entry.instruction_address = instruction_id;
				*allocated_reservation_station_entry = temp_reservation_station_entry;
				#ifdef DEBUG_PRINTS
                std::cout << std::format("Dispatched Instructions[{}] timestamp : {} to ReservationStationPool[{}].\n", instruction_id,temp_reservation_station_entry.timestamp,temp_reservation_station_entry.self_tag);
				#endif
            }
            else {
                std::cout << "Dynamic cast of Instruction -> Jump_Instruction failed\n";
                exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
            }
            return DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH;

        }

		std::pair<RESERVATION_STATION_ID, EXECUTION_UNIT_MODE> Dispatcher::get_reservation_station_id_and_execution_mode_for_register_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction){
            switch (instruction->flow())
            {
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
                            return { RESERVATION_STATION_ID::BITWISE,EXECUTION_UNIT_MODE::BITWISE_AND };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::OR:
                            return { RESERVATION_STATION_ID::BITWISE,EXECUTION_UNIT_MODE::BITWISE_XOR };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::XOR:
                            return { RESERVATION_STATION_ID::BITWISE,EXECUTION_UNIT_MODE::LOGICAL_XOR };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SLL:
                            return { RESERVATION_STATION_ID::BITWISE,EXECUTION_UNIT_MODE::BITWISE_SHIFT_LEFT_LOGICAL };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SRL:
                            return { RESERVATION_STATION_ID::BITWISE,EXECUTION_UNIT_MODE::BITWISE_SHIFT_RIGHT_LOGICAL };
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SRA:
                            return { RESERVATION_STATION_ID::BITWISE,EXECUTION_UNIT_MODE::BITWISE_SHIFT_RIGHT_ARITHMETIC };
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

        std::vector<DISPATCH_FEEDBACK> Dispatcher::dispatch_fetch_group()
        {
            std::vector<DISPATCH_FEEDBACK> feedback;
            feedback.reserve(Constants::FETCH_WIDTH);
            if (Fetch_Group::group.empty())
                return std::vector<DISPATCH_FEEDBACK>(Constants::FETCH_WIDTH,DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH);
			for (const auto& fetch_element : Fetch_Group::group) {
                feedback.emplace_back(dispatch_fetch_element(fetch_element));
			}
            return feedback;
        }
     
	} // namespace Core
    
} // namespace OoOVis
