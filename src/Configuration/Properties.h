#ifndef _VIRTUAL_PAD_PROPERTIES_h
#define _VIRTUAL_PAD_PROPERTIES_h

#include "Flag.h"

namespace VirtualPad
{
	namespace Configuration
	{
		/// <summary>
		/// Default value for PropertyFlags.
		/// </summary>
		static constexpr uint8_t NoProperties = 0;

		enum class PropertiesEnum : uint8_t
		{
			/// <summary>
			/// Main analog joystick is actually a digital output.
			/// </summary>
			Joy1Digital = 0,

			/// <summary>
			/// Secondary analog joystick is actually a digital output.
			/// </summary>
			Joy2Digital = 1,

			/// <summary>
			/// Analog triggers are actually digital outputs.
			/// </summary>
			L2R2Digital = 2,

			EnumCount
		};

		/// <summary>
		/// Template parameter generator for PropertyFlags.
		/// </summary>
		/// <typeparam name="...properties">Property list.</typeparam>
		/// <returns>PropertyFlags code.</returns>
		template<PropertiesEnum... properties>
		static constexpr uint16_t GetPropertyFlags()
		{
			return TemplateFlag::GetFlags<(uint8_t)properties...>();
		}
	}
}
#endif