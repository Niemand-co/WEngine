#pragma once

class MeshFilter;

struct ImageData
{
	~ImageData()
	{
		free(pData);
	}

	unsigned char *pData;
	int width;
	int height;
	int nrChannel;
};

class FileLoader
{
public:

	//static MeshFilter* ModelLoad( const char* filePath );

	static ImageData* ImageLoad( const char* filePath );

};