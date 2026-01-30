#include <Core/Dispatch/Dispatcher.h>
#include <Core/RegisterFile/RegisterFile.h>
#include <Core/Commit/ReorderBuffer.h>
#include <iostream>
namespace OoOVis
{
    namespace Core
    {
            std::pair<RESERVATION_STATION_ID, EXECUTION_UNIT_MODE> Dispatcher::get_reservation_station_id_and_execution_mode_for(const std::unique_ptr<FrontEnd::Instruction>& instruction){
            switch (instruction->flow_type())
            {
            case FrontEnd::FLOW_TYPE::LOAD:
                return { RESERVATION_STATION_ID::LOAD_STORE,EXECUTION_UNIT_MODE::LOAD_STORE_LOAD };
            case FrontEnd::FLOW_TYPE::STORE:
                return { RESERVATION_STATION_ID::LOAD_STORE,EXECUTION_UNIT_MODE::LOAD_STORE_STORE };
            case FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL:
                return { RESERVATION_STATION_ID::BRANCH,EXECUTION_UNIT_MODE::BRANCH_CONDITIONAL };
            case FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL:
                return { RESERVATION_STATION_ID::BRANCH,EXECUTION_UNIT_MODE::BRANCH_UNCONDITIONAL };
            case FrontEnd::FLOW_TYPE::REGISTER:
                if (const auto* register_instr = dynamic_cast<const FrontEnd::Register_Instruction*>(instruction.get())) {
                    switch (register_instr->register_instruction_type()) {
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
            void Dispatcher::dispatch_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {
            switch (instruction->flow_type()) {
            case FrontEnd::FLOW_TYPE::REGISTER:
                dispatch_register_instruction(instruction, station);
                break;
            case FrontEnd::FLOW_TYPE::LOAD:
                dispatch_load_instruction(instruction, station);
                break;
            case FrontEnd::FLOW_TYPE::STORE:
                dispatch_store_instruction(instruction, station);
                break;
            case FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL:
                dispatch_branch_instruction(instruction, station);
                break;
            case FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL:
                dispatch_jump_instruction(instruction, station);
                break;
            case FrontEnd::FLOW_TYPE::UNKNOWN: // @HandleThis
            default:
                std::cout << "Handle MEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n";
                return ; // @HandleThis
            }
        }
        void Dispatcher::dispatch_register_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {
            if (const auto* instr = dynamic_cast<FrontEnd::Register_Instruction*>(instruction.get())) {
                auto info = get_reservation_station_id_and_execution_mode_for(instruction);
                Reservation_Station_Entry copy_reservation_station_entry;
                copy_reservation_station_entry.mode = info.second;
                Physical_Register_File_Entry entry = Register_File::read(instr->src1_reg());
                copy_reservation_station_entry.tag1 = entry.producer_tag;
                copy_reservation_station_entry.tag2 = NO_PRODUCER_TAG;
                copy_reservation_station_entry.src1 = entry.data;
                Reorder_Buffer::allocate({ Register_File::aliasof(instr->dest_reg()),false });
                Reservation_Station_Entry& allocated_reservation_station_entry = station.allocate_entry(); // We do the fullness checking before calling this function
                reg_id_t allocated_physical_register_id = Register_File::allocate_physical_register_for(instr->dest_reg(), allocated_reservation_station_entry.self_tag);
                if (allocated_physical_register_id == INVALID_REGISTER_ID) {
                    //@HandleThis
                    std::cout << "Handle MEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n";
                }
                if (instr->uses_immval()) {
                    copy_reservation_station_entry.src2 = { static_cast<unsigned long long>(instr->src2().imm_val) };
                }
                else {
                    copy_reservation_station_entry.src2 = Register_File::read(instr->src2().src2_reg).data;
                }
                allocated_reservation_station_entry = copy_reservation_station_entry;
                allocated_reservation_station_entry.busy = true;
            }
            // @HandleThis
            else {
                std::cout << "Handle MEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n";
            }
        }
        void Dispatcher::dispatch_load_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {

        }
        void Dispatcher::dispatch_store_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {

        }
        void Dispatcher::dispatch_branch_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {

        }
        void Dispatcher::dispatch_jump_instruction(const std::unique_ptr<FrontEnd::Instruction>& instruction, Reservation_Station& station) {

        }

      
	} // namespace Core
    
} // namespace OoOVis
