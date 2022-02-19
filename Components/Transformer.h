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

	void SetScale(Matrix4x4f scale);
	void SetRotate(Matrix4x4f rotate);
	void SetTranslate(Matrix4x4f translate);

	Matrix4x4f GetModelMatrix();
private:
	Matrix4x4f m_scale;
	Matrix4x4f m_rotate;
	Matrix4x4f m_translate;
	Matrix4x4f Model;
};

#endif