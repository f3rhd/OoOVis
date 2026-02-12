#include <Core/Commit/ReorderBufferEntry.h>
#include <memory>
namespace OoOVisual 
{
	namespace Core 
	{
		class Reorder_Buffer {

		public:
			static size_t allocate(std::unique_ptr<Reorder_Buffer_Entry>&& entry);
			static void   set_ready(u64 target_entry_index);
			static void   set_branch_evaluation(u64 target,bool was_misprediction,time_t flush_boundary);
			static void   commit();
			static bool   full();
		private:
			static std::vector<std::unique_ptr<Reorder_Buffer_Entry>> _buffer;
			static size_t _head; // points to the slot that is gonna be retired (if ready) next cycle
			static size_t _tail; // points to the slot that is going to be allocated 
		};
	} // namespace Core
} // namespace OoOVis