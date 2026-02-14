#include <Frontend/Parser/Parser.h>
#include <Core/Dispatch/Dispatcher.h>
#include <Core/Execution/ExecutionStage.h>
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Fetch/Fetch.h>
#include <Core/RegisterFile/RegisterFile.h>
#include <Visualizer/App.h>
#include <Visualizer/DrawElements.h>
#include <memory>
using namespace OoOVisual;
using namespace Core;
int main(int argc, char** argv) {
	FrontEnd::Parser parser;
	auto cli_args(parser.parse_cli(argc, argv));
	auto parse_result(parser.parse_instructions(cli_args.input_file));
	Fetch_Unit::init(std::move(parse_result.first));
	Register_File::init();
    Visualizer::App app{};
	std::unique_ptr<Visualizer::Draw_Element> element(std::make_unique<Visualizer::Draw_Element_Reservation_Station>(
		Visualizer::DRAW_ELEMENT_ID::STATION_0,
		ImVec2(50,50), 
		ImVec2(300,300 )
	));
	if (!app.init())
		return -1;
	while (!app.should_close()) {
		app.start_frame();
		ImGui::Begin("Pipeline Canvas", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
		element->show_architectural();
		if (element->is_hovered())
			element->show_tooltip();
		else if (element->is_double_clicked())
			element->show_detailed();
		ImGui::End();
		app.end_frame();
		//Reorder_Buffer::commit();
		//Execution_Stage::issue_and_execute();
		//auto feedback(Dispatcher::dispatch_fetch_group());
		//Fetch_Group::group = (Fetch_Unit::fetch(feedback));
	}
    app.cleanup();
}