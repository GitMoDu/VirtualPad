#ifndef _VIRTUAL_PAD_MOTION_PAD_h
#define _VIRTUAL_PAD_MOTION_PAD_h

#include "AnalogPad.h"

namespace VirtualPad
{
	/// <summary>
	/// Expands analog_pad_state_t and adds Accelerometer and Gyro features.
	/// </summary>
	struct motion_pad_state_t : analog_pad_state_t
	{
		int16_t accelerationX = 0;
		int16_t accelerationY = 0;
		int16_t accelerationZ = 0;

		int16_t rotationRateX = 0;
		int16_t rotationRateY = 0;
		int16_t rotationRateZ = 0;

	public:// Read API.
		int16_t AccelerationX() const
		{
			return accelerationX;
		}

		int16_t AccelerationY() const
		{
			return accelerationY;
		}

		int16_t AccelerationZ() const
		{
			return accelerationZ;
		}

		int16_t RotationRateX() const
		{
			return rotationRateX;
		}

		int16_t RotationRateY() const
		{
			return rotationRateY;
		}

		int16_t RotationRateZ() const
		{
			return rotationRateZ;
		}

	public:// Write API.
		void SetAcceleration(const int16_t x, const int16_t y, const int16_t z)
		{
			accelerationX = x;
			accelerationY = y;
			accelerationZ = z;
		}

		void SetRotationRate(const int16_t x, const int16_t y, const int16_t z)
		{
			rotationRateX = x;
			rotationRateY = y;
			rotationRateZ = z;
		}

		void SetAccelerationX(const int16_t value)
		{
			accelerationX = value;
		}

		void SetAccelerationY(const int16_t value)
		{
			accelerationY = value;
		}

		void SetAccelerationZ(const int16_t value)
		{
			accelerationZ = value;
		}

		void SetRotationRateX(const int16_t value)
		{
			rotationRateX = value;
		}

		void SetRotationRateY(const int16_t value)
		{
			rotationRateY = value;
		}

		void SetRotationRateZ(const int16_t value)
		{
			rotationRateZ = value;
		}
	};
}
#endif