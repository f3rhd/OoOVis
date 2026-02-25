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
			static bool init();     // All the GLFW & ImGui setup goes here
			static void start_frame(); // The "NewFrame" calls
			static void end_frame();   // The "Render" and "SwapBuffers" calls
			static bool should_close();
			static void  close();
			static void cleanup();
			static ImVec2 window_size();

		private:
			static GLFWwindow* _window;
		};

	} // namespace Visualizer
} // namespace OoOVisual