#pragma once
#ifndef SR_SHADER_H
#define SR_SHADER_H
#include "Math/Matrix.h"
#include "Render/Vertex.h"
#include <vector>
#include <unordered_map>
#include <string>

class Shader
{
public:
	virtual ~Shader() = 0;
};

class ShaderLibrary
{
public:
	static Shader* Allocate();
private:
	static std::vector<Shader*> m_shaders;
};

#endif