#pragma  once
#include <Core/Constants/Constants.h>
#include <Core/Types/Types.h>

#include <array>

namespace OoOVisual
{
	namespace Core 
	{
		struct Frame_Buffer {
			static std::array<u32, Constants::CORE_SCREEN_SIZE> buffer;
		};
	} // namespace Core

} // namespace OoOVisual