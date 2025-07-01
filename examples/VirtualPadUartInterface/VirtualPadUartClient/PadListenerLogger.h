#ifndef _PAD_LISTENER_LOGGER_h
#define _PAD_LISTENER_LOGGER_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

template<typename pad_state_t = VirtualPad::analog_pad_state_t>
class PadListenerLogger : public VirtualPad::StateListener<pad_state_t>
{
public:
	PadListenerLogger()
	{
	}

	void Setup()
	{
		pinMode(LED_BUILTIN, OUTPUT);
		digitalWrite(LED_BUILTIN, LOW);
	}

	void OnUpdate(const pad_state_t& state) final
	{
		if (state.Connected())
		{
			if (!digitalRead(LED_BUILTIN))
			{
				digitalWrite(LED_BUILTIN, HIGH);
#ifdef DEBUG
				Serial.println();
				Serial.println(F("Pad connected."));
#endif
			}

#ifdef DEBUG
			Serial.println();
			Serial.println(F("Pad update."));
#endif
		}
		else
		{
			if (digitalRead(LED_BUILTIN))
			{
				digitalWrite(LED_BUILTIN, LOW);
#ifdef DEBUG
				Serial.println();
				Serial.println(F("Pad disconnected."));
#endif
			}
		}
	}
};
#endif

