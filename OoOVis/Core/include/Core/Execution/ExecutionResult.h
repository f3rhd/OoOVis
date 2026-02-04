#pragma  once
#include <Core/Types/Types.h>
namespace OoOVis
{
	namespace Core
	{
		/*
		instructions will generate this kind of layout upon execution and will be sent to common data bus, 
		at the same time set the respectful rob entry ready
		*/
		struct Forwarding_Data {
			bool		valid{ false };
			data_t		produced_data{};
			u32			producer_tag{};
		};
	}
}