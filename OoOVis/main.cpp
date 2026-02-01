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
	//reservation_station_pool_t pool{
	//	{0,{Core::RESERVATION_STATION_ID::ADD_SUB}},
	//	{1,{Core::RESERVATION_STATION_ID::BITWISE}},
	//	{2,{Core::RESERVATION_STATION_ID::SET_LESS}},
	//	{3,{Core::RESERVATION_STATION_ID::MULTIPLIER}},
	//	{4,{Core::RESERVATION_STATION_ID::DIVIDER}},
	//	{5,{Core::RESERVATION_STATION_ID::LOAD_STORE}},
	//	{6,{Core::RESERVATION_STATION_ID::BRANCH}}
	//};
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