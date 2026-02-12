#include <Frontend/Parser/Parser.h>
#include <Core/Dispatch/Dispatcher.h>
#include <Core/Execution/ExecutionStage.h>
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Fetch/Fetch.h>
#include <Core/RegisterFile/RegisterFile.h>
using namespace OoOVisual;
using namespace Core;
int main(int argc, char** argv) {
	FrontEnd::Parser parser;
	auto cli_args(parser.parse_cli(argc, argv));
	auto parse_result(parser.parse_instructions(cli_args.input_file));
	Fetch_Unit::init(std::move(parse_result.first));
	Register_File::init();
	while (true) {
		Reorder_Buffer::commit();
		Execution_Stage::issue_and_execute();
		auto feedback(Dispatcher::dispatch_fetch_group());
		Fetch_Group::group = (Fetch_Unit::fetch(feedback));
	}
}