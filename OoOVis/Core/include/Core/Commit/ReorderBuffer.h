#include <Core/Commit/ReorderBufferEntry.h>
#include <queue>
namespace OoOVis 
{
	namespace Core 
	{
		class Reorder_Buffer {

		public:
			static void allocate(const Reorder_Buffer_Entry& entry);
			static bool full();
		private:
			static std::queue<Reorder_Buffer_Entry> _buffer;
		};
	}
}