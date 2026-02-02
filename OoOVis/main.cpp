#include <Frontend/Parser/Parser.h>
#include <Core/Dispatch/Dispatcher.h>
#include <Core/ReservationStation/ReservationStation.h>
#include <Core/Fetch/Fetch.h>
using namespace OoOVis;
int main(int argc, char** argv) {
	//FrontEnd::Parser parser;
	//auto cli_args(parser.parse_cli(argc, argv));
	//auto parse_result(parser.parse_instructions(cli_args.input_file));
	//Core::Fetch_Unit::init(std::move(parse_result.first));
	//Core::Dispatcher dispatcher;
	//while (true) {
	//	Core::Fetch_Group fetch_group(Core::Fetch_Unit::fetch());
	//	if(fetch_group.first)
	//	dispatcher.dispatch_instruction(*fetch_group.first,pool);
	//	if(fetch_group.second)
	//		dispatcher.dispatch_instruction(*fetch_group.second,pool);
	//	if(fetch_group.third)
	//		dispatcher.dispatch_instruction(*fetch_group.third,pool);
	//}
}