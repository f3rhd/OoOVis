#pragma once
#include <imgui.h>
namespace OoOVisual
{
	namespace Visualizer
	{
		namespace Utils
		{

			void draw_glow_rec(ImVec2 pos, ImVec2 size, ImU32 col);
			void draw_glow_line(ImVec2 p1, ImVec2 p2, ImU32 col);

		}
	}
}