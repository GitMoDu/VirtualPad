// AxisTemplates.h

#ifndef _AXIS_TEMPLATES_h
#define _AXIS_TEMPLATES_h


#include <stdint.h>

//
//template<typename OutT,
//	const uint8_t MinX,
//	const uint8_t MaxX,
//	const uint8_t MinY,
//	const uint8_t MaxY,
//	const int8_t OffsetX,
//	const int8_t OffsetY,
//	const uint8_t DeadZoneRadius,
//	const OutT OutBottom,
//	const OutT OutTop>
//	class AxisElipse8Bit
//{
//private:
//	const uint8_t MidX = (MaxX + MinX) / 2 + OffsetX;
//	const uint8_t MidY = (MaxY + MinY) / 2 + OffsetY;
//	const uint16_t DeadzoneRadiusSquared = DeadZoneRadius * DeadZoneRadius;
//
//	const OutT OutCenter = (OutTop + OutBottom) / 2;
//
//private:
//
//	inline bool DeadZone(const uint8_t x, const uint8_t y, uint8_t xStart, uint8_t yStart)
//	{
//		const int8_t dX = x - MidX;
//		const int8_t dY = y - MidY;
//		const uint16_t radius = (dX * dX) + (dY * dY);
//
//		//Is in deadzone?Todo.
//		if (radius <= DeadzoneRadiusSquared)
//		{
//			xStart = MidX;
//			yStart = MidY;
//		}
//		else
//		{
//			xStart = MidX;
//			yStart = MidY;
//		}
//	}
//
//
//public:
//	void Parse(const uint8_t x, const uint8_t y, OutT& outX, OutT& outY)
//	{
//		const uint8_t ValueClippedX = constrain(x, MinX, MaxX);
//		const uint8_t ValueClippedY = constrain(y, MinY, MaxY);
//
//		uint8_t StartX;
//		uint8_t StartY;
//		if (DeadZone(x, y, StartX, StartY))
//		{
//			outX = OutCenter;
//			outY = OutCenter;
//		}
//		else
//		{
//			if (ValueClippedX >= MidX)
//			{
//				outX = map(ValueClippedX, StartX, MaxX, OutCenter, OutTop);
//			}
//			else
//			{
//				outX = map(ValueClippedX, MinX, StartX, OutCenter, OutTop);
//			}
//
//			if (ValueClippedY >= MidY)
//			{
//				outY = map(ValueClippedY, StartY, MaxX, OutCenter, OutTop);
//			}
//			else
//			{
//				outY = map(ValueClippedY, MinY, StartY, OutCenter, OutTop);
//			}
//		}
//	}
//};
#endif