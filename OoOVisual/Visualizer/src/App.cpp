#include <Visualizer/App.h>
#include <iostream>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
namespace OoOVisual
{
	namespace Visualizer
	{

		bool App::init() {
			if (!glfwInit()) return false;

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			window = glfwCreateWindow(1280, 720, "OoO CPU Visualizer", nullptr, nullptr);
			if (!window) {
				glfwTerminate();
				return false;
			}

			glfwMakeContextCurrent(window);
			glfwSwapInterval(1); 

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 330");

			return true;
		}

	void App::start_frame() {
		glfwPollEvents();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void App::end_frame() {
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		
		glClearColor(0.1f, 0.1f, 0.12f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);
	}

	bool App::should_close() {
		return glfwWindowShouldClose(window);
	}

	void App::cleanup() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}
	}
}