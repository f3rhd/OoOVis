#include <Frontend/Parser/Parser.h>
#include <Core/Dispatch/Dispatcher.h>
#include <Core/Fetch/Fetch.h>
using namespace OoOVis;
int main(int argc, char** argv) {
	FrontEnd::Parser parser;
	auto cli_args(parser.parse_cli(argc, argv));
	auto parse_result(parser.parse_instructions(cli_args.input_file));
	Core::Fetch_Unit::init(std::move(parse_result.first));
	Core::Dispatcher dispatcher;
	while (true) {
		Core::Fetch_Group fetch_group(Core::Fetch_Unit::fetch());
		if (fetch_group.first_instruction.first) {
			dispatcher.dispatch_instruction(*fetch_group.first_instruction.first,fetch_group.first_instruction.second);
		}
		if (fetch_group.second_instruction.first) {
			dispatcher.dispatch_instruction(*fetch_group.second_instruction.first,fetch_group.second_instruction.second);
		}
		if (fetch_group.third_instruction.first) {
			dispatcher.dispatch_instruction(*fetch_group.third_instruction.first,fetch_group.third_instruction.second);
		}
	}
}