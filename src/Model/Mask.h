#ifndef _VIRTUAL_PAD_MASK_h
#define _VIRTUAL_PAD_MASK_h

#include <stdint.h>

namespace VirtualPad
{
	namespace FlagMask
	{
		template<const uint8_t bitIndex>
		static constexpr uint16_t GetMask()
		{
			return ((uint16_t)1) << ((uint16_t)bitIndex);
		}

		template<const uint8_t bitIndex>
		static constexpr bool GetFlag(const uint16_t flags)
		{
			return flags & GetMask<bitIndex>();
		}

		template<const uint8_t bitIndex>
		void SetFlag(uint16_t& flags, const bool value)
		{
			if (value)
			{
				flags |= GetMask<bitIndex>();
			}
			else
			{
				flags &= ~GetMask<bitIndex>();
			}
		}
	}
}

#endif