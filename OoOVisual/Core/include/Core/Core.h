#pragma  once
#include <Frontend/Parser/Parser.h>
namespace OoOVisual 
{
	namespace Core
	{

		enum class CORE_MODE {
			RUN,
			STEP,
			STOP,
		};
		FrontEnd::cli_args_t init(int argc, char** argv);
		void run();
		float* tick_speed();
		void reset();
		void set_core_mode(CORE_MODE mode);
	}

}