#pragma once
#include <vector>
#include <Core/Execution/ExecutionResult.h>
namespace OoOVisual
{
	namespace Core
	{
		struct Execution_Stage {
			static void issue_and_execute(); // forwarding logic is implemented here along with execution
		};
	}
}