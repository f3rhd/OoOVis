#include <Core/Dispatch/Dispatch.h>
namespace OoOVis
{
    namespace Core
    {
        RESERVATION_STATION_ID Dispatch::assign_reservation_station_id_to(const std::unique_ptr<FrontEnd::Instruction>& instruction) {
            switch (instruction->flow_type())
            {
            case FrontEnd::FLOW_TYPE::LOAD:
            case FrontEnd::FLOW_TYPE::STORE:
                return RESERVATION_STATION_ID::LOAD_STORE;
            case FrontEnd::FLOW_TYPE::BRANCH_CONDITIONAL:
            case FrontEnd::FLOW_TYPE::BRANCH_UNCONDITIONAL:
                return RESERVATION_STATION_ID::BRANCH;
            case FrontEnd::FLOW_TYPE::REGISTER:
                const FrontEnd::Register_Instruction* register_instr = dynamic_cast<FrontEnd::Register_Instruction*>(instruction.get());
                if (const auto* register_instr = dynamic_cast<const FrontEnd::Register_Instruction*>(instruction.get())) {
                    switch (register_instr->register_instruction_type()) {
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::ADD:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SUB:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::LOAD_UPPER:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::AUIPC:
                            return RESERVATION_STATION_ID::ADD_SUB;
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::DIV:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::DIVU:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::REM:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::REMU:
                            return RESERVATION_STATION_ID::DIVIDER;
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MUL:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MULH:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MULHSU:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::MULHU:
                            return RESERVATION_STATION_ID::MULTIPLIER;
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::AND:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::OR:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::XOR:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SLL:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SRL:
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SRA:
                            return RESERVATION_STATION_ID::LOGICAL;
                        case FrontEnd::Register_Instruction::REGISTER_INSTRUCTION_TYPE::SLT:
                            return RESERVATION_STATION_ID::SET_LESS;
                    }
                }
            default:
                return RESERVATION_STATION_ID::UNKNOWN;
                break;
            }
        }
        
    } // namespace Core
    
} // namespace OoOVis
