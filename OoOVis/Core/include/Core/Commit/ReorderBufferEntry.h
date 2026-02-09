#pragma  once

#include <Core/Types/Types.h>
#include <Frontend/Parser/Instruction.h>
namespace OoOVis
{
	namespace Core {
		struct Reorder_Buffer_Entry {
			virtual ~Reorder_Buffer_Entry() = default;
			Reorder_Buffer_Entry(FrontEnd::FLOW_TYPE flow) : flow_type(flow) {}
			virtual FrontEnd::FLOW_TYPE flow() { return flow_type; };
		public:
			bool ready = false;
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
				reg_id_t new_
			) : old_alias(old), 
				new_alias(new_), 
				architectural_register_id(architectural), 
				Reorder_Buffer_Entry(flow) {}
		};
		struct Store_Reorder_Buffer_Entry : Reorder_Buffer_Entry {
			u32 id;
			Store_Reorder_Buffer_Entry(FrontEnd::FLOW_TYPE flow, u32 id_) : id(id_), Reorder_Buffer_Entry(flow) {}
		};
		struct Branch_Conditional_Reorder_Buffer_Entry : Reorder_Buffer_Entry {

			bool mispredicted = false;
			Branch_Conditional_Reorder_Buffer_Entry(FrontEnd::FLOW_TYPE flow) : Reorder_Buffer_Entry(flow) {}
		};
	} // namespace Core
} // namespace OoOVis
