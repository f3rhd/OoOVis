#include <Visualizer/App.h>
#include <Visualizer/Scene.h>
#include <Core/Core.h>
using namespace OoOVisual;
int main(int argc, char** argv) {
	Core::init(argc, argv);
	Visualizer::Scene scene{};
	if (!Visualizer::App::init())
		return -1;
	scene.init();
	while (!Visualizer::App::should_close()) {
		Core::run();
		Visualizer::App::start_frame();
		scene.play();
		Visualizer::App::end_frame();
	}
	Visualizer::App::cleanup();
}