#pragma  once

#include <Core/Types/Types.h>
#include <Frontend/Parser/Instruction.h>
#include <Core/Constants/Constants.h>
namespace OoOVisual
{
	namespace Core {
		struct Reorder_Buffer_Entry {
			virtual ~Reorder_Buffer_Entry() = default;
			Reorder_Buffer_Entry(FrontEnd::FLOW_TYPE flow,u32 speculation_id_) : flow_type(flow), origin_branch_id(speculation_id_) {}
			virtual FrontEnd::FLOW_TYPE flow() { return flow_type; };
		public:
			bool ready = false;
			u32 origin_branch_id = Constants::NOT_SPECULATIVE;
			FrontEnd::FLOW_TYPE flow_type = FrontEnd::FLOW_TYPE::UNKNOWN;
		};
		struct Register_Reorder_Buffer_Entry : Reorder_Buffer_Entry { // load dispatching and jump will also create this type of entry
			reg_id_t old_alias;
			reg_id_t new_alias;
			reg_id_t architectural_register_id;
			Register_Reorder_Buffer_Entry ( 
				FrontEnd::FLOW_TYPE flow, 
				reg_id_t architectural, 
				reg_id_t old,
				reg_id_t new_,
				u32 speculation_id
			) : old_alias(old), 
				new_alias(new_), 
				architectural_register_id(architectural), 
				Reorder_Buffer_Entry(flow,speculation_id) {}
		};
		struct Store_Reorder_Buffer_Entry : Reorder_Buffer_Entry {
			u32 store_id;
			Store_Reorder_Buffer_Entry(FrontEnd::FLOW_TYPE flow, u32 id_,u32 speculation_id) : store_id(id_), Reorder_Buffer_Entry(flow,speculation_id) {}
		};
		struct Branch_Conditional_Reorder_Buffer_Entry : Reorder_Buffer_Entry {

			bool mispredicted = false;
			memory_addr_t self_id;
			Branch_Conditional_Reorder_Buffer_Entry(
				FrontEnd::FLOW_TYPE flow,
				u32 speculation_id,
				memory_addr_t instruction_id_
			) : self_id(instruction_id_),
				Reorder_Buffer_Entry(flow,speculation_id) {}
		};
		struct Branch_Unconditional_Reorder_Buffer_Entry : Reorder_Buffer_Entry {
			bool mispredicted = false;
			reg_id_t old_alias;
			reg_id_t new_alias;
			reg_id_t architectural_register_id;
			memory_addr_t self_id;
			Branch_Unconditional_Reorder_Buffer_Entry ( 
				FrontEnd::FLOW_TYPE flow, 
				reg_id_t architectural, 
				reg_id_t old,
				reg_id_t new_,
				bool mispredicted_, // jalr will always be true 
				u32 speculation_id,
				memory_addr_t instruction_id_
			) : old_alias(old), 
				new_alias(new_), 
				architectural_register_id(architectural), 
				mispredicted(mispredicted_),
				self_id(instruction_id_),
				Reorder_Buffer_Entry(flow,speculation_id) {}

		};
	} // namespace Core
} // namespace OoOVis
