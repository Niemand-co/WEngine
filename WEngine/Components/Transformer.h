#pragma once
#ifndef SR_TRANSFORMER_H
#define SR_TRANSFORMER_H

#include "Components/Component.h"
#include "Math/Matrix.h"

class Transformer : public Component
{
public:
	Transformer();
	~Transformer() = default;

	void SetScale(Vec3 scale);
	void SetRotate(Vec3 rotate);
	void SetTranslate(Vec3 translate);

	Matrix4x4f GetModelMatrix();
private:
	Vec3 m_scale;
	Vec3 m_rotate;
	Vec3 m_translate;
	Matrix4x4f ScaleMatrix;
	Matrix4x4f RotateMatrix;
	Matrix4x4f TranslateMatrix;
	Matrix4x4f Model;
};

#endif