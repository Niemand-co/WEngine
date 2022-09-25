#pragma once
#ifndef SR_OBJLOADER_H
#define SR_OBJLOADER_H

class MeshFilter;

class OBJLoader
{
public:
	static MeshFilter* ModelLoad( const char* filePath );
	static unsigned char* ImageLoad( const char* filePath, int* width, int* height, int* comp, int nrChannel );
};

#endif