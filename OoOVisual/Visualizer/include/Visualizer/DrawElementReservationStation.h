#include <Visualizer/DrawElement.h>
#include <Core/ReservationStation/ReservationStation.h>
namespace OoOVisual
{
	namespace Visualizer
	{

		struct Draw_Element_Reservation_Station : Draw_Element {

			explicit Draw_Element_Reservation_Station(DRAW_ELEMENT_ID id, ImVec2 position, ImVec2 dimension);
			void show_tooltip() override;
			void show_detailed() override;

		private:
			const Core::Reservation_Station* _core_station{};
		};

	}
}