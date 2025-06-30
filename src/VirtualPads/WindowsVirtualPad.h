#ifndef _WINDOWS_VIRTUAL_PAD_h
#define _WINDOWS_VIRTUAL_PAD_h

#if defined(_WINDOWS_)

#include <windows.h>
#include <VirtualPad.h>

namespace VirtualPad
{
	namespace WindowsPad
	{
		using namespace Windows::Gaming::Input;

		static constexpr uint32_t ConfigurationCode = VirtualPad::Configuration::XBoxControllerConfigCode<>();

		using VirtualPadType = VirtualPad::TemplateVirtualPad<VirtualPad::analog_pad_state_t, ConfigurationCode>;

		class VirtualPadUpdater : public VirtualPadType
		{
		public:
			VirtualPadUpdater() : VirtualPadType() {}

			void OnGamepadInput(Windows::Gaming::Input::GamepadReading reading)
			{
				SetSelect(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::View);
				SetStart(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::Menu);
				SetP1(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::Paddle1);
				SetP2(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::Paddle2);

				SetA(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::A);
				SetB(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::B);
				SetX(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::X);
				SetY(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::Y);
				SetL1(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::LeftShoulder);
				SetR1(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::RightShoulder);

				SetL3(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::LeftThumbstick);
				SetR3(uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::RightThumbstick);

				SetDPad(
					uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::DPadUp,
					uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::DPadDown,
					uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::DPadLeft,
					uint32_t(reading.Buttons) & (uint32_t)Windows::Gaming::Input::GamepadButtons::DPadRight
				);

				SetJoy1(NormalizeJoy(reading.LeftThumbstickX), NormalizeJoy(reading.LeftThumbstickY));
				SetJoy2(NormalizeJoy(reading.RightThumbstickX), NormalizeJoy(reading.RightThumbstickY));
				SetL2(NormalizeTrigger(reading.LeftTrigger));
				SetR2(NormalizeTrigger(reading.RightTrigger));
				SetConnected(true);
			}

		private:
			static int16_t NormalizeJoy(const double value)
			{
				if (value >= 0)
				{
					return IntegerSignal::MinValue((int32_t)INT16_MAX, (int32_t)round(value * double(INT16_MAX)));
				}
				else
				{
					return IntegerSignal::MaxValue((int32_t)INT16_MIN, (int32_t)round(-value * double(INT16_MIN)));
				}
			}

			static int16_t NormalizeTrigger(const double value)
			{
				return IntegerSignal::MinValue((uint32_t)UINT16_MAX, (uint32_t)round(value * double(UINT16_MAX)));
			}
		};
	}
}
#endif

#endif