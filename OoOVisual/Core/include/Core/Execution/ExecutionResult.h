#pragma  once
#include <Core/Types/Types.h>
#include <Core/Constants/Constants.h>
namespace OoOVis
{
	namespace Core
	{
		/*
		instructions will generate this kind of layout upon execution and will be sent to common data bus, 
		at the same time set the respectful rob entry ready
		*/
		struct Forwarding_Data {
			u32			kind = Constants::FORWARDING_DATA_INVALID;
			data_t		produced_data{};
			u32			producer_tag{};
		};
	 } // namespace Core
} // namespace OoOVis