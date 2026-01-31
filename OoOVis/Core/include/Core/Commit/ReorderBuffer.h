#include <Core/Commit/ReorderBufferEntry.h>
#include <queue>
#include <memory>
namespace OoOVis 
{
	namespace Core 
	{
		class Reorder_Buffer {

		public:
			static size_t allocate(std::unique_ptr<Reorder_Buffer_Entry>&& entry);
			static bool full();
		private:
			static std::queue<std::unique_ptr<Reorder_Buffer_Entry>> _buffer;
		};
	}
}