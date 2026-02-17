#include <Visualizer/App.h>
#include <Visualizer/Scene.h>
#include <Core/Core.h>
using namespace OoOVisual;
int main(int argc, char** argv) {
	Core::init(argc,argv);
	Visualizer::App app{};
	Visualizer::Scene scene{};
	if (!app.init())
		return -1;
	scene.init();
	bool step{};
	while (!app.should_close()) {
		if (true) {
			Core::tick();
			step = false;
		}
		app.start_frame();
		ImGui::SetNextWindowSize(app.window_size());
		scene.play();
		if (ImGui::Button("step")) {
			step = true;
		}
		if (ImGui::Button("Reset")) {
			Core::restart();
		}
		ImGui::End();
		app.end_frame();
	}
    app.cleanup();
}