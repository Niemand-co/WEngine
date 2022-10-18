#include "pch.h"
#include "Utils/Public/FileLoader.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "RHI/Public/RHIBuffer.h"
#include "RHI/Public/RHIContext.h"
#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"

ImageData* FileLoader::ImageLoad(const char* filePath)
{
	ImageData *data = new ImageData();
	data->pData = stbi_load(filePath, &data->width, &data->height, &data->nrChannel, 0);
	return data;
}

RHIBuffer* FileLoader::ImageLoad(const char* filePath, RHIContext *pContext)
{
	int width, height, nrChannel;
	unsigned char *pData = stbi_load(filePath, &width, &height, &nrChannel, 0);

	size_t imageSize = width * height * nrChannel;

	RE_ASSERT(pData, "Failed to Load Image.");

	RHIBufferDescriptor bufferDescriptor = {};
	{
		bufferDescriptor.pData = pData;
		bufferDescriptor.size = imageSize;
		bufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	RHIBuffer* buffer = pContext->CreateTextureBuffer(&bufferDescriptor);

	return buffer;
}
