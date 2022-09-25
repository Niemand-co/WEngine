#pragma once

#define VULKAN_API
#ifdef VULKAN_API

#define DEVICE_TYPE_OTHER             0x0001
#define DEVICE_TYPE_INTEGRATED_GPU    0x0002
#define DEVICE_TYPE_DISCRETE_GPU      0x0004
#define DEVICE_TYPE_VIRTUAL_GPU       0x0008
#define DEVICE_TYPE_CPU               0x0010

#define QUEUE_PROPERTY_GRAPHICS       0x00000001
#define QUEUE_PROPERTY_COMPUTE        0x00000002
#define QUEUE_PROPERTY_BLIT           0x00000004
#define QUEUE_PROPERTY_PROTECTED      0x00000010
#define QUEUE_PROPERTY_PRESENT        0x00000020

#define    robustBufferAccess                             0x0000000000000000
#define    fullDrawIndexUint32							  0x0000000000000000
#define    imageCubeArray								  0x0000000000000000
#define    independentBlend								  0x00000000
#define    FEATURE_GEOMETRY_SHADER                        0x0001
#define    FEATURE_TESSELATION_SHADER                     0x0002
#define    sampleRateShading							  0x0000000000000000
#define    dualSrcBlend									  0x0000000000000000
#define    logicOp										  0x0000000000000000
#define    multiDrawIndirect							  0x0000000000000000
#define    drawIndirectFirstInstance					  0x0000000000000000
#define    depthClamp									  0x0000000000000000
#define    depthBiasClamp								  0x0000000000000000
#define    fillModeNonSolid								  0x0000000000000000
#define    depthBounds									  0x0000000000000000
#define    wideLines									  0x0000000000000000
#define    largePoints									  0x0000000000000000
#define    alphaToOne									  0x0000000000000000
#define    multiViewport								  0x0000000000000000
#define    samplerAnisotropy							  0x0000000000000000
#define    textureCompressionETC2						  0x0000000000000000
#define    textureCompressionASTC_LDR					  0x0000000000000000
#define    textureCompressionBC							  0x0000000000000000
#define    occlusionQueryPrecise						  0x0000000000000000
#define    pipelineStatisticsQuery						  0x0000000000000000
#define    vertexPipelineStoresAndAtomics				  0x0000000000000000
#define    fragmentStoresAndAtomics						  0x0000000000000000
#define    shaderTessellationAndGeometryPointSize		  0x0000000000000000
#define    shaderImageGatherExtended					  0x0000000000000000
#define    shaderStorageImageExtendedFormats			  0x0000000000000000
#define    shaderStorageImageMultisample				  0x0000000000000000
#define    shaderStorageImageReadWithoutFormat			  0x0000000000000000
#define    shaderStorageImageWriteWithoutFormat			  0x0000000000000000
#define    shaderUniformBufferArrayDynamicIndexing		  0x0000000000000000
#define    shaderSampledImageArrayDynamicIndexing		  0x0000000000000000
#define    shaderStorageBufferArrayDynamicIndexing		  0x0000000000000000
#define    shaderStorageImageArrayDynamicIndexing		  0x0000000000000000
#define    shaderClipDistance							  0x0000000000000000
#define    shaderCullDistance							  0x0000000000000000
#define    shaderFloat64								  0x0000000000000000
#define    shaderInt64									  0x0000000000000000
#define    shaderInt16									  0x0000000000000000
#define    shaderResourceResidency						  0x0000000000000000
#define    shaderResourceMinLod							  0x0000000000000000
#define    sparseBinding								  0x0000000000000000
#define    sparseResidencyBuffer						  0x0000000000000000
#define    sparseResidencyImage2D						  0x0000000000000000
#define    sparseResidencyImage3D						  0x0000000000000000
#define    sparseResidency2Samples						  0x0000000000000000
#define    sparseResidency4Samples						  0x0000000000000000
#define    sparseResidency8Samples						  0x0000000000000000
#define    sparseResidency16Samples						  0x0000000000000000
#define    sparseResidencyAliased						  0x0000000000000000
#define    variableMultisampleRate						  0x0000000000000000
#define    inheritedQueries								  0x0000000000000000

#endif