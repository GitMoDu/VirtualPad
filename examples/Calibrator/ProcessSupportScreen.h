// ProcessSupportScreen.h

#ifndef _PROCESSSUPPPORTSCREEN_h
#define _PROCESSSUPPPORTSCREEN_h


#define _TASK_OO_CALLBACKS

#include <TaskSchedulerDeclarations.h>


#include <SPI.h>
#include <SSD_13XX.h>


//#define DEBUG_MOCK_CONTROLLER

#define SCREEN_TARGET_PERIOD 4
#define SCREEN_SKIP_CLEAR_MAX 1
#define SCREEN_SKIP_ALPHA (1 * (max(SCREEN_SKIP_CLEAR_MAX,1)))

#define SUPPORT_SCREEN_SPI_RETRY_PERIOD 5

#define OLED_BRIGHTNESS 10


#if defined(ARDUINO_ARCH_STM32)
#define __CS1 31
#define __DC  22
#define __RST 26
#endif


#define SCREEN_WIDTH  Driver.cgWidth()
#define SCREEN_HEIGHT Driver.cgHeight()

#define RENDER_ABSOLUTE_MARGIN	2
#define RENDER_TEXT_HEIGHT		6
#define RENDER_TOTAL_HEIGHT		SCREEN_HEIGHT
#define RENDER_HALF_HEIGHT		(RENDER_TOTAL_HEIGHT/2)
#define RENDER_TOTAL_WIDTH		SCREEN_WIDTH
#define RENDER_HALF_WIDTH		(RENDER_TOTAL_WIDTH/2)



#define RENDER_RAW_TEXT_X		RENDER_ABSOLUTE_MARGIN
#define RENDER_RAW_TEXT_Y		RENDER_ABSOLUTE_MARGIN

#define RENDER_RAW_TEXT_MARGIN	2

#define RENDER_RAW_DOT_SIZE		2
//#define RENDER_RAW_INPUT_RANGE	300

#define RENDER_RAW_DOT_RANGE	(min(RENDER_HALF_WIDTH, RENDER_HALF_HEIGHT) - RENDER_ABSOLUTE_MARGIN - 5)
#define RENDER_RAW_INPUT_BOTTOM	(0)
#define RENDER_INPUT_BOTTOM		(0)
#define RENDER_INPUT_TOP		(UINT16_MAX)

#include <IInputController.h>


SPIClass SPI_Instance(2); //Create an instance of the SPI Class called SPI_2 that uses the 2nd SPI Port

template<
	typename Calibration>
	class ProcessSupportScreen : Task
{
private:

	const int16_t BarWidth = 8;
	const int16_t RENDER_RAW_L_TEXT_X = (((RENDER_TOTAL_WIDTH * 1) / 4) - RENDER_ABSOLUTE_MARGIN - BarWidth);
	const int16_t RENDER_RAW_R_TEXT_X = (((RENDER_TOTAL_WIDTH * 3) / 4) - RENDER_ABSOLUTE_MARGIN - BarWidth);
	const int16_t RENDER_RAW_L_TEXT_Y = (((RENDER_TOTAL_HEIGHT * 4) / 4) - RENDER_TEXT_HEIGHT - RENDER_RAW_TEXT_MARGIN);


	const int16_t CrossSize = 11;

	const int16_t ArrowHeight = 3;
	const int16_t ArrowWidth = 6;

	const int16_t BarHeight = RENDER_HALF_HEIGHT - RENDER_ABSOLUTE_MARGIN;
	const int16_t BarStartY = RENDER_TOTAL_HEIGHT - BarHeight;
	const int16_t BarLineWidth = RENDER_ABSOLUTE_MARGIN;


	const uint8_t ButtonRadius = 3;


	SSD_13XX Driver = SSD_13XX(&SPI_Instance, __CS1, __DC, __RST);

	uint16_t ColorGrunt = 0x0f00;
	uint16_t ColorWhite, ColorBlack, ColorGreen, ColorRed, ColorRedDark, ColorGrey, ColorGreyDark, ColorGreenDark, ColorYellow, ColorYellowDark, ColorPurple, ColorPurpleDark;
	uint32_t RenderStarted = 0;


	uint8_t ClearCounter = 0;


	IInputController* Controller = nullptr;

	GameCubeData_t RawData;

	const uint16_t RawRange = Calibration::GamecubeAxisRange - 1;

public:
	ProcessSupportScreen(Scheduler* scheduler, IInputController* controller)
		: Task(SCREEN_TARGET_PERIOD, TASK_FOREVER, scheduler, true)
	{
		ColorWhite = Driver.Color565(0xFF, 0xFF, 0xFF);
		ColorBlack = Driver.Color565(0x00, 0x00, 0x00);
		ColorGreen = Driver.Color565(0x00, 0xC0, 0x00);
		ColorGreenDark = Driver.Color565(0x00, 0x50, 0x00);
		ColorRed = Driver.Color565(0xFF, 0x00, 0x00);
		ColorRedDark = Driver.Color565(0x30, 0x00, 0x00);
		ColorGrey = Driver.Color565(0x50, 0x50, 0x50);
		ColorGreyDark = Driver.Color565(0x10, 0x10, 0x10);
		ColorYellow = Driver.Color565(0xFF, 0xE6, 0x00);
		ColorYellowDark = Driver.Color565(0x3B, 0x50, 0x00);
		ColorPurple = Driver.Color565(0x59, 0x00, 0xff);
		ColorPurpleDark = Driver.Color565(0x14, 0x00, 0x38);

		Controller = controller;


	}

	bool Setup()
	{

		/*if (== nullptr)
		{
			return false;
		}
*/
		Driver.begin(false);

		uint8_t errorCode = Driver.getErrorCode();
		if (errorCode != 0)
		{
#ifdef DEBUG_LOG
			Serial.print("Support screen error.");
			if (bitRead(errorCode, 0)) Serial.print("MOSI or SCLK pin mismach!\n");
			if (bitRead(errorCode, 1)) Serial.print("CS or DC pin mismach!\n");
#endif
			return false;
		}


		Driver.setBrightness(OLED_BRIGHTNESS);
		Driver.changeMode(SSD_13XX_modes::DISP_ON);

		Driver.setBackground(Driver.Color565(0, 0, 0));
		Driver.clearScreen();

		randomSeed(micros());

		enableIfNot();

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
	uint16_t X, Y;
	int8_t dX, dY;


	bool IsInDeadZone(const uint16_t x, const uint16_t y, const uint16_t radius)
	{
		return (((x - (RawRange / 2)) * (x - (RawRange / 2))) +
			((y - (RawRange / 2)) * (y - (RawRange / 2)))) < (radius * radius);
	}

	void RenderTargetBackground(GameCubeData_t& rawData)
	{
		uint16_t DeadZoneColor;
		if (IsInDeadZone(rawData.joystickX, rawData.joystickY, Calibration::JoyDeadZoneRadius))
		{
			DeadZoneColor = ColorGreenDark;
		}
		else
		{
			DeadZoneColor = ColorRed;
		}

		////Draw target.
		if (Transparent == 0)
		{
			Driver.drawFastHLine(0, RENDER_HALF_HEIGHT, RENDER_TOTAL_WIDTH, DeadZoneColor);
			Driver.drawFastVLine(RENDER_HALF_WIDTH, 0, RENDER_TOTAL_HEIGHT, DeadZoneColor);
			Driver.drawCircle(RENDER_HALF_WIDTH, RENDER_HALF_HEIGHT, RENDER_RAW_DOT_RANGE, DeadZoneColor);
		}

		//Draw DeadZone.
		uint16_t Radius = map(Calibration::JoyDeadZoneRadius, 0, RawRange, 1, RENDER_RAW_DOT_RANGE);
		Driver.fillCircle(RENDER_HALF_WIDTH, RENDER_HALF_HEIGHT, Radius, ColorGrey);
	}

	////Joystick.
	AxisCentered<uint8_t, uint16_t, Calibration::JoyXMin, Calibration::JoyXMax, Calibration::JoyXOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1X;
	AxisCentered<uint8_t, uint16_t, Calibration::JoyYMin, Calibration::JoyYMax, Calibration::JoyYOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1Y;

	////Joystick C.
	AxisCentered<uint8_t, uint16_t, Calibration::JoyCXMin, Calibration::JoyCXMax, Calibration::JoyCXOffset, Calibration::JoyCDeadZoneRadius, 0, UINT16_MAX> AxisJoy2X;
	AxisCentered<uint8_t, uint16_t, Calibration::JoyCYMin, Calibration::JoyCYMax, Calibration::JoyCYOffset, Calibration::JoyCDeadZoneRadius, 0, UINT16_MAX> AxisJoy2Y;


	void RenderRawInputJoy(GameCubeData_t& rawData, bool showText)
	{
		const int16_t RENDER_RAW_JOY_TEXT_X = RENDER_ABSOLUTE_MARGIN;

		//Draw raw dot.
		X = constrain(AxisJoy1X.Parse(rawData.joystickX), 0, UINT16_MAX);
		Y = constrain(AxisJoy1Y.Parse(rawData.joystickY), 0, UINT16_MAX);
		dX = map(X, 0, UINT16_MAX, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);
		dY = map(Y, UINT16_MAX, 0, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);

		Driver.fillCircle(RENDER_HALF_WIDTH + dX, RENDER_HALF_HEIGHT + dY, RENDER_RAW_DOT_SIZE, ColorWhite);

		if (showText)
		{
			Driver.setTextColor(ColorWhite);
			Driver.setCursor(RENDER_RAW_JOY_TEXT_X, RENDER_RAW_TEXT_Y);
			Driver.print(rawData.joystickX);
			Driver.setCursor(RENDER_RAW_JOY_TEXT_X, RENDER_RAW_TEXT_Y + RENDER_TEXT_HEIGHT + RENDER_RAW_TEXT_MARGIN);
			Driver.print(rawData.joystickY);
		}
	}

	void RenderRawInputCJoy(GameCubeData_t& rawData, bool showText)
	{
		const int16_t RENDER_RAW_C_TEXT_X = (((RENDER_TOTAL_WIDTH * 3) / 4) - RENDER_RAW_TEXT_MARGIN);

		//Draw raw dot.
		X = constrain(AxisJoy2X.Parse(rawData.cX), 0, UINT16_MAX);
		Y = constrain(AxisJoy2Y.Parse(rawData.cY), 0, UINT16_MAX);
		dX = map(X, 0, UINT16_MAX, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);
		dY = map(Y, UINT16_MAX, 0, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);

		////Draw raw dot.
		//X = constrain(rawData.cX, RENDER_RAW_INPUT_BOTTOM, RawRange);
		//Y = constrain(rawData.cY, RENDER_RAW_INPUT_BOTTOM, RawRange);
		//dX = map(X, RENDER_RAW_INPUT_BOTTOM, RawRange, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);
		//dY = map(Y, RawRange, RENDER_RAW_INPUT_BOTTOM, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);

		Driver.fillCircle(RENDER_HALF_WIDTH + dX, RENDER_HALF_HEIGHT + dY, RENDER_RAW_DOT_SIZE, ColorYellow);

		if (showText)
		{
			Driver.setTextColor(ColorWhite);
			Driver.setCursor(RENDER_RAW_C_TEXT_X, RENDER_RAW_TEXT_Y);
			Driver.print(rawData.cX);
			Driver.setCursor(RENDER_RAW_C_TEXT_X, RENDER_RAW_TEXT_Y + RENDER_TEXT_HEIGHT + RENDER_RAW_TEXT_MARGIN);
			Driver.print(rawData.cY);
		}
	}

	void RenderFilteredJoy(const uint16_t x, const uint16_t y)
	{
		//Draw processed cross indicator.
		dX = map(x, 0, UINT16_MAX, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);
		dY = map(y, UINT16_MAX, 0, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);

		Driver.drawFastHLine(RENDER_HALF_WIDTH - (CrossSize / 2) + dX, RENDER_HALF_HEIGHT + dY, CrossSize, ColorWhite);
		Driver.drawFastVLine(RENDER_HALF_WIDTH + dX, RENDER_HALF_HEIGHT + dY - (CrossSize / 2), CrossSize, ColorWhite);
	}

	void RenderFilteredCJoy(const uint16_t x, const uint16_t y)
	{
		dX = map(x, 0, UINT16_MAX, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);
		dY = map(y, UINT16_MAX, 0, -RENDER_RAW_DOT_RANGE, RENDER_RAW_DOT_RANGE);

		Driver.drawFastHLine(RENDER_HALF_WIDTH - (CrossSize / 2) + dX, RENDER_HALF_HEIGHT + dY, CrossSize, ColorYellow);
		Driver.drawFastVLine(RENDER_HALF_WIDTH + dX, RENDER_HALF_HEIGHT + dY - (CrossSize / 2), CrossSize, ColorYellow);
	}

	void RenderTriggers(GameCubeData_t& rawData, bool showText, const uint16_t l, const uint16_t r, const bool lButton, const bool rButton)
	{
		int16_t BarStartX = 0;
		uint16_t DeadZoneColor;
		int16_t Height;
		int16_t ActualHeight;
		bool IsInDeadZone;
		// Left trigger.
		IsInDeadZone = l == 0;
		Height = map(l, UINT16_MAX, 0, 0, BarHeight);

		if (IsInDeadZone)
		{
			DeadZoneColor = ColorGreen;
		}
		else if (lButton)
		{
			DeadZoneColor = ColorRed;
		}
		else
		{
			DeadZoneColor = ColorGrey;
		}
		Driver.fillRect(BarStartX, BarStartY, BarWidth, BarHeight, DeadZoneColor);
		Driver.fillRect(BarStartX + BarLineWidth, BarStartY + BarLineWidth, BarWidth - (BarLineWidth * 2), BarHeight - (BarLineWidth * 2), ColorBlack);

		ActualHeight = BarHeight - Height - (BarLineWidth * 2);

		if (ActualHeight > 0)
		{
			Driver.fillRect(BarStartX + BarLineWidth, BarStartY + Height + BarLineWidth, BarWidth - (BarLineWidth * 2), ActualHeight, ColorRed);
		}

		// Right trigger.
		BarStartX = RENDER_TOTAL_WIDTH - BarWidth;
		IsInDeadZone = r == 0;
		Height = map(r, UINT16_MAX, 0, 0, BarHeight);


		if (IsInDeadZone)
		{
			DeadZoneColor = ColorGreen;
		}
		else if (rButton)
		{
			DeadZoneColor = ColorRed;
		}
		else
		{
			DeadZoneColor = ColorGrey;
		}
		Driver.fillRect(BarStartX, BarStartY, BarWidth, BarHeight, DeadZoneColor);
		Driver.fillRect(BarStartX + BarLineWidth, BarStartY + BarLineWidth, BarWidth - (BarLineWidth * 2), BarHeight - (BarLineWidth * 2), ColorBlack);

		ActualHeight = BarHeight - Height - (BarLineWidth * 2);
		if (ActualHeight > 0)
		{
			Driver.fillRect(BarStartX + BarLineWidth, BarStartY + Height + BarLineWidth, BarWidth - (BarLineWidth * 2), ActualHeight, ColorRed);
		}

		if (showText)
		{
			Driver.setTextColor(ColorWhite);
			Driver.setCursor(RENDER_RAW_L_TEXT_X, RENDER_RAW_L_TEXT_Y);
			Driver.print(rawData.shoulderLeft);
			Driver.setCursor(RENDER_RAW_R_TEXT_X, RENDER_RAW_L_TEXT_Y);
			Driver.print(rawData.shoulderRight);
		}
	}

	void RenderButtons()
	{
		uint16_t ButtonColor;

		
		// Start
		if (Controller->GetButton7())
		{
			Driver.fillCircle(RENDER_HALF_WIDTH, RENDER_HALF_HEIGHT, RENDER_RAW_DOT_RANGE - RENDER_ABSOLUTE_MARGIN, ColorWhite);

		}

		if (Controller->GetButton0())
		{
			ButtonColor = ColorGreen;
		}
		else
		{
			ButtonColor = ColorGreenDark;
		}
		Driver.fillCircle(RENDER_TOTAL_WIDTH - BarWidth - (RENDER_ABSOLUTE_MARGIN * 2) - ButtonRadius, RENDER_HALF_HEIGHT + RENDER_ABSOLUTE_MARGIN + ButtonRadius, ButtonRadius, ButtonColor);

		if (Controller->GetButton1())
		{
			ButtonColor = ColorRed;
		}
		else
		{
			ButtonColor = ColorRedDark;
		}
		Driver.fillCircle(RENDER_TOTAL_WIDTH - BarWidth - RENDER_ABSOLUTE_MARGIN - (ButtonRadius * 2), RENDER_HALF_HEIGHT + (RENDER_ABSOLUTE_MARGIN * 2) + (ButtonRadius * 2) + 3, ButtonRadius - 1, ButtonColor);


		if (Controller->GetButton2())
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		Driver.fillCircle(RENDER_TOTAL_WIDTH - RENDER_ABSOLUTE_MARGIN - (ButtonRadius * 1), RENDER_HALF_HEIGHT - (ButtonRadius * 2) + 1, ButtonRadius - 1, ButtonColor);

		if (Controller->GetButton3())
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		Driver.fillCircle(RENDER_TOTAL_WIDTH - (RENDER_ABSOLUTE_MARGIN * 1) - (ButtonRadius * 4), RENDER_HALF_HEIGHT - (ButtonRadius * 2) + 1, ButtonRadius - 1, ButtonColor);

		if (Controller->GetButton4())
		{
			ButtonColor = ColorPurple;
		}
		else
		{
			ButtonColor = ColorPurpleDark;
		}
		Driver.drawFastHLine(RENDER_TOTAL_WIDTH - (ButtonRadius * 4) - (RENDER_ABSOLUTE_MARGIN * 2) + 2, RENDER_HALF_HEIGHT - (ButtonRadius * 2) - (RENDER_ABSOLUTE_MARGIN * 2) - 2, (ButtonRadius * 4) + RENDER_ABSOLUTE_MARGIN - 4, ButtonColor);
		Driver.drawFastHLine(RENDER_TOTAL_WIDTH - (ButtonRadius * 4) - (RENDER_ABSOLUTE_MARGIN * 2) + 1, RENDER_HALF_HEIGHT - (ButtonRadius * 2) - (RENDER_ABSOLUTE_MARGIN * 2) - 1, (ButtonRadius * 4) + RENDER_ABSOLUTE_MARGIN - 2, ButtonColor);
		Driver.drawFastHLine(RENDER_TOTAL_WIDTH - (ButtonRadius * 4) - (RENDER_ABSOLUTE_MARGIN * 2) + 1, RENDER_HALF_HEIGHT - (ButtonRadius * 2) - (RENDER_ABSOLUTE_MARGIN * 2), (ButtonRadius * 4) + RENDER_ABSOLUTE_MARGIN - 2, ButtonColor);

		int x, y;
		bool Left, Right, Up, Down;
		Controller->GetDirection(Left,Right, Up, Down);
		if (Up)
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		x = (RENDER_ABSOLUTE_MARGIN * 2) + ArrowHeight;
		y = RENDER_HALF_HEIGHT - (ArrowHeight * 2) - (RENDER_ABSOLUTE_MARGIN * 2) - 2;
		Driver.fillTriangle(x, y + ArrowHeight, x + (ArrowWidth / 2), y, x + ArrowWidth, y + ArrowHeight, ButtonColor);

		if (Down)
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		y += RENDER_ABSOLUTE_MARGIN + ArrowHeight;
		Driver.fillTriangle(x, y, x + (ArrowWidth / 2), y + ArrowHeight, x + ArrowWidth, y, ButtonColor);

		if (Left)
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		x = RENDER_ABSOLUTE_MARGIN;
		y = RENDER_HALF_HEIGHT - (ArrowHeight * 2) - (RENDER_ABSOLUTE_MARGIN * 2) - 1;
		Driver.fillTriangle(x, y + (ArrowWidth / 2), x + ArrowHeight, y, x + ArrowHeight, y + ArrowWidth, ButtonColor);

		if (Right)
		{
			ButtonColor = ColorWhite;
		}
		else
		{
			ButtonColor = ColorGrey;
		}
		x += (RENDER_ABSOLUTE_MARGIN * 2) + ArrowHeight + ArrowWidth;
		Driver.fillTriangle(x, y, x + ArrowHeight, y + (ArrowWidth / 2), x, y + ArrowWidth, ButtonColor);

	}


	bool Callback()
	{

#ifdef DEBUG_MOCK_CONTROLLER
		DebugCounter++;

		if (millis() - LastRandom > BUTTON_RANDOM_PERIOD)
		{
			LastRandom = millis();

			RandomButton(GamecubeButtons::A);
			RandomButton(GamecubeButtons::B);
			RandomButton(GamecubeButtons::X);
			RandomButton(GamecubeButtons::Y);
			RandomButton(GamecubeButtons::Z);

			RandomButton(GamecubeButtons::Left);
			RandomButton(GamecubeButtons::Up);
			RandomButton(GamecubeButtons::Right);
			RandomButton(GamecubeButtons::Down);

			SetButton(GamecubeButtons::L, RawData.shoulderLeft > Calibration::TriggerLMainRange);
			SetButton(GamecubeButtons::R, RawData.shoulderRight > Calibration::TriggerRMainRange);
		}

		if (DebugCounter > (2 * (SCREEN_DEBUG_PERIOD / SCREEN_TARGET_PERIOD)))
		{
			RawData.joystickX++;
			RawData.joystickY++;
			RawData.cX++;
			RawData.cY += 3;
			RawData.shoulderLeft++;
			RawData.shoulderRight += 4;
			DebugCounter = 0;
		}

		if (RawData.joystickX > RawRange)
		{
			RawData.joystickX = 0;
		}
		if (RawData.joystickY > RawRange)
		{
			RawData.joystickY = 0;
		}

		if (RawData.cX > RawRange)
		{
			RawData.cX = 0;
		}
		if (RawData.cY > RawRange)
		{
			RawData.cY = 0;
		}

		if (RawData.shoulderLeft > RawRange)
		{
			RawData.shoulderLeft = 0;
		}
		if (RawData.shoulderRight > RawRange)
		{
			RawData.shoulderRight = 0;
		}
#else
		RawData = *Controller->GetRawData();
#endif
		RenderStarted = millis();
		//noInterrupts();
		Render();
		//interrupts();
		return true;
	}

	void RenderRumble(const bool on)
	{
		if (on)
		{
			Driver.setBackground(ColorGreyDark);
		}
		else
		{
			Driver.setBackground(ColorBlack);
		}
	}

	uint8_t Transparent = 0;


	void Render()
	{
		ClearCounter++;
		if (ClearCounter > SCREEN_SKIP_CLEAR_MAX)
		{
			Driver.clearScreen();
			ClearCounter = 0;
		}

		Transparent++;
		if (Transparent > SCREEN_SKIP_ALPHA)
		{
			Transparent = 0;
		}

		RenderRumble(Controller->GetRumbleDACOutput());
		RenderTargetBackground(RawData);

		RenderTriggers(RawData, false, Controller->GetTriggerL(), Controller->GetTriggerR(), Controller->GetButton5(), Controller->GetButton6());

		RenderButtons();

		RenderFilteredCJoy(Controller->GetJoy2X(), Controller->GetJoy2Y());
		RenderFilteredJoy(Controller->GetJoy1X(), Controller->GetJoy1Y());

		RenderRawInputCJoy(RawData, false);
		RenderRawInputJoy(RawData, true);

	}

#ifdef DEBUG_MOCK_CONTROLLER
	uint8_t DebugCounter = 0;
	const int16_t SCREEN_DEBUG_PERIOD = 1;
	const int16_t BUTTON_RANDOM_PERIOD = 100;

	uint32_t LastRandom = 0;


	void SetButton(const uint8_t bit, const bool on)
	{
		if (on)
			RawData.buttons |= (1 << bit);
		else
			RawData.buttons &= ~(1 << bit);
	}
	void RandomButton(const uint8_t bit)
	{
		SetButton(bit, random(INT32_MAX) < ((INT32_MAX * 1) / 9));
	}

#endif
};
#endif