#pragma once
#include <Core/Types/Types.h>
namespace OoOVis {
	namespace FrontEnd {
		auto constexpr NO_LABEL = 0;
		auto constexpr FORWARD_LABEL = 1;
		enum class FLOW_TYPE {
			REGISTER,
			LOAD,
			STORE,
			BRANCH_CONDITIONAL,
			BRANCH_UNCONDITIONAL,
			UNKNOWN
		};
		struct Instruction {
			virtual label_id_t target_label() const { return NO_LABEL; }
			virtual bool is_label_instruction() const { return false; }
			virtual ~Instruction() = default;
			virtual FLOW_TYPE flow_type() const = 0;
		};
		struct Memory_Instruction : Instruction {
			Memory_Instruction(
				const reg_id_t  dest_reg_,
				const reg_id_t  base_reg_,
				offset_t offset_
			) :
				_dest_src_reg(dest_reg_),
				_base_reg(base_reg_),
				_offset(offset_) {
			}
		protected:
			reg_id_t _dest_src_reg;
			reg_id_t _base_reg;
			offset_t _offset;
		};
		struct Load_Instruction : Memory_Instruction {
			enum class LOAD_INSTRUCTION_TYPE
			{
				LB,
				LH,
				LW,
				LBU,
				LHU,
				UNKNOWN
			};
			Load_Instruction(
				LOAD_INSTRUCTION_TYPE type_,
				const reg_id_t dest_reg_,
				offset_t offset_,
				const reg_id_t base_reg_
			) :
				Memory_Instruction(dest_reg_, base_reg_, offset_), _type(type_) {
			}
			FLOW_TYPE flow_type() const override {
				return FLOW_TYPE::LOAD;
			};
		private:
			LOAD_INSTRUCTION_TYPE _type;
		};
		struct Store_Instruction : Memory_Instruction {
			enum class STORE_INSTRUCTION_TYPE
			{
				SB,
				SH,
				SW,
				UNKNOWN
			};
			Store_Instruction(
				STORE_INSTRUCTION_TYPE type_,
				const reg_id_t dest_reg_,
				offset_t offset_,
				const reg_id_t base_reg_
			) :
				Memory_Instruction(dest_reg_, base_reg_, offset_), _type(type_) {
			}
			FLOW_TYPE flow_type() const override {
				return FLOW_TYPE::STORE;
			};
		private:
			STORE_INSTRUCTION_TYPE _type;
		};
		struct Register_Instruction : Instruction {
			enum class REGISTER_INSTRUCTION_TYPE
			{
				ADD,
				SUB,
				SLL,
				SLT,
				SLTU,
				XOR,
				SRL,
				SRA,
				OR,
				AND,
				MUL,
				MULH,
				MULHSU,
				MULHU,
				DIV,
				DIVU,
				REM,
				REMU,
				LOAD_UPPER,
				AUIPC,
				UNKNOWN
			};
			union _src2_ {
				reg_id_t src2_reg;
				int64_t imm_val;
			};
			Register_Instruction(
				REGISTER_INSTRUCTION_TYPE type_,
				const reg_id_t dest_reg_,
				const reg_id_t src1_reg_,
				int64_t src2_val,
				bool is_imm_
			) :
				_dest_reg(dest_reg_),
				_src1_reg(src1_reg_),
				_type(type_),
				_is_imm(is_imm_) {

				if (_is_imm)
					_src2.imm_val = src2_val;
				else
					_src2.src2_reg = (reg_id_t)src2_val;

			}
			FLOW_TYPE flow_type() const override {
				return FLOW_TYPE::REGISTER;
			};
			REGISTER_INSTRUCTION_TYPE register_instruction_type() const {
				return _type;
			}
			bool uses_immval() const { return _is_imm; }
			reg_id_t dest_reg() const { return _dest_reg; }
			reg_id_t src1_reg() const { return _src1_reg; }
			_src2_   src2() const { return _src2; }
		private:
			_src2_ _src2;
			reg_id_t _dest_reg;
			reg_id_t _src1_reg;
			REGISTER_INSTRUCTION_TYPE _type;
			bool _is_imm = false;
		};
		struct Branch_Instruction : Instruction {
			enum class BRANCH_INSTRUCTION_TYPE
			{
				BEQ,
				BNE,
				BLT,
				BGE,
				BLTU,
				BGEU,
				UNKNOWN
			} _type;
			Branch_Instruction(
				BRANCH_INSTRUCTION_TYPE type_,
				reg_id_t src1_,
				reg_id_t src2_,
				label_id_t label_,
				branch_instruction_id_t branch_id
			) :
				_src1_reg(src1_),
				_src2_reg(src2_),
				_target_label_id(label_),
				_type(type_),
				_id(branch_id)
			{
			}
			label_id_t target_label() const override { return _target_label_id; }
			void set_target_label(label_id_t label_id) { _target_label_id = label_id; };
			FLOW_TYPE flow_type() const override {
				return FLOW_TYPE::BRANCH_CONDITIONAL;
			}

		private:
			branch_instruction_id_t _id;
			reg_id_t _src1_reg;
			reg_id_t _src2_reg;
			label_id_t _target_label_id;
		};
		struct Jump_Instruction : Instruction {
			enum class JUMP_INSTRUCTION_TYPE
			{
				JALR,
				JAL,
				UNKNOWN
			} _type;
			label_id_t target_label() const override { return _target_label_id; }
			void set_target_label(label_id_t label_id) { _target_label_id = label_id; };
			Jump_Instruction(
				JUMP_INSTRUCTION_TYPE type_,
				reg_id_t dest_reg_,
				reg_id_t src1_,
				label_id_t label_id_,
				int64_t imm_
			) : _type(type_),
				_dest_reg(dest_reg_),
				_src1(src1_),
				_target_label_id(label_id_),
				_imm(imm_) {
			}
			FLOW_TYPE flow_type() const {
				return FLOW_TYPE::BRANCH_UNCONDITIONAL;
			}
		private:
			reg_id_t _dest_reg;
			label_id_t _target_label_id; // jal uses this
			reg_id_t _src1;// jalr uses this
			int64_t _imm;  // jalr uses this
		};
		struct Label_Instruction : Instruction {
			Label_Instruction(label_id_t label_) : _label_id(label_) {}
			label_id_t target_label() const override { return _label_id; }
			bool is_label_instruction() const override { return true; }
			FLOW_TYPE flow_type() const override { return FLOW_TYPE::UNKNOWN; }
		private:
			label_id_t _label_id;
		};

	} // namespace Instruction
} //namespace OoOVis
