#pragma once
#ifndef SR_VECTOR_H
#define SR_VECTOR_H

#include <stdint.h>
#include <math.h>
#include <iostream>

template<typename T>
struct Vector4Type;
template<typename T>
struct Vector3Type;

template<typename T, size_t SizeOfArray>
constexpr size_t ElementCount(T(&)[SizeOfArray]) { return SizeOfArray; }

template<typename T>
struct Vector2Type {

	union {

		T data[2];
		struct { T x, y; };
		struct { T r, g; };
		struct { T u, v; };

	};

	Vector2Type<T>() : x(0), y(0) {}
	Vector2Type<T>(const T& _v) : x(_v), y(_v) {}
	Vector2Type<T>(const T& _x, const T& _y) : x(_x), y(_y) {}

	int Size() { return (int)2; }

	operator T* () { return data; }
	operator const T* () const { return static_cast<const T*>(data); }

	Vector2Type<T>& operator=(const Vector2Type<T>& rval) {
		x = rval.x;
		y = rval.y;
		return *this;
	}

	Vector2Type<T> operator+(const Vector2Type<T>& v) {

		Vector2Type result;
		result[0] = data[0] + v[0];
		result[1] = data[1] + v[1];
		return result;

	}

	Vector2Type<T> operator*(const T& a) {

		Vector2Type<T> result;
		result[0] = data[0] * a;
		result[1] = data[1] * a;
		return result;

	}

	Vector2Type<T> operator/(const T& a) {

		Vector2Type<T> result;
		T div = (T)1 / a;
		result[0] = data[0] * div;
		result[1] = data[1] * div;
		return result;

	}

	void operator+=(const Vector2Type<T>& v) {

		data[0] += v[0];
		data[1] += v[1];

	}

	void operator-=(const Vector2Type<T>& v) {

		data[0] -= v[0];
		data[1] -= v[1];

	}

	void operator*=(const T& a) {

		data[0] *= a;
		data[1] *= a;

	}

	void operator/=(const T& a) {

		T div = (T)1 / a;
		data[0] *= div;
		data[1] *= div;

	}

	T distance() { return static_cast<T>(sqrt(((double)data[0] * data[0] + data[1] * data[1]))); }

};

template<typename T>
struct Vector3Type {

	union {

		T data[3];
		struct { T x, y, z; };
		struct { T r, g, b; };

	};

	Vector3Type<T>() : x(0), y(0), z(0) {}
	Vector3Type<T>(const T& _v) : x(_v), y(_v), z(_v) {}
	Vector3Type<T>(const Vector3Type<T>& v3) : x(v3.x), y(v3.y), z(v3.z) {}
	Vector3Type<T>(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {}
	Vector3Type<T>(const Vector4Type<T>& v4) : x(v4.x), y(v4.y), z(v4.z) {}

	int Size() { return (int)3; }

	operator T* () { return data; }
	operator const T* () const { return static_cast<const T*>(data); }

	T distance() { return static_cast<T>(sqrt(((double)data[0] * data[0] + (double)data[1] * data[1] + (double)data[2] * data[2]))); }
	T squareDistance() { return static_cast<T>(((double)data[0] * data[0] + (double)data[1] * data[1] + (double)data[2] * data[2])); }
	Vector3Type<T> Norm()
	{
		Vector3Type<T> result;
		T length = x * x + y * y + z * z;
		length = (T)(1.0 / sqrt(length));
		for (size_t i = 0; i < 3; ++i)
			result[i] = data[i] * length;
		return result;
	}

	inline void Set(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}

	Vector3Type<T>& operator=(const Vector3Type<T>& rval) {
		x = rval.x;
		y = rval.y;
		z = rval.z;
		return *this;
	}

};

template<typename T>
struct Vector4Type {

	union {

		T data[4];
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };

	};

	Vector4Type<T>() : x(0), y(0), z(0), w(0) {}
	Vector4Type<T>(const T& _v) : x(_v), y(_v), z(_v), w(_v) {}
	Vector4Type<T>(const T& _x, const T& _y, const T& _z, const T& _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector4Type<T>(const Vector3Type<T>& v3) : x(v3.x), y(v3.y), z(v3.z), w(1.0f) {}
	Vector4Type<T>(const Vector3Type<T>& v3a, const T& _w) : x(v3a.x), y(v3a.y), z(v3a.z), w(_w) {}

	int Size() { return (int)4; }

	operator T* () { return data; }
	operator const T* () const { return static_cast<const T*>(data); }

	Vector4Type<T>& operator=(const Vector4Type<T>& rval) {
		memcpy(data, rval, sizeof(T) * 4);
		return *this;
	}

	Vector4Type<T>& operator=(const T& f) {
		memcpy(data, f, sizeof(T) * 4);
		return *this;
	}

};

template<template<typename> class TT, typename T>
TT<T> operator-(const TT<T>& vec1, const TT<T>& vec2) {

	TT<T> result;

	size_t cnt = ElementCount(vec1.data);
	for (size_t i = 0; i < cnt; ++i)
		result[i] = vec1[i] - vec2[i];

	return result;

}

template<template<typename> class TT, typename T>
TT<T> operator*(int factor, const TT<T>& vector) {

	TT<T> result;

	size_t cnt = ElementCount(vector.data);
	for (size_t i = 0; i < cnt; ++i)
		result[i] = vector[i] * factor;

	return result;

}

template<template<typename> class TT, typename T>
TT<T> operator*(const TT<T>& vec1, const TT<T>& vec2) {

	TT<T> result;

	size_t cnt = ElementCount(vec1.data);
	for (size_t i = 0; i < cnt; ++i)
		result[i] = vec1[i] * vec2[i];

	return result;

}

template<template<typename> class TT, typename T>
TT<T> operator*(double factor, const TT<T>& vector) {

	TT<T> result;

	size_t cnt = ElementCount(vector.data);
	for (size_t i = 0; i < cnt; ++i)
		result[i] = vector[i] * factor;

	return result;

}

template<template<typename> class TT, typename T>
TT<T> operator*(const TT<T>& vector, int factor) {

	TT<T> result;

	size_t cnt = ElementCount(vector.data);
	for (size_t i = 0; i < cnt; ++i)
		result[i] = vector[i] * factor;

	return result;

}

template<template<typename> class TT, typename T>
TT<T> operator*(const TT<T>& vector, float factor) {

	TT<T> result;

	size_t cnt = ElementCount(vector.data);
	for (size_t i = 0; i < cnt; ++i)
		result[i] = vector[i] * factor;

	return result;

}

template<template<typename> class TT, typename T>
TT<T> operator*(const TT<T>& vector, double factor) {

	TT<T> result;

	size_t cnt = ElementCount(vector.data);
	for (size_t i = 0; i < cnt; ++i)
		result[i] = vector[i] * factor;

	return result;

}

template<template<typename> class TT, typename T>
std::ostream& operator<<(std::ostream& out, TT<T> vector) {

	out << "(";
	size_t cnt = ElementCount(vector.data);
	for (size_t i = 0; i < cnt; ++i)
		out << vector.data[i] << ((i == cnt - 1) ? ')' : ',');

	return out;

}

template<template<typename> class TT, typename T>
void VectorAdd(TT<T>& result, const TT<T>& vec1, const TT<T>& vec2) {

	size_t cnt = ElementCount(result.data);
	for (size_t i = 0; i < cnt; ++i)
		result[i] = vec1[i] + vec2[i];

}

template<template<typename> class TT, typename T>
TT<T> operator+(const TT<T>& vec1, const TT<T>& vec2) {

	TT<T> result;
	size_t cnt = ElementCount(result.data);
	for (size_t i = 0; i < cnt; ++i)
		result[i] = vec1[i] + vec2[i];
	return result;

}

template<template<typename> class TT, typename T>
void operator+=(TT<T>& vec1, const TT<T>& vec2) {

	size_t cnt = ElementCount(vec1.data);
	for (size_t i = 0; i < cnt; ++i)
		vec1[i] += vec2[i];

}

template<template<typename> class TT, typename T>
inline TT<T> CrossProduct(const TT<T>& vec1, const TT<T>& vec2) {

	TT<T> result;

	result[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	result[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	result[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];

	return result;

}

template<template<typename> class TT, typename T>
inline T DotProduct(const TT<T>& vec1, const TT<T>& vec2) {

	size_t cnt = ElementCount(vec1.data);
	T result = 0;
	for (size_t i = 0; i < cnt; ++i)
		result += vec1[i] * vec2[i];

	return result;

}

template<template<typename> class TT, typename T>
inline void Normalize(TT<T>& vector) {

	T length;
	size_t cnt = ElementCount(vector.data);
	length = DotProduct(vector, vector);
	length = (T)(1.0 / sqrt(length));
	for (size_t i = 0; i < cnt; ++i)
		vector[i] *= length;

}

template<template<typename> class TT, typename T>
inline TT<T> Unit(const TT<T>& vector) {

	TT<T> result;

	T length;
	size_t cnt = ElementCount(vector.data);
	length = DotProduct(vector, vector);
	length = (T)(1.0 / sqrt(length));
	for (size_t i = 0; i < cnt; ++i)
		result[i] = vector[i] * length;

	return result;

}

template<template<typename> class TT, typename T>
TT<T> operator-(const TT<T>& vector) {

	TT<T> tmp;
	size_t cnt = ElementCount(vector.data);
	for (size_t i = 0; i < cnt; ++i)
		tmp[i] = -vector[i];
	return tmp;

}

// TODO: == is ambiguous
template<typename T>
bool operator==(const Vector3Type<T>& vec1, const Vector3Type<T>& vec2) {

	for (unsigned int i = 0; i < 3; ++i) {

		if (std::abs(vec1[i] - vec2[i]) > 1e-5)
			return false;

	}

	return true;

}

template<template<typename> class TT, typename T>
TT<T> lerp(const TT<T>& vec1, const TT<T>& vec2, float factor)
{
	return (1.0f - factor) * vec1 + factor * vec2;
}

typedef Vector3Type<float> Vec3;
typedef Vector4Type<float> Vec4;
typedef Vector2Type<float> Vec2;

#endif