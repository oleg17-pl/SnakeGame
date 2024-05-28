#pragma once

#include <cmath>

template <typename T>
struct vec2
{
	T X, Y;

	vec2(T X, T Y) : X{ X }, Y{ Y } {}
	vec2(T value) : X{ value }, Y{ value } {}
	vec2() : X{}, Y{} {}

	T operator[](int index)
	{
		switch (index)
		{
		case 0:  return X;
		case 1:  return Y;
		default: return 0;
		}
	}

	float GetLen() { return sqrt((X * X) + (Y * Y)); }
	vec2 Normalized()
	{
		float len = this->getLen();
		return vec2{ X / len, Y / len };
	}

	bool operator==(vec2 other) { return X == other.X && Y == other.Y; }
	bool operator!=(vec2 other) { return !(*this == other); }

	void operator+=(vec2 other) { X += other.X; Y += other.Y; }
	void operator-=(vec2 other) { X -= other.X; Y -= other.Y; }
	void operator*=(vec2 other) { X *= other.X; Y *= other.Y; }

	vec2 operator+(vec2 other) { return vec2{ this->X + other.X, this->Y + other.Y }; }
	vec2 operator-(vec2 other) { return vec2{ this->X - other.X, this->Y - other.Y }; }
	vec2 operator*(vec2 other) { return vec2{ this->X * other.X, this->Y * other.Y }; }
};