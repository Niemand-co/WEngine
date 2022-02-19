#include "Transformer.h"

Transformer::Transformer()
	: m_scale(Matrix4x4f::GetIdentityMatrix()), m_rotate(Matrix4x4f::GetIdentityMatrix()),
	  m_translate(Matrix4x4f::GetIdentityMatrix()), Model(Matrix4x4f::GetIdentityMatrix())
{
}

void Transformer::SetScale(Matrix4x4f scale)
{
	m_scale = scale;
	Model = m_translate * m_rotate * m_scale;
}

void Transformer::SetRotate(Matrix4x4f rotate)
{
	m_rotate = rotate;
	Model = m_translate * m_rotate * m_scale;
}

void Transformer::SetTranslate(Matrix4x4f translate)
{
	m_translate = translate;
	Model = m_translate * m_rotate * m_scale;
}

Matrix4x4f Transformer::GetModelMatrix()
{
	return Model;
}

