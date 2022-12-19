#pragma once
#include "Utils/Container/Public/WArray.h"
#include "Utils/Container/Public/WString.h"

class ShaderCodeBlob
{
public:

	ShaderCodeBlob(const WEngine::WString& path);

	~ShaderCodeBlob();

public:

	unsigned int GetSize();

	unsigned int* GetCode();

private:

	void ReadShaderFromPath(const WEngine::WString& path);
	
private:

	WEngine::WArray<char> m_buffer;

};