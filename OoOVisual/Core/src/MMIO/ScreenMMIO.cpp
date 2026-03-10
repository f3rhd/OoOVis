#include <Core/MMIO/ScreenMMIO.h>
namespace OoOVisual
{
	namespace Core
	{
		std::array<u32, Constants::CORE_SCREEN_SIZE> Screen_MMIO::_buffer{0xFFFFFFFF};
		Screen_MMIO::CMD_TYPE	Screen_MMIO::_command_type;
		i32		Screen_MMIO::_param_x1;
		i32		Screen_MMIO::_param_y1;
		i32		Screen_MMIO::_param_x2;
		i32		Screen_MMIO::_param_y2;
		i32		Screen_MMIO::_param_color;
		const std::array<u32, Constants::CORE_SCREEN_SIZE>& Screen_MMIO::buffer() { return _buffer; }
		void Screen_MMIO::clear_screen() {
			std::fill(_buffer.begin(), _buffer.end(), 0x000000);
		}
		bool Screen_MMIO::handle_write(u32 address, i32 value) {
			if (address == Constants::START_ADDR && value == 1) {

				switch (_command_type) {

					case CMD_TYPE::FILL:
					{
						i32 start_index{ _param_x1 + _param_y1 * Constants::CORE_SCREEN_WIDTH };
						i32 end_index{ _param_x2 + _param_y2 * Constants::CORE_SCREEN_WIDTH };
						std::fill(_buffer.begin() + start_index, _buffer.begin() + end_index, _param_color);
						return true ;
					}

					case CMD_TYPE::LINE:
					{
						i32 x1{ _param_x1 }, y1{ _param_y1 };
						i32 x2{ _param_x2 }, y2{ _param_y2 };

						i32 dx { std::abs(x2 - x1)};
						i32 dy { std::abs(y2 - y1)};
						i32 sx { (x1 < x2) ? 1 : -1};
						i32 sy { (y1 < y2) ? 1 : -1};
						i32 err { dx - dy};

						while (true) {
							_buffer[x1 + y1 * Constants::CORE_SCREEN_WIDTH] = _param_color;
							if (x1 == x2 && y1 == y2) break;
							i32 e2{ 2 * err };
							if (e2 > -dy) { err -= dy; x1 += sx; }
							if (e2 <  dx) { err += dx; y1 += sy; }
						}
						return true;
					}

					case CMD_TYPE::RECT:
					{
						// Top and bottom edges
						std::fill(_buffer.begin() + _param_x1 + _param_y1 * Constants::CORE_SCREEN_WIDTH,
								  _buffer.begin() + _param_x2 + _param_y1 * Constants::CORE_SCREEN_WIDTH + 1, _param_color);
						std::fill(_buffer.begin() + _param_x1 + _param_y2 * Constants::CORE_SCREEN_WIDTH,
								  _buffer.begin() + _param_x2 + _param_y2 * Constants::CORE_SCREEN_WIDTH + 1, _param_color);

						// Left and right edges
						for (i32 y = _param_y1 + 1; y < _param_y2; ++y) {
							_buffer[_param_x1 + y * Constants::CORE_SCREEN_WIDTH] = _param_color;
							_buffer[_param_x2 + y * Constants::CORE_SCREEN_WIDTH] = _param_color;
						}
						return true;
					}
					case CMD_TYPE::UNKNOWN:
					default:
						return false;

				}
			}
			switch (address)
			{
			case Constants::PARAM_COL_ADDR:
				value = _param_color = ((value & 0xFF) << 24) |         
									   ((value >> 8  & 0xFF) << 16) |   
									   ((value >> 16 & 0xFF) << 8)  |   
									   ((value >> 24 & 0xFF));
				_param_color = value;
				return true;
			case Constants::PARAM_X1_ADDR:
				_param_x1 = value;
				return true;
			case Constants::PARAM_X2_ADDR:
				_param_x2 = value;
				return true;
			case Constants::PARAM_Y1_ADDR:
				_param_y1 = value;
				return true;
			case Constants::PARAM_Y2_ADDR:
				_param_y2 = value;
				return true;
			case Constants::CMD_TYPE_ADDR:
				_command_type = static_cast<CMD_TYPE>(value);
				return true;
			default:
				break;
			}
			return false;
		}

	}
}