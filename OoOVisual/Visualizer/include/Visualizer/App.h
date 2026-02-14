#include <GLFW/glfw3.h>
#include <imgui.h>
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

		private:
			GLFWwindow* window;
		};

	} // namesapce Visualizer
} // namespace OoOVisual