#ifndef _VIRTUAL_PAD_FEATURES_h
#define _VIRTUAL_PAD_FEATURES_h

#include "Flag.h"

namespace VirtualPad
{
	namespace Configuration
	{
		/// <summary>
		/// Available features of the Virtual Pad.
		/// All features can be on/off.
		/// </summary>
		enum class FeaturesEnum : uint8_t
		{
			/// <summary>
			/// Pad has a digital directional pad (DPad).
			/// </summary>
			DPad = 0,

			/// <summary>
			/// Pad has a main analog joystick.
			/// </summary>
			Joy1 = 1,

			/// <summary>
			/// Pad has a secondary analog joystick.
			/// </summary>
			Joy2 = 2,

			/// <summary>
			/// Pad has a left analog trigger.
			/// </summary>
			L2 = 3,

			/// <summary>
			/// Pad has a right analog trigger.
			/// </summary>
			R2 = 4,

			/// <summary>
			/// Pad has face button A.
			/// </summary>
			A = 5,

			/// <summary>
			/// Pad has face button B.
			/// </summary>
			B = 6,

			/// <summary>
			/// Pad has face button X.
			/// </summary>
			X = 7,

			/// <summary>
			/// Pad has face button Y.
			/// </summary>
			Y = 8,

			/// <summary>
			/// Pad has a left bumper.
			/// </summary>
			L1 = 9,

			/// <summary>
			/// Pad has a right bumper.
			/// </summary>
			R1 = 10,

			/// <summary>
			/// Pad has clicky main joystick button.
			/// </summary>
			L3 = 11,

			/// <summary>
			/// Pad has clicky secondary joystick button.
			/// </summary>
			R3 = 12,

			/// <summary>
			/// Pad has left back paddle.
			/// </summary>
			P1 = 13,

			/// <summary>
			/// Pad has right back paddle.
			/// </summary>
			P2 = 14,

			/// <summary>
			/// Pad has chat button or equivalent.
			/// </summary>
			Chat = 15,

			/// <summary>
			/// Pad has a start button.
			/// </summary>
			Start = 16,

			/// <summary>
			/// Pad has a select button or equivalent.
			/// </summary>
			Select = 17,

			/// <summary>
			/// Pad has a home button or equivalent.
			/// </summary>
			Home = 18,

			/// <summary>
			/// Pad has a share button or equivalent.
			/// </summary>
			Share = 19,

			/// <summary>
			/// Pad has a motion capabilities.
			/// </summary>
			Motion = 20,

			FeatureEnumCount
		};

		/// <summary>
		/// Template parameter generator for FeatureFlags.
		/// </summary>
		/// <typeparam name="...features">Feature list.</typeparam>
		/// <returns>FeatureFlags code.</returns>
		template<FeaturesEnum...features>
		static constexpr uint32_t GetFeatureFlags()
		{
			return TemplateFlag::GetFlags<(uint8_t)features...>();
		}		
	}
}
#endif