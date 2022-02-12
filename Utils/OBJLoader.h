#pragma once
#ifndef SR_OBJLOADER_H
#define SR_OBJLOADER_H

#include "Components/Mesh.h"

class OBJLoader
{
public:
	static Mesh Load(char* filePath);
};

#endif