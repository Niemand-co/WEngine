#pragma once

class MeshFilter;

class OBJLoader
{
public:
	//static MeshFilter* ModelLoad( const char* filePath );
	static unsigned char* ImageLoad( const char* filePath, int* width, int* height, int* comp, int nrChannel );
};