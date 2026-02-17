#pragma  once
#include <Core/ReservationStation/ReservationStationEntry.h>
#include <Core/Execution/ExecutionResult.h>
namespace OoOVisual
{
	namespace Core
	{

		class Execution_Unit_Adder { // does subtracting too.
		public:
			static Forwarding_Data execute(const Reservation_Station_Entry* source_entry);
		};
		class Execution_Unit_Bitwise {
		public:
			static Forwarding_Data execute(const Reservation_Station_Entry* source_entry);
		};

		// in real hardware designs having a seperate unit just for this bitch ass instruction is very nooby thing to do  but we gon do it in software anyway 
		class Execution_Unit_Set_Less_Than {
		public:
			static Forwarding_Data execute(const Reservation_Station_Entry* source_entry);

		};
		class Execution_Unit_Multiplier {
		public:
			static Forwarding_Data execute(const Reservation_Station_Entry* source_entry);
		};
		class Execution_Unit_Divider {
		public:
			static Forwarding_Data execute(const Reservation_Station_Entry* source_entry);
		};

		class Execution_Unit_Load_Store {
		public:
			struct Buffer_Entry {
				EXECUTION_UNIT_MODE mode{EXECUTION_UNIT_MODE::UNKNOWN};
				u32			  timestamp; // Reorder buffer will find the buffer to flush thanks to this field
				reg_id_t      register_id; // load uses this to write  the value to the regsiter file
				size_t	      reorder_buffer_entry_index; // load  uses this to set the rob entry free
				data_t        register_data; // this is  used by store  
				memory_addr_t calculated_address; // this is also both used by store and load
				u32			  producer_tag; // load uses this to broadcast the tag to the common data bus for forwarding logic
				Buffer_Entry(
					EXECUTION_UNIT_MODE mode_,
					u32 id_,
					u32 id__,
					size_t index, 
					data_t data, 
					memory_addr_t addr,
					u32 producer_tag_
				) : mode(mode_),
					timestamp(id_), 
					register_id(id__),
					reorder_buffer_entry_index(index), 
					register_data(data), 
					calculated_address(addr),
					producer_tag(producer_tag_){}
			};
			static Forwarding_Data					buffer_allocation_phase(const Reservation_Station_Entry* source_entry);
			static Forwarding_Data					execute_load();
			static void								execute_store(memory_addr_t store_id);
			static time_t							flush_mispredicted(time_t timestamp);
			static bool								store_buffer_is_full() { return _store_buffer.size() >= Constants::STORE_BUFFER_SIZE; }
			static bool								load_buffer_is_full() { return _load_buffer.size() >= Constants::LOAD_BUFFER_SIZE; }
			static const std::vector<Buffer_Entry>& store_buffer() { return _store_buffer; } // used by visualizer
			static const std::vector<Buffer_Entry>& load_buffer() { return _load_buffer; } // used by visualizer
			static void							    reset();
		private:
			static std::pair<size_t,size_t>		find_load_that_is_executable(); // .first stands for executable load if it can be executed by forwarding .second will hold the store buffer entry index that is being forwarded from
		private:
			static std::vector<Buffer_Entry> _store_buffer;
			static std::vector<Buffer_Entry> _load_buffer;
		};
		class Execution_Unit_Branch {
		public:
			static Forwarding_Data execute(const Reservation_Station_Entry* source_entry);
		};
	} // namespace Core
} // namespace OooVis