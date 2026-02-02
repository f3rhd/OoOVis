#pragma  once
#include <Core/ReservationStation/ReservationStationEntry.h>
#include <Core/Execution/ExecutionResult.h>
namespace OoOVis
{
	namespace Core
	{

		class Execution_Unit_Adder { // does subtracting too.
		public:
			static Execution_Result execute(const Reservation_Station_Entry* source_entry);
		};
		class Execution_Unit_Bitwise {
		public:
			static Execution_Result execute(const Reservation_Station_Entry* source_entry);
		};

		// in real hardware designs having a seperate unit just for this bitch ass instruction is very nooby thing to do  but we gon do it in software anyway 
		class Execution_Unit_Set_Less_Than {
		public:
			static Execution_Result execute(const Reservation_Station_Entry* source_entry);

		};
		class Execution_Unit_Multiplier {
		public:
			static Execution_Result execute(const Reservation_Station_Entry* source_entry);
		};
		class Execution_Unit_Divider {
		public:
			static Execution_Result execute(const Reservation_Station_Entry* source_entry);
		};

		class Execution_Unit_Load_Store {
		public:
			static void buffer_allocation_phase(const Reservation_Station_Entry* source_entry);
			static Execution_Result execute_load();
			static void				execute_store(memory_addr_t store_id);
			static bool				store_buffer_is_full() { return _store_buffer.size() >= STORE_BUFFER_SIZE; }
			static bool				load_buffer_is_full() { return _load_buffer.size() >= LOAD_BUFFER_SIZE; }
			struct Buffer_Entry {
				u32			  self_id; // Reorder buffer will find the buffer to flush thanks to this field
				reg_id_t      register_id; // load uses this to broadcast the value to common data bus
				size_t	      reorder_buffer_entry_index; // load also uses this to set the rob entry free
				data_t        register_data; // this is both used by store and load 
				memory_addr_t calculated_address; // this is also both used by store and load
				u32			  producer_tag; // load uses this to broadcast the tag to the common data bus for forwarding logic
				Buffer_Entry(
					u32 id_,
					u32 id__,
					size_t index, 
					data_t data, 
					memory_addr_t addr,
					u32 producer_tag_
				) : self_id(id_), 
					register_id(id__),
					reorder_buffer_entry_index(index), 
					register_data(data), 
					calculated_address(addr),
					producer_tag(producer_tag_){}
				Buffer_Entry() = default;
			};
		private:
			static std::vector<Buffer_Entry> _store_buffer;
			static std::vector<Buffer_Entry> _load_buffer;
		};
		class Execution_Unit_Branch {
			static Execution_Result execute(const Reservation_Station_Entry* source_entry);
		};
	} // namespace Core
} // namespace OooVis