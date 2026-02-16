#include <Core/Core.h>
#include <Frontend/Parser/Parser.h>
#include <Core/Dispatch/Dispatcher.h>
#include <Core/Execution/ExecutionStage.h>
#include <Core/Commit/ReorderBuffer.h>
#include <Core/Fetch/Fetch.h>
#include <Core/DCache/DCache.h>
#include <Core/RegisterManager/RegisterManager.h>
#include <Core/ReservationStation/ReservationStationPool.h>

namespace OoOVisual
{
	namespace Core
	{

		void init(int argc, char** argv) {
			FrontEnd::Parser parser;
			auto cli_args(parser.parse_cli(argc, argv));
			auto parse_result(parser.parse_instructions(cli_args.input_file));
			Fetch_Unit::init(std::move(parse_result.first),std::move(parse_result.second));
			Register_Manager::init();
		}
		void tick() {
			Reorder_Buffer::commit();
			Execution_Stage::issue_and_execute();
			auto feedback(Dispatcher::dispatch_fetch_group());
			Fetch_Group::group = (Fetch_Unit::fetch(feedback));
		}

		void restart() { 

			Reorder_Buffer::reset();
			DCache::reset();
			Fetch_Unit::reset();
			Reservation_Station_Pool::reset();
		}

	}
}


