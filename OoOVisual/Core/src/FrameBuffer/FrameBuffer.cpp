#include <Core/FrameBuffer/FrameBuffer.h>
namespace OoOVisual
{
	namespace Core
	{
		std::array<u32, Constants::CORE_SCREEN_SIZE> Frame_Buffer::buffer{0xFFFFFFFF};
	}
}