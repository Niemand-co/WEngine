#pragma once

class ShaderCodeBlob
{
public:

	ShaderCodeBlob(std::string path);

	~ShaderCodeBlob();

public:

	unsigned int GetSize();

	unsigned int* GetCode();

private:

	void ReadShaderFromPath(std::string path);
	
private:

	std::vector<char> m_buffer;

};