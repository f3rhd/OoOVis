#pragma  once
#include <Core/ReservationStation/ReservationStationEntry.h>
#include <Core/Execution/ExecutionResult.h>
namespace OoOVis
{
	namespace Core
	{

		class Execution_Unit_Adder { // does subtracting too.

			static Execution_Result execute(const Reservation_Station_Entry* source_entry);
		};
		class Execution_Unit_Bitwise {
			static Execution_Result execute(const Reservation_Station_Entry* source_entry);
		};

		// in real hardwware designs having a seperate unit just for this bitch ass instruction is very nooby thing to do  but we gon do it in software anyway 
		class Execution_Unit_Set_Less_Than {
			static Execution_Result execute(const Reservation_Station_Entry* source_entry);

		};
		class Execution_Unit_Multiplier {
			static Execution_Result execute(const Reservation_Station_Entry* source_entry);
		};
		class Execution_Unit_Divider {
			static Execution_Result execute(const Reservation_Station_Entry* source_entry);
		};

	}
}