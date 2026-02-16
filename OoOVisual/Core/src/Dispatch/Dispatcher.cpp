#include <Core/Dispatch/Dispatcher.h>
#include <Core/RegisterManager/RegisterManager.h>
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
            DISPATCH_FEEDBACK feedback{};
            RESERVATION_STATION_ID station_id{};
            if (!fetch_element.instruction) feedback = DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH;
            else if (Reorder_Buffer::full()) {
                feedback =  DISPATCH_FEEDBACK::REORDER_BUFFER_WAS_FULL;
            }
            else {
				switch ((*fetch_element.instruction)->flow()) {
				case FrontEnd::FLOW_TYPE::REGISTER:
                    station_id = RESERVATION_STATION_ID::ADD_SUB;
					feedback = dispatch_register_instruction(fetch_element, Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID::ADD_SUB));
                    break;
				case FrontEnd::FLOW_TYPE::LOAD:
                    station_id = RESERVATION_STATION_ID::LOAD_STORE;
					feedback = dispatch_load_instruction(fetch_element, Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID::LOAD_STORE));
                    break;
				case FrontEnd::FLOW_TYPE::STORE:
                    station_id = RESERVATION_STATION_ID::LOAD_STORE;
					feedback =  dispatch_store_instruction(fetch_element, Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID::LOAD_STORE));
                    break;
				case FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL:
                    station_id = RESERVATION_STATION_ID::BRANCH;
					feedback =  dispatch_branch_instruction(fetch_element, Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID::BRANCH));
                    break;
				case FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL:
                    station_id = RESERVATION_STATION_ID::BRANCH;
					feedback =  dispatch_jump_instruction(fetch_element, Reservation_Station_Pool::get_reservation_station(RESERVATION_STATION_ID::BRANCH));
                    break;
				case FrontEnd::FLOW_TYPE::UNKNOWN: // @HandleThis
				default:
					std::cout << "Faced with unknown instruction flow type\n";
					exit(EXIT_FAILURE); // @VisitLater : Make the termination of the program cleaner.
				}
            }
            
            _station_dispatch_map.at(station_id) = feedback;
            return feedback;
        }
		DISPATCH_FEEDBACK Dispatcher::dispatch_register_instruction(const Fetch_Element& fetch_element, Reservation_Station& station) {
            const auto& instruction = *fetch_element.instruction;
            const auto& instruction_id = fetch_element.address;
            if (const auto* register_instruction{ dynamic_cast<FrontEnd::Register_Instruction*>(instruction.get()) }) {
                if (register_instruction->dest_reg() == 0)
                    return DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH;
                if (station.full()) 
                    return DISPATCH_FEEDBACK::RESERVATION_STATION_WAS_FULL;
                if (Register_Manager::full()) return DISPATCH_FEEDBACK::REGISTER_FILE_WAS_FULL;

                Reservation_Station_Entry temp_reservation_station_entry;
                switch (register_instruction->instruction_type()) {
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::ADD:
					temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::ADD_SUB_UNIT_ADD ;
					break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SUB:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::ADD_SUB_UNIT_SUB;
						break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::LOAD_UPPER:
					temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::ADD_SUB_UNIT_LOAD_UPPER;
					break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::AUIPC:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::ADD_SUB_UNIT_AUIPC;
					break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::DIV:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::DIVIDER_DIVIDE_SIGNED ;
                     break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::DIVU:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::DIVIDER_DIVIDE_UNSIGNED ;
                     break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::REM:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::DIVIDER_REMAINDER_SIGNED ;
                     break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::REMU:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::DIVIDER_REMAINDER_UNSIGNED ;
                     break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MUL:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_SIGNED ;
                     break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MULH:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH ;
                     break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MULHSU:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH_SIGNED_UNSIGNED ;
                     break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MULHU:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::MULTIPLIER_MULTIPLY_HIGH_UNSIGNED ;
                     break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::AND:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BITWISE_AND;
					break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::OR:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BITWISE_XOR;
					break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::XOR:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::LOGICAL_XOR;
                     break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SLL:
                    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BITWISE_SHIFT_LEFT_LOGICAL;
					break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SRL:
				    temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BITWISE_SHIFT_RIGHT_LOGICAL;
					break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SRA:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::BITWISE_SHIFT_RIGHT_ARITHMETIC;
                     break;
				case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SLT:
					 temp_reservation_station_entry.mode = EXECUTION_UNIT_MODE::SET_LESS_THAN;
                     break;
				default: // shouldn't happen
					break;
                }
                Physical_Register_File_Entry entry(Register_Manager::read_with_alias(register_instruction->src1_reg()));
				temp_reservation_station_entry.producer_tag1 = entry.producer_tag;
				temp_reservation_station_entry.src1 = entry.data;
                // store the old alias before renaming
                auto old_alias{ Register_Manager::aliasof(register_instruction->dest_reg()) };
                Reservation_Station_Entry* allocated_reservation_station_entry{ station.allocate_entry() }; 
                if (register_instruction->uses_immval()) {
                    temp_reservation_station_entry.src2.signed_ = register_instruction->src2().imm_val;
                    temp_reservation_station_entry.producer_tag2 = Constants::NO_PRODUCER_TAG;
                }
                else {
					temp_reservation_station_entry.src2 = Register_Manager::read_with_alias(register_instruction->src2().src2_reg).data;
                    temp_reservation_station_entry.producer_tag2 = Register_Manager::read_with_alias(register_instruction->src2().src2_reg).producer_tag;
                }
                reg_id_t destination_physical_register_id{ Register_Manager::allocate_physical_register_for(register_instruction->dest_reg(), allocated_reservation_station_entry->self_tag) }; // we do the fulness checking before calling this, so we good
                // allocate reorder buffer before renaming the destination register, this function call is invoked in the case where Reorder_Buffer is not full
                auto target_reorder_buffer_entry_index{ Reorder_Buffer::allocate(
						std::make_unique<Register_Reorder_Buffer_Entry>(
							register_instruction->flow(),
							register_instruction->dest_reg(),
							old_alias,
							destination_physical_register_id,
                            fetch_element.timestamp
						)
					) 
                };
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
            if (const auto* load_instruction{ dynamic_cast<FrontEnd::Load_Instruction*>(instruction.get()) }) {
                if (load_instruction->dest_reg() == 0)
                    return DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH;
                if (station.full())
                    return DISPATCH_FEEDBACK::RESERVATION_STATION_WAS_FULL;
                if (Register_Manager::full())
                    return DISPATCH_FEEDBACK::REGISTER_FILE_WAS_FULL;
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
                Physical_Register_File_Entry entry(Register_Manager::read_with_alias(load_instruction->base_reg()));
                temp_reservation_station_entry.src1 = entry.data;
                temp_reservation_station_entry.producer_tag1 = entry.producer_tag;
                temp_reservation_station_entry.destination_register_id = load_instruction->dest_reg();
                temp_reservation_station_entry.src2.signed_ = (load_instruction->offset());
                temp_reservation_station_entry.producer_tag2 = Constants::NO_PRODUCER_TAG;
                temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == Constants::NO_PRODUCER_TAG && temp_reservation_station_entry.producer_tag2 == Constants::NO_PRODUCER_TAG;

                auto old_alias{ Register_Manager::aliasof(load_instruction->dest_reg()) };
                Reservation_Station_Entry* allocated_reservation_station_entry{ station.allocate_entry() };
			    reg_id_t destination_physical_register_id = Register_Manager::allocate_physical_register_for(load_instruction->dest_reg(),allocated_reservation_station_entry->self_tag); // allocate reorder buffer before renaming the destination register, this function call is invoked in the case where Reorder_Buffer is not full
                auto target_reorder_buffer_entry_index{ Reorder_Buffer::allocate(
                    std::make_unique<Register_Reorder_Buffer_Entry>(
                        load_instruction->flow(),
                        load_instruction->dest_reg(),
                        old_alias,
                        destination_physical_register_id,
						fetch_element.timestamp
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
                Physical_Register_File_Entry src1_entry(Register_Manager::read_with_alias(store_instruction->src1()));
                Physical_Register_File_Entry src2_entry(Register_Manager::read_with_alias(store_instruction->src2()));
                Reservation_Station_Entry* allocated_reservation_station(station.allocate_entry());
                temp_reservation_station_entry.producer_tag1 = src1_entry.producer_tag;
                temp_reservation_station_entry.producer_tag2 = src2_entry.producer_tag;
                temp_reservation_station_entry.src1 = src1_entry.data;
                temp_reservation_station_entry.src2 = src2_entry.data;
                temp_reservation_station_entry.destination_register_id = static_cast<reg_id_t>(store_instruction->offset()); // the offset lives in destination id here
                temp_reservation_station_entry.destination_register_id_as_ofsset = true;
                temp_reservation_station_entry.store_source_register_id = Register_Manager::aliasof(store_instruction->src2());
                temp_reservation_station_entry.self_tag = allocated_reservation_station->self_tag;
                temp_reservation_station_entry.busy = true;
                temp_reservation_station_entry.ready = temp_reservation_station_entry.producer_tag1 == Constants::NO_PRODUCER_TAG && temp_reservation_station_entry.producer_tag2 == Constants::NO_PRODUCER_TAG;
                temp_reservation_station_entry.timestamp = fetch_element.timestamp;
                // when the store instruction is in the store buffer it will wait a signal from reorder buffer to finish its execution architecturally, that is why we need store_id
                temp_reservation_station_entry.reorder_buffer_entry_index = static_cast<u32>(Reorder_Buffer::allocate(
						std::make_unique<Store_Reorder_Buffer_Entry>(
							store_instruction->flow(),
                            fetch_element.timestamp,
                            fetch_element.timestamp
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
                        fetch_element.timestamp
                    )
                );
				Physical_Register_File_Entry entry1(Register_Manager::read_with_alias(branch_instruction->src1()));
				Physical_Register_File_Entry entry2(Register_Manager::read_with_alias(branch_instruction->src2()));
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
                reg_id_t old_alias = Register_Manager::aliasof(jump_instruction->dest_reg());
                Physical_Register_File_Entry entry{ Register_Manager::read_with_alias(jump_instruction->src1()) };
				reg_id_t destination_pysical_register_id = Register_Manager::allocate_physical_register_for(jump_instruction->dest_reg(), allocated_reservation_station_entry->self_tag);
				temp_reservation_station_entry.destination_register_id = destination_pysical_register_id;
                u64 target_reorder_buffer_entry_index{};
                switch (jump_instruction->kind()) {
                case FrontEnd::Jump_Instruction::JUMP_INSTRUCTION_TYPE::JALR:
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

        std::vector<DISPATCH_FEEDBACK> 
        Dispatcher::_last_dispatch_feedback{ std::vector<DISPATCH_FEEDBACK>(Constants::FETCH_WIDTH,DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH) };

		const std::vector<DISPATCH_FEEDBACK>&  Dispatcher::last_dispatch_feedback() {
            return _last_dispatch_feedback;
		}

		DISPATCH_FEEDBACK Dispatcher::dispatch_feedback_of(RESERVATION_STATION_ID id) {
            return _station_dispatch_map.at(id);
		}

		std::map<RESERVATION_STATION_ID, DISPATCH_FEEDBACK> Dispatcher::_station_dispatch_map{
            {RESERVATION_STATION_ID::ADD_SUB,DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH},
            {RESERVATION_STATION_ID::BITWISE,DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH},
            {RESERVATION_STATION_ID::SET_LESS,DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH},
            {RESERVATION_STATION_ID::MULTIPLIER,DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH},
            {RESERVATION_STATION_ID::DIVIDER,DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH},
            {RESERVATION_STATION_ID::LOAD_STORE,DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH},
            {RESERVATION_STATION_ID::BRANCH,DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH},
        };
		std::vector<DISPATCH_FEEDBACK> Dispatcher::dispatch_fetch_group() {
            std::vector<DISPATCH_FEEDBACK> feedback(Constants::FETCH_WIDTH, DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH);
            if (Fetch_Group::group.empty()) {
                _last_dispatch_feedback = feedback;
                return feedback;
            }
            for (size_t i{}; i < Constants::FETCH_WIDTH; i++) {
                feedback[i] = dispatch_fetch_element(Fetch_Group::group[i]);
                if (
                    feedback[i] != DISPATCH_FEEDBACK::NO_INSTRUCTION_TO_DISPATCH &&
                    feedback[i] != DISPATCH_FEEDBACK::SUCCESSFUL_DISPATCH
                ) {
                    break;
                }
            }
			_last_dispatch_feedback = feedback;
            return feedback;
        }
     
	} // namespace Core
    
} // namespace OoOVis
