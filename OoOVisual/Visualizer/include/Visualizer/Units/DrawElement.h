#pragma  once
#include <imgui.h>
#include <Visualizer/Camera.h>
namespace OoOVisual
{
	namespace Visualizer
	{
		enum class DRAW_ELEMENT_ID {
			FETCH_UNIT,
			FETCH_GROUP,
			DISPATCHER,
			STATION_0,
			STATION_1,
			STATION_2,
			STATION_3,
			STATION_4,
			STATION_5,
			STATION_6,
			ADDER_SUBTRACTOR_UNIT,
			BITWISE_UNIT,
			LESS_THAN_SETTER_UNIT,
			MULTIPLIER_UNIT,
			DIVIDER_UNIT,
			LOAD_STORE_UNIT,
			BRANCH_UNIT,
			REGISTER_MANAGER,
			REORDER_BUFFER
		};
		struct Draw_Element {
			virtual ~Draw_Element() = default;
			virtual void show_detailed() = 0;
			virtual void show_tooltip() = 0;
			Draw_Element(DRAW_ELEMENT_ID id, ImVec2 position, ImVec2 dimension) : _id(id), _position(position), _dimension(dimension) {}
			void show_architectural(Camera& cam);
			bool is_hovered(const Camera& cam);
			DRAW_ELEMENT_ID id() const { return _id; }
			void set_detailed(const Camera& cam);
			bool detailed() const { return _detailed; }
		protected:
			ImVec2 _position{};
			ImVec2 _dimension{};
			DRAW_ELEMENT_ID _id;
			bool _detailed{ false };
		};
	}
}