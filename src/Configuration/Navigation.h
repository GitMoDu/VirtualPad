#ifndef _VIRTUAL_PAD_NAVIGATION_
#define _VIRTUAL_PAD_NAVIGATION_

#include <stdint.h>
 
namespace VirtualPad
{
	namespace Configuration
	{
		/// <summary>
		/// Accept and Reject buttons configuration for navigation.
		/// All combinations of face buttons can be used.
		/// </summary>
		enum class NavigationEnum : uint8_t
		{
			/// <summary>
			/// A accept, B reject.
			/// </summary>
			AB = 0,

			/// <summary>
			/// A accept, X reject.
			/// </summary>
			AX = 1,

			/// <summary>
			/// A accept, Y reject.
			/// </summary>
			AY = 2,

			/// <summary>
			/// B accept, A reject.
			/// </summary>
			BA = 3,

			/// <summary>
			/// B accept, X reject.
			/// </summary>
			BX = 4,

			/// <summary>
			/// B accept, Y reject.
			/// </summary>
			BY = 5,

			/// <summary>
			/// X accept, A reject.
			/// </summary>
			XA = 6,

			/// <summary>
			/// X accept, B reject.
			/// </summary>
			XB = 7,

			EnumCount
		};
	}
}
#endif