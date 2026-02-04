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
	memory_addr_t program_counter_increment{};
	while (!Fetch_Unit::endofprogram()) {
		Reorder_Buffer::commit();
		Execution_Stage::issue_and_execute();
		Fetch_Unit::increment_counter_by(program_counter_increment);
		program_counter_increment = 0;
		Fetch_Group fetch_group(Fetch_Unit::fetch());
		if (fetch_group.first_instruction.first && !Reorder_Buffer::full())
			if (dispatcher.dispatch_instruction(*fetch_group.first_instruction.first, fetch_group.first_instruction.second))
				program_counter_increment++;
		if (fetch_group.second_instruction.first && !Reorder_Buffer::full())
			if (dispatcher.dispatch_instruction(*fetch_group.second_instruction.first, fetch_group.second_instruction.second))
				program_counter_increment++;
		if (fetch_group.third_instruction.first && !Reorder_Buffer::full())
			if(dispatcher.dispatch_instruction(*fetch_group.third_instruction.first,fetch_group.third_instruction.second))
				program_counter_increment++;
			
	}
}