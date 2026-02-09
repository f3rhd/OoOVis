#include <Frontend/Parser/Parser.h>
#include <Core/Dispatch/Dispatcher.h>
#include <Core/Execution/ExecutionStage.h>
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Fetch/Fetch.h>
#include <Core/RegisterFile/RegisterFile.h>
using namespace OoOVis;
using namespace Core;
int main(int argc, char** argv) {
	FrontEnd::Parser parser;
	auto cli_args(parser.parse_cli(argc, argv));
	auto parse_result(parser.parse_instructions(cli_args.input_file));
	Fetch_Unit::init(std::move(parse_result.first));
	Register_File::init();
	Dispatcher dispatcher;
	while (true) {
		Reorder_Buffer::commit();
		Execution_Stage::issue_and_execute();
		memory_addr_t successful_dispatches{};
		for (const auto& fetch_element : Fetch_Group::group) {
			if (!Reorder_Buffer::full()) {
				if (dispatcher.dispatch_instruction(fetch_element))
					successful_dispatches++;
			}
		}
		if(!Fetch_Unit::next_fetch_is_set())
			Fetch_Unit::adjust_program_counter_based_on_successful_dispatches(successful_dispatches);
		Fetch_Group::group = (Fetch_Unit::fetch());
	}
}