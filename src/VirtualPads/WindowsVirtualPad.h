#ifndef _WINDOWS_VIRTUAL_PAD_h
#define _WINDOWS_VIRTUAL_PAD_h

#if defined(_WINDOWS_)

#include <cstdint>
#include <cmath>
#include <VirtualPad.h>

// Include Windows.Gaming.Input with compatibility for different SDK versions.
#if defined(__has_include)
#if __has_include(<winrt/Windows.Gaming.Input.h>)
#include <winrt/Windows.Gaming.Input.h>
namespace VIRTUALPAD_WINDOWSPAD_NAMESPACE = winrt::Windows::Gaming::Input;
#else
#include <windows.h>
namespace VIRTUALPAD_WINDOWSPAD_NAMESPACE = Windows::Gaming::Input;
#endif
#else
#include <windows.h>
namespace VIRTUALPAD_WINDOWSPAD_NAMESPACE = Windows::Gaming::Input;
#endif

namespace VirtualPad
{
	namespace WindowsPad
	{
		/// <summary>
		/// Standard configuration code for Xbox controller.
		/// </summary>
		static constexpr uint32_t ConfigurationCode = VirtualPad::Configuration::XBoxControllerConfigCode<>();

		using VirtualPadType = VirtualPad::TemplateVirtualPad<VirtualPad::analog_pad_state_t, ConfigurationCode>;

		struct VirtualPadUpdater : public VirtualPadType
		{
			VirtualPadUpdater() : VirtualPadType() {}

			void OnGamepadInput(const VIRTUALPAD_WINDOWSPAD_NAMESPACE::GamepadReading& reading)
			{
				using namespace VIRTUALPAD_WINDOWSPAD_NAMESPACE;

				SetSelect((reading.Buttons & GamepadButtons::View) != GamepadButtons::None);
				SetStart((reading.Buttons & GamepadButtons::Menu) != GamepadButtons::None);
				SetP1((reading.Buttons & GamepadButtons::Paddle1) != GamepadButtons::None);
				SetP2((reading.Buttons & GamepadButtons::Paddle2) != GamepadButtons::None);

				SetA((reading.Buttons & GamepadButtons::A) != GamepadButtons::None);
				SetB((reading.Buttons & GamepadButtons::B) != GamepadButtons::None);
				SetX((reading.Buttons & GamepadButtons::X) != GamepadButtons::None);
				SetY((reading.Buttons & GamepadButtons::Y) != GamepadButtons::None);

				SetL1((reading.Buttons & GamepadButtons::LeftShoulder) != GamepadButtons::None);
				SetR1((reading.Buttons & GamepadButtons::RightShoulder) != GamepadButtons::None);

				SetL3((reading.Buttons & GamepadButtons::LeftThumbstick) != GamepadButtons::None);
				SetR3((reading.Buttons & GamepadButtons::RightThumbstick) != GamepadButtons::None);

				SetDPad(
					(reading.Buttons & GamepadButtons::DPadUp) != GamepadButtons::None,
					(reading.Buttons & GamepadButtons::DPadDown) != GamepadButtons::None,
					(reading.Buttons & GamepadButtons::DPadLeft) != GamepadButtons::None,
					(reading.Buttons & GamepadButtons::DPadRight) != GamepadButtons::None
				);

				SetJoy1(NormalizeJoy(reading.LeftThumbstickX), NormalizeJoy(reading.LeftThumbstickY));
				SetJoy2(NormalizeJoy(reading.RightThumbstickX), NormalizeJoy(reading.RightThumbstickY));
				SetL2(NormalizeTrigger(reading.LeftTrigger));
				SetR2(NormalizeTrigger(reading.RightTrigger));

				SetConnected(true);
			}

		private:
			template<typename T>
			static T Clamp(T value, T lo, T hi)
			{
				return (value < lo) ? lo : (value > hi) ? hi : value;
			}

			static int16_t NormalizeJoy(double value)
			{
				// Map [-1.0, +1.0] to [INT16_MIN, INT16_MAX]
				// Use asymmetric magnitude: positive max is INT16_MAX (32767), negative full range uses 32768.
				const double multiplier = (value >= 0.0) ? static_cast<double>(INT16_MAX) : -static_cast<double>(INT16_MIN);
				const double scaled = std::round(value * multiplier);
				const int32_t clamped = Clamp<int32_t>(static_cast<int32_t>(scaled), static_cast<int32_t>(INT16_MIN), static_cast<int32_t>(INT16_MAX));
				return static_cast<int16_t>(clamped);
			}

			static uint16_t NormalizeTrigger(double value)
			{
				// Map [0.0, 1.0] to [0, UINT16_MAX]
				const double scaled = std::round(value * static_cast<double>(UINT16_MAX));
				const uint32_t clamped = Clamp<uint32_t>(static_cast<uint32_t>((scaled < 0.0) ? 0u : static_cast<uint32_t>(scaled)), 0u, static_cast<uint32_t>(UINT16_MAX));
				return static_cast<uint16_t>(clamped);
			}
		};
	}
}
#endif
#endif