// ProcessSupportScreen.h

#ifndef _PROCESSSUPPPORTSCREEN_h
#define _PROCESSSUPPPORTSCREEN_h


//#define DEBUG_MOCK_CONTROLLER
#ifdef DEBUG_LOG
//#define DEBUG_RENDER_TIMINGS
#endif

#define SCREEN_TARGET_PERIOD_MICROS (10000)
#define OLED_BRIGHTNESS 10

#define _TASK_OO_CALLBACKS

#include <TaskSchedulerDeclarations.h>

#include <IFrameRender.h>
#include <ColourHelper.h>

#include <RendererSSD13XX.h>




#if defined(ARDUINO_ARCH_STM32F1)
#define __CS1 31
#define __DC  22
#define __RST 26
#endif

//#define RENDER_RAW_TEXT_X		RENDER_ABSOLUTE_MARGIN
//#define RENDER_RAW_TEXT_Y		RENDER_ABSOLUTE_MARGIN
//
//#define RENDER_RAW_TEXT_MARGIN	2
//
//#define RENDER_RAW_DOT_SIZE		2
////#define RENDER_RAW_INPUT_RANGE	300
//
//#define RENDER_RAW_INPUT_BOTTOM	(0)
//#define RENDER_INPUT_BOTTOM		(0)
//#define RENDER_INPUT_TOP		(UINT16_MAX)

#include <IInputController.h>

template<
	const uint8_t SPI_Index>
	class ProcessSupportScreen : Task
{
private:
	SPIClass SPI_Instance; //Create an instance of the SPI Class called SPI_2 that uses the 2nd SPI Port


	static const uint16_t ScreenWidth = 96;
	static const uint16_t ScreenHeight = 64;


	static const int16_t BarWidth = 8;
	static const int16_t CrossSize = 11;

	static const int16_t ArrowHeight = 3;
	static const int16_t ArrowWidth = 6;


	static const int16_t RENDER_ABSOLUTE_MARGIN = 2;
	////#define RENDER_TEXT_HEIGHT		(6)
	static const int16_t RENDER_TOTAL_HEIGHT = ScreenHeight;
	static const int16_t RENDER_HALF_HEIGHT = RENDER_TOTAL_HEIGHT / 2;
	static const int16_t RENDER_TOTAL_WIDTH = ScreenWidth;
	static const int16_t RENDER_HALF_WIDTH = RENDER_TOTAL_WIDTH / 2;
	static const int16_t RENDER_RAW_DOT_SIZE = 2;
	static const int16_t RENDER_RAW_DOT_RANGE = RENDER_HALF_HEIGHT - (CrossSize / 2);

	////const int16_t RENDER_RAW_L_TEXT_X = (((RENDER_TOTAL_WIDTH * 1) / 4) - RENDER_ABSOLUTE_MARGIN - BarWidth);
	////const int16_t RENDER_RAW_R_TEXT_X = (((RENDER_TOTAL_WIDTH * 3) / 4) - RENDER_ABSOLUTE_MARGIN - BarWidth);
	////const int16_t RENDER_RAW_L_TEXT_Y = (((RENDER_TOTAL_HEIGHT * 4) / 4) - RENDER_TEXT_HEIGHT - RENDER_RAW_TEXT_MARGIN);



	static const int16_t BarHeight = RENDER_HALF_HEIGHT - RENDER_ABSOLUTE_MARGIN;
	static const int16_t BarStartY = RENDER_TOTAL_HEIGHT - BarHeight;
	static const int16_t BarLineWidth = RENDER_ABSOLUTE_MARGIN;


	static const uint8_t ButtonRadius = 3;
	static const uint16_t RawRange = 20;


	RendererSSD13XX<ScreenWidth, ScreenHeight, __CS1, __DC, __RST> FrameRenderer;

	IFrameRender<uint8_t, uint16_t>* Driver = nullptr;


	//uint16_t ColorGrunt = 0x0f00;
	static const uint16_t ColorWhite = ColourHelper::ToColour(0xFF, 0xFF, 0xFF);
	static const uint16_t ColorBlack = ColourHelper::ToColour(0x00, 0x00, 0x00);
	static const uint16_t ColorBlue = ColourHelper::ToColour(0xFF, 0x00, 0x00);
	static const uint16_t ColorGreen = ColourHelper::ToColour(0x00, 0xFF, 0x00);
	static const uint16_t ColorGreenDark = ColourHelper::ToColour(0x00, 0xA0, 0x00);
	static const uint16_t ColorRed = ColourHelper::ToColour(0xFF, 0x00, 0x00);
	static const uint16_t ColorRedDark = ColourHelper::ToColour(0x70, 0x00, 0x00);
	static const uint16_t ColorGrey = ColourHelper::ToColour(0xAA, 0xAA, 0xAA);
	static const uint16_t ColorYellow = ColourHelper::ToColour(0xFF, 0xE4, 0x00);
	static const uint16_t ColorYellowDark = ColourHelper::ToColour(0xAA, 0xAB, 0x00);
	static const uint16_t ColorPurple = ColourHelper::ToColour(0xa9, 0x70, 0xff);
	static const uint16_t ColorPurpleDark = ColourHelper::ToColour(0x4B, 0x32, 0x72);


	uint32_t RenderStarted = 0;
	uint32_t RenderDuration = 0;
	uint32_t PushStarted = 0;
	uint32_t PushDuration = 0;


	uint8_t ClearCounter = 0;


	IInputController* Controller = nullptr;


private:
	//static const GetRENDER_RAW_DOT_RANGE()
	//{
	//	return min((uint32_t)(RENDER_TOTAL_WIDTH / 2), (uint32_t)(RENDER_TOTAL_HEIGHT / 2)) - RENDER_ABSOLUTE_MARGIN - 5;
	//}

public:
	ProcessSupportScreen(Scheduler* scheduler, IInputController* controller)
		: Task(0, TASK_FOREVER, scheduler, false)
		, SPI_Instance(SPI_Index)
		, FrameRenderer(&SPI_Instance)
	{
		Controller = controller;
		Driver = &FrameRenderer;
	}



	void Debug(Stream* serial)
	{
		FrameRenderer.Debug(serial);
	}


	bool Setup()
	{
		if (!FrameRenderer.Setup(OLED_BRIGHTNESS))
		{

#ifdef DEBUG_LOG
			Serial.print(F("FrameRenderer error."));
#endif
			return false;
		}

		//SPI_Instance.setClockDivider(SPI_CLOCK_DIV2);


		Task::enable();

		return true;
	}

	void PrintName(Stream* serial)
	{
		serial->print(F("SupportScreen"));
	}

protected:
	bool OnEnable()
	{
#ifdef DEBUG_LOG
		Serial.println(F("Support screen active."));
#endif


		return true;
	}
	void OnDisable()
	{
#ifdef DEBUG_LOG
		Serial.println(F("Support screen disabled."));
#endif
	}

	//Draw dot helpers.
	//uint16_t X, Y;
	//int8_t dX, dY;


	//bool IsInDeadZone(const uint16_t x, const uint16_t y, const uint16_t radius)
	//{
	//	return (((x - INT8_MAX) * (x - INT8_MAX)) +
	//		((y - INT8_MAX) * (y - INT8_MAX))) < (radius * radius);
	//}

	void RenderTargetBackground(const uint16_t x, const uint16_t y)
	{
		uint16_t DeadZoneColor;
		if (x == UINT16_MAX / 2 &&
			y == UINT16_MAX / 2)
		{
			DeadZoneColor = ColorGreen;
		}
		else
		{
			DeadZoneColor = ColorRed;
		}

		// Draw target.
		Driver->DrawHorizontalLine(RENDER_HALF_WIDTH - RENDER_RAW_DOT_RANGE, RENDER_HALF_HEIGHT, RENDER_RAW_DOT_RANGE - (CrossSize / 2), ColorRed);
		Driver->DrawHorizontalLine(RENDER_HALF_WIDTH + (CrossSize / 2), RENDER_HALF_HEIGHT, RENDER_RAW_DOT_RANGE - (CrossSize / 2), ColorRed);
		Driver->DrawVerticalLine(RENDER_HALF_WIDTH, RENDER_HALF_HEIGHT - RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE - (CrossSize / 2), ColorRed);
		Driver->DrawVerticalLine(RENDER_HALF_WIDTH, RENDER_HALF_HEIGHT + (CrossSize / 2), RENDER_RAW_DOT_RANGE - (CrossSize / 2), ColorRed);

		// Draw border.
		Driver->DrawRectangle(RENDER_HALF_WIDTH - RENDER_RAW_DOT_RANGE - 1, RENDER_HALF_HEIGHT - RENDER_RAW_DOT_RANGE - 1, RENDER_RAW_DOT_RANGE * 2 + 3, RENDER_RAW_DOT_RANGE * 2 + 2, DeadZoneColor);
	}


	void RenderFilteredJoy(const uint16_t x, const uint16_t y)
	{
		int8_t dX = (int16_t)map(x, 0, UINT16_MAX, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);
		int8_t dY = (int16_t)map(min(y + 1, UINT16_MAX), UINT16_MAX, 0, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);

		if (x == UINT16_MAX / 2)
		{
			dX++;
		}

		if (y == UINT16_MAX / 2)
		{
			dY++;
		}

		Driver->DrawHorizontalLine(RENDER_HALF_WIDTH - (CrossSize / 2) + dX, RENDER_HALF_HEIGHT + dY, CrossSize, ColorWhite);
		Driver->DrawVerticalLine(RENDER_HALF_WIDTH + dX, RENDER_HALF_HEIGHT + dY - (CrossSize / 2), CrossSize, ColorWhite);
	}

	void RenderFilteredCJoy(const uint16_t x, const uint16_t y)
	{
		int8_t dX = (int16_t)map(x, 0, UINT16_MAX, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);
		int8_t dY = (int16_t)map(min(y + 1, UINT16_MAX), UINT16_MAX, 0, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);

		if (x == UINT16_MAX / 2)
		{
			dX++;
		}

		if (y == UINT16_MAX / 2)
		{
			dY++;
		}

		Driver->DrawHorizontalLine(RENDER_HALF_WIDTH - (CrossSize / 2) + dX, RENDER_HALF_HEIGHT + dY, CrossSize, ColorYellow);
		Driver->DrawVerticalLine(RENDER_HALF_WIDTH + dX, RENDER_HALF_HEIGHT + dY - (CrossSize / 2), CrossSize, ColorYellow);
	}

	void RenderTriggers(const uint16_t l, const uint16_t r, const bool lButton, const bool rButton)
	{
		int16_t BarStartX = 0;
		uint16_t ZoneColor;
		int16_t Height;
		int16_t ActualHeight;
		bool IsInDeadZone;

		// Left trigger.
		IsInDeadZone = l == 0;
		Height = map(l, UINT16_MAX, 0, 0, BarHeight - (BarLineWidth * 4));
		if (IsInDeadZone)
		{
			ZoneColor = ColorGreen;
		}
		else if (lButton)
		{
			ZoneColor = ColorRed;
		}
		else
		{
			ZoneColor = ColorGrey;
		}
		Driver->DrawRectangle(BarStartX, BarStartY, BarWidth, BarHeight - (BarLineWidth * 2), ZoneColor);
		Driver->FillRectangle(BarStartX + BarLineWidth, BarStartY + BarLineWidth + Height, BarWidth - (BarLineWidth * 2), BarHeight - Height - (BarLineWidth * 4), ColorRed);

		// Right trigger.
		BarStartX = RENDER_TOTAL_WIDTH - BarWidth - (BarLineWidth * 2);

		IsInDeadZone = r == 0;
		Height = map(r, UINT16_MAX, 0, 0, BarHeight - (BarLineWidth * 4));
		if (IsInDeadZone)
		{
			ZoneColor = ColorGreen;
		}
		else if (rButton)
		{
			ZoneColor = ColorRed;
		}
		else
		{
			ZoneColor = ColorGrey;
		}
		Driver->DrawRectangle(BarStartX, BarStartY, BarWidth, BarHeight - (BarLineWidth * 2), ZoneColor);
		Driver->FillRectangle(BarStartX + BarLineWidth, BarStartY + BarLineWidth + Height, BarWidth - (BarLineWidth * 2), BarHeight - Height - (BarLineWidth * 4), ColorRed);
	}

	void RenderButtons()
	{
		uint16_t ButtonColor;


		// Start
		if (Controller->GetButtonHome())
		{
			Driver->FillCircle(RENDER_HALF_WIDTH, RENDER_HALF_HEIGHT, RENDER_RAW_DOT_RANGE, ColorGrey);
		}

		// Z
		uint8_t Y = RENDER_ABSOLUTE_MARGIN * 2;

		if (Controller->GetButton4())
		{
			ButtonColor = ColorPurple;
		}
		else
		{
			ButtonColor = ColorPurpleDark;
		}
		Driver->DrawHorizontalLine(RENDER_TOTAL_WIDTH - (ButtonRadius * 4) - (RENDER_ABSOLUTE_MARGIN * 2) + 2, Y++, (ButtonRadius * 4) + RENDER_ABSOLUTE_MARGIN - 4, ButtonColor);
		Driver->DrawHorizontalLine(RENDER_TOTAL_WIDTH - (ButtonRadius * 4) - (RENDER_ABSOLUTE_MARGIN * 2) + 1, Y++, (ButtonRadius * 4) + RENDER_ABSOLUTE_MARGIN - 2, ButtonColor);
		Driver->DrawHorizontalLine(RENDER_TOTAL_WIDTH - (ButtonRadius * 4) - (RENDER_ABSOLUTE_MARGIN * 2) + 1, Y++, (ButtonRadius * 4) + RENDER_ABSOLUTE_MARGIN - 2, ButtonColor);

		// Y 
		Y += ButtonRadius + (ButtonRadius * 1);
		if (Controller->GetButton3())
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		Driver->FillCircle(RENDER_TOTAL_WIDTH - (RENDER_ABSOLUTE_MARGIN * 1) - (ButtonRadius * 4), Y, ButtonRadius - 1, ButtonColor);

		// X
		Y += RENDER_ABSOLUTE_MARGIN;
		if (Controller->GetButton2())
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		Driver->FillCircle(RENDER_TOTAL_WIDTH - RENDER_ABSOLUTE_MARGIN - (ButtonRadius * 1), Y - (ButtonRadius * 1), ButtonRadius - 1, ButtonColor);



		// A
		Y += RENDER_ABSOLUTE_MARGIN + 1;
		if (Controller->GetButton0())
		{
			ButtonColor = ColorGreen;
		}
		else
		{
			ButtonColor = ColorGreenDark;
		}
		Driver->FillCircle(RENDER_TOTAL_WIDTH - (RENDER_ABSOLUTE_MARGIN * 1) - 3, Y + ButtonRadius, ButtonRadius, ButtonColor);

		// B
		Y += 1 + (ButtonRadius * 2);
		if (Controller->GetButton1())
		{
			ButtonColor = ColorRed;
		}
		else
		{
			ButtonColor = ColorRedDark;
		}
		Driver->FillCircle(RENDER_TOTAL_WIDTH - (RENDER_ABSOLUTE_MARGIN * 4) - (ButtonRadius * 2), Y, ButtonRadius - 2, ButtonColor);


		const uint8_t DPadSize = 20;
		const uint8_t DPadButtonSize = (DPadSize / 3) - 1;

		uint8_t DPadX = RENDER_ABSOLUTE_MARGIN + (DPadSize / 3 ) -1;
		uint8_t DPadY = 4 + RENDER_ABSOLUTE_MARGIN + (DPadSize / 2);


		bool Left, Right, Up, Down;
		Controller->GetDirection(Left, Right, Up, Down);
		if (Up)
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		Driver->FillRectangle(DPadX, DPadY - DPadButtonSize, DPadButtonSize, DPadButtonSize, ButtonColor);

		if (Down)
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		Driver->FillRectangle(DPadX, DPadY + 1, DPadButtonSize, DPadButtonSize, ButtonColor);


		if (Left)
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		Driver->FillRectangle(DPadX - DPadButtonSize - 1, DPadY - (DPadButtonSize / 2), DPadButtonSize, DPadButtonSize, ButtonColor);
		//x = RENDER_ABSOLUTE_MARGIN;
		//y = RENDER_HALF_HEIGHT - (ArrowHeight * 2) - (RENDER_ABSOLUTE_MARGIN * 2) - 1;
		//Driver->FillTriangle(x, y + (ArrowWidth / 2), x + ArrowHeight, y, x + ArrowHeight, y + ArrowWidth, ButtonColor);

		if (Right)
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		Driver->FillRectangle(DPadX + DPadButtonSize + 1, DPadY - (DPadButtonSize / 2), DPadButtonSize, DPadButtonSize, ButtonColor);

		//x += (RENDER_ABSOLUTE_MARGIN * 2) + ArrowHeight + ArrowWidth;
		//Driver->FillTriangle(x, y, x + ArrowHeight, y + (ArrowWidth / 2), x, y + ArrowWidth, ButtonColor);

	}

	enum RenderStateEnum : uint8_t
	{
		ClearBuffer = 0,
		RenderGraphics = 1,
		//PushBuffer = 2,
		PushBufferStaggered = 2,
		RenderStateEnumCount
	} RenderState = RenderStateEnum::ClearBuffer;

	enum PushStateEnum : uint8_t
	{
		PrepareIt = 0,
		DoIt = 1,
		CloseIt = 2,
		DoneWithIt = 3,
		PushStateEnumCount
	} PushState = PushStateEnum::PrepareIt;


	uint32_t FrameStart, FrameFinish, FrameDuration, RenderHelper;
	uint32_t LastDebug = 0;

public:
	bool Callback()
	{
		switch (RenderState)
		{
		case  RenderStateEnum::ClearBuffer:
			RenderStarted = micros();
			Driver->ClearBuffer();
			RenderState = RenderStateEnum::RenderGraphics;
			Task::forceNextIteration();
			RenderDuration = 0;
			break;
		case  RenderStateEnum::RenderGraphics:
			RenderHelper = micros();
			if (Render())
			{
				PushState = PushStateEnum::PrepareIt;
				RenderState = RenderStateEnum::PushBufferStaggered;
			}
			RenderDuration += micros() - RenderHelper;
			Task::forceNextIteration();
			break;
			/*case  RenderStateEnum::PushBuffer:
				PushStarted = micros();
				Driver->PushFrame();
				PushDuration = micros() - PushStarted;
				RenderState = RenderStateEnum::ClearBuffer;
				forceNextIteration();
	#ifdef DEBUG_RENDER_TIMINGS
				FrameDuration = micros() - FrameStart;
				FrameStart = micros();

				if (millis() - LastDebug >= 1000)
				{
					LastDebug = millis();
					Serial.print(((float)1000000 / (float)max(FrameDuration, (uint32_t)1)), 2);
					Serial.println(F(" FPS"));
					Serial.print(F("RenderDuration: "));
					Serial.print(RenderDuration);
					Serial.println(F(" us"));
					Serial.print(F("PushDuration: "));
					Serial.print(PushDuration);
					Serial.println(F(" us"));
					Serial.print(F("FrameDuration: "));
					Serial.print(FrameDuration);
					Serial.println(F(" us"));
				}
	#endif

				if (FrameDuration < SCREEN_TARGET_PERIOD_MICROS &&
					RenderDuration < SCREEN_TARGET_PERIOD_MICROS)
				{
					Task::delay(((RenderStarted + SCREEN_TARGET_PERIOD_MICROS) - micros()) / 1000);
				}
				else
				{
					Task::delay(1);
				}

				break;*/
		case  RenderStateEnum::PushBufferStaggered:
			switch (PushState)
			{
			case PushStateEnum::PrepareIt:
				PushStarted = micros();
				Driver->PushFrame1();
				PushDuration = micros() - PushStarted;
				PushState = PushStateEnum::DoIt;
				Task::forceNextIteration();
				break;
			case PushStateEnum::DoIt:
				PushStarted = micros();
				Driver->PushFrame2();
				PushDuration += micros() - PushStarted;
				PushState = PushStateEnum::CloseIt;
				Task::delay(6);
				break;
			case PushStateEnum::CloseIt:
				PushStarted = micros();
				Driver->PushFrame3();
				PushDuration += micros() - PushStarted;
				PushState = PushStateEnum::DoneWithIt;
				FrameFinish = micros();
				Task::forceNextIteration();
				break;
			case PushStateEnum::DoneWithIt:
				RenderState = RenderStateEnum::ClearBuffer;
				RenderHelper = micros() - RenderStarted;


				if (RenderHelper < SCREEN_TARGET_PERIOD_MICROS)
				{
					Task::delay(max((uint32_t)1, (SCREEN_TARGET_PERIOD_MICROS - RenderHelper) / 1000));
				}
				else
				{
					Task::delay(1);
				}


#ifdef DEBUG_RENDER_TIMINGS
				FrameDuration = micros() - FrameStart;
				FrameStart = micros();
				if (millis() - LastDebug >= 1000)
				{
					LastDebug = millis();
					Serial.print((float)(1000000 / max((uint32_t)FrameDuration, (uint32_t)1)), 2);
					Serial.println(F(" FPS"));
					Serial.print(F("RenderDuration: "));
					Serial.print(RenderDuration);
					Serial.println(F(" us"));
					Serial.print(F("PushDuration: "));
					Serial.print(PushDuration);
					Serial.println(F(" us"));
					Serial.print(F("FrameDuration: "));
					Serial.print(FrameDuration);
					Serial.println(F(" us"));
					Serial.print(F("SleepDuration: "));
					Serial.print(SCREEN_TARGET_PERIOD_MICROS - FrameDuration);
					Serial.println(F(" us"));
				}
#endif
				break;
			default:
				break;
			}

			break;
		default:
			RenderState = RenderStateEnum::ClearBuffer;
			Driver->ClearBuffer();
			break;
		}

		/*RenderState = RenderState + 1;

		if (RenderState >= RenderStateEnumCount)
		{
			RenderState = 0;
		}*/

		return true;
	}

	void RenderRumble(const bool on)
	{
		//if (on)
		//{
		//	Driver.setBackground(ColorGreyDark);
		//}
		//else
		//{
		//	Driver.setBackground(ColorBlack);
		//}
	}


	uint8_t RenderPass = 0;
	const uint8_t RenderPassCount = 4;
	bool Render()
	{
		switch (RenderPass)
		{
		case 0:
			RenderTargetBackground(Controller->GetJoy1X(), Controller->GetJoy1Y());
			break;
		case 1:
			RenderTriggers(Controller->GetTriggerL(), Controller->GetTriggerR(), Controller->GetButton5(), Controller->GetButton6());
			break;
		case 2:
			RenderButtons();
			break;
		case 3:
			RenderFilteredCJoy(Controller->GetJoy2X(), Controller->GetJoy2Y());
			RenderFilteredJoy(Controller->GetJoy1X(), Controller->GetJoy1Y());
			//Driver->DrawRectangle(0, 0, Driver->GetWidth(), Driver->GetHeight(), 0x0F0B);
			//Driver->DrawRectangle(20, 20, 10, 20, 0x0FAB);
			//Driver->FillRectangle(50, 20, 10, 20, 0x0FAB);
			break;
		default:
			break;
		}

		RenderPass++;

		if (RenderPass >= RenderPassCount)
		{
			RenderPass = 0;
			return true;
		}
		else
		{
			return false;
		}

		//RenderRumble(Controller->GetRumbleDACOutput());

	}
};
#endif