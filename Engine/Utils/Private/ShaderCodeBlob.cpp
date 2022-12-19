#include "pch.h"
#include "Utils/Public/ShaderCodeBlob.h"

ShaderCodeBlob::ShaderCodeBlob(const WEngine::WString& path)
{
	ReadShaderFromPath(path);
}

ShaderCodeBlob::~ShaderCodeBlob()
{
}

unsigned int ShaderCodeBlob::GetSize()
{
	return m_buffer.Size();
}

unsigned int* ShaderCodeBlob::GetCode()
{
	return reinterpret_cast<unsigned int*>(m_buffer.GetData());
}

void ShaderCodeBlob::ReadShaderFromPath(const WEngine::WString& path)
{
	std::ifstream file(path.Data(), std::ios::ate | std::ios::binary);
	RE_ASSERT(file.is_open(), "Failed to Open Shader File.");

	size_t fileSize = (size_t)file.tellg();
	m_buffer.Resize(fileSize);

	file.seekg(0);
	file.read(m_buffer.GetData(), fileSize);

	file.close();
}
