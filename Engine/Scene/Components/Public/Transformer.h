#pragma once
#ifndef SR_TRANSFORMER_H
#define SR_TRANSFORMER_H

#include "Scene/Components/Public/Component.h"

template<typename T>
struct Vector3Type;
typedef Vector3Type<float> Vector3;
template<typename T, int RowSize, int ColSize>
struct Matrix;
typedef Matrix<float, 4, 4> Matrix4x4f;

class Transformer : public Component
{
public:
	Transformer();
	~Transformer() = default;

	void SetScale(Vector3 scale);
	void SetRotate(Vector3 rotate);
	void SetTranslate(Vector3 translate);

	Matrix4x4f GetModelMatrix();

private:
	Vector3 m_scale;
	Vector3 m_rotate;
	Vector3 m_translate;
	Matrix4x4f ScaleMatrix;
	Matrix4x4f RotateMatrix;
	Matrix4x4f TranslateMatrix;
	Matrix4x4f Model;
};

#endif