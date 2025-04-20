#ifndef _VIRTUAL_PAD_FLAG_h
#define _VIRTUAL_PAD_FLAG_h

#include <stdint.h>

namespace VirtualPad
{
	/// <summary>
	/// Variadic template helper class.
	/// </summary>
	namespace TemplateFlag
	{
		template<typename... Parameters>
		static constexpr uint32_t FlagsOrIndex(const uint8_t first)
		{
			return ((uint32_t)1 << (uint8_t)first);
		}

		template<typename... Parameters>
		static constexpr uint32_t FlagsOrIndex(const uint8_t first, Parameters... parameters)
		{
			return ((uint32_t)1 << (uint8_t)first) | FlagsOrIndex(parameters...);
		}

		static constexpr uint32_t GetFlags()
		{
			return 0;
		}

		template<uint8_t...flags>
		static constexpr uint32_t GetFlags()
		{
			return FlagsOrIndex(flags...);
		}

		template<const uint8_t flag>
		static constexpr bool GetFlagEnabled(const uint32_t flags)
		{
			return flags & (((uint32_t)1) << ((uint8_t)flag));
		}
	}
}

#endif