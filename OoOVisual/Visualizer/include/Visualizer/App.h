#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
namespace OoOVisual
{
	namespace Visualizer
	{
		class App {
		public:
			App() = default;
			bool init();     // All the GLFW & ImGui setup goes here
			void start_frame(); // The "NewFrame" calls
			void end_frame();   // The "Render" and "SwapBuffers" calls
			bool should_close();
			void cleanup();
			ImVec2 window_size();

		private:
			GLFWwindow* window;
		};

	} // namespace Visualizer
} // namespace OoOVisual