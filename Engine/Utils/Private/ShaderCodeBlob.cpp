#include "pch.h"
#include "Utils/Public/ShaderCodeBlob.h"

ShaderCodeBlob::ShaderCodeBlob(std::string path)
{
	ReadShaderFromPath(path);
}

ShaderCodeBlob::~ShaderCodeBlob()
{
}

unsigned int ShaderCodeBlob::GetSize()
{
	return m_buffer.size();
}

unsigned int* ShaderCodeBlob::GetCode()
{
	return reinterpret_cast<unsigned int*>(m_buffer.data());
}

void ShaderCodeBlob::ReadShaderFromPath(std::string path)
{
	std::ifstream file(path, std::ios::ate | std::ios::binary);
	RE_ASSERT(file.is_open(), "Failed to Open Shader File.");

	size_t fileSize = (size_t)file.tellg();
	m_buffer.resize(fileSize);

	file.seekg(0);
	file.read(m_buffer.data(), fileSize);

	file.close();
}
