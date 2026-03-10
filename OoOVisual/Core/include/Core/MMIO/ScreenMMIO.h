#pragma  once
#include <Core/Constants/Constants.h>
#include <Core/Types/Types.h>

#include <array>

namespace OoOVisual
{
	namespace Core 
	{
		struct Screen_MMIO {
			enum class CMD_TYPE : i32 {
				UNKNOWN = 0,
				FILL,
				LINE,
				RECT
			};
			static bool handle_write(u32 address,i32 value);
			static void clear_screen();
			static const std::array<u32, Constants::CORE_SCREEN_SIZE>& buffer();
		private:
			static CMD_TYPE _command_type;
			static i32		_param_x1;
			static i32		_param_y1;
			static i32		_param_x2;
			static i32		_param_y2;
			static i32		_param_color;
			static std::array<u32, Constants::CORE_SCREEN_SIZE> _buffer;
		};


	} // namespace Core

} // namespace OoOVisual