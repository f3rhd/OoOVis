#include <Visualizer/Camera.h>
namespace OoOVisual
{
	namespace Visualizer 
	{

		ImVec2 Camera::world_to_screen(ImVec2 world_pos) const
		{
			return ImVec2{
				world_pos.x * zoom + translation.x,
				world_pos.y * zoom + translation.y
			};

		}

		void Camera::update()
		{
			ImGuiIO& io{ ImGui::GetIO() };

			if (io.WantCaptureMouse) return;
			if (io.MouseWheel != 0.0f) {
				ImVec2 mouse_pos{ io.MousePos };
				
				ImVec2 mouse_world_pos{ (mouse_pos.x - translation.x) / zoom, 
										(mouse_pos.y - translation.y) / zoom };

				float zoom_factor{ 1.1f };
				if (io.MouseWheel > 0) zoom *= zoom_factor;
				else zoom /= zoom_factor;
				if (zoom < 0.1f) zoom = 0.1f;
				if (zoom > 5.0f) zoom = 5.0f;

				translation.x = mouse_pos.x - mouse_world_pos.x * zoom;
				translation.y = mouse_pos.y - mouse_world_pos.y * zoom;
			}
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {

				ImVec2 delta{ io.MouseDelta };
				translation.x += delta.x;
				translation.y += delta.y;
			}
		}

	}

}