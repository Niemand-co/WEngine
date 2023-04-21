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

#define FEATURE_GEOMETRY_SHADER                        0x0001
#define FEATURE_TESSELATION_SHADER                     0x0002

#define MEMORY_PROPERTY_DEVICE_LOCAL        0x0001
#define MEMORY_PROPERTY_HOST_VISIBLE        0x0002
#define MEMORY_PROPERTY_HOST_COHERENT       0x0004
#define MEMORY_PROPERTY_HOST_CACHED         0x0008
#define MEMORY_PROPERTY_LAZILY_ALLOCATED    0x0010
#define MEMORY_PROPERTY_PROTECTED           0x0020
#define MEMORY_PROPERTY_DEVICE_COHERENT     0x0040
#define MEMORY_PROPERTY_DEVICE_UNCACHED     0x0080
#define MEMORY_PROPERTY_RDMA_CAPABLE        0x0100

enum class EBufferUsageFlags : uint32
{
	BF_None            = 0x0,
	BF_TransferSrc     = 0x00000001,
	BF_TransferDst     = 0x00000002,
	BF_UniformTexel    = 0x00000004,
	BF_StorageTexel    = 0x00000008,
	BF_UniformBuffer   = 0x00000010,
	BF_SingleFrame     = 0x00000020,
	BF_StorageBuffer   = 0x00000040,
	BF_IndexBuffer     = 0x00000080,
	BF_VertexBuffer    = 0x00000100,
	BF_IndirectBuffer  = 0x00000200,
	BF_SRV             = 0x00000400,
	BF_UAV             = 0x00000800,
	BF_Dynamic         = 0x00001000,
	BF_CPUAccessable   = 0x00002000,
};
ENUM_CLASS_FLAGS(EBufferUsageFlags)

enum class EImageUsageFlags : uint32
{
	IM_CopySrc                = 0x00000001,
	IM_CopyDst                = 0x00000002,
	IM_Sampled                = 0x00000004,
	IM_Storage                = 0x00000008,
	IM_ColorAttachment        = 0x00000010,
	IM_DepthStencilAttachment = 0x00000020,
	IM_TransientAttachment    = 0x00000040,
	IM_InputAttachment        = 0x00000080,
};

#define IMAGE_ASPECT_COLOR       0x00000001
#define IMAGE_ASPECT_DEPTH       0x00000002
#define IMAGE_ASPECT_STENCIL     0x00000004
#define IMAGE_ASPECT_METADATA    0x00000008
#define IMAGE_ASPECT_PLANE_0     0x00000010
#define IMAGE_ASPECT_PLANE_1     0x00000020
#define IMAGE_ASPECT_PLANE_2     0x00000040

#define PIPELINE_STAGE_TOP_OF_PIPE                       0x00000001
#define PIPELINE_STAGE_DRAW_INDIRECT                     0x00000002
#define PIPELINE_STAGE_VERTEX_INPUT                      0x00000004
#define PIPELINE_STAGE_VERTEX_SHADER                     0x00000008
#define PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER       0x00000010
#define PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER    0x00000020
#define PIPELINE_STAGE_GEOMETRY_SHADER                   0x00000040
#define PIPELINE_STAGE_FRAGMENT_SHADER                   0x00000080
#define PIPELINE_STAGE_EARLY_FRAGMENT_TESTS              0x00000100
#define PIPELINE_STAGE_LATE_FRAGMENT_TESTS               0x00000200
#define PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT           0x00000400
#define PIPELINE_STAGE_COMPUTE_SHADER                    0x00000800
#define PIPELINE_STAGE_TRANSFER                          0x00001000
#define PIPELINE_STAGE_BOTTOM_OF_PIPE                    0x00002000
#define PIPELINE_STAGE_HOST                              0x00004000
#define PIPELINE_STAGE_ALL_GRAPHICS                      0x00008000
#define PIPELINE_STAGE_ALL_COMMANDS                      0x00010000

#define SHADER_STAGE_VERTEX                     0x00000001
#define SHADER_STAGE_TESSELLATION_CONTROL       0x00000002
#define SHADER_STAGE_TESSELLATION_EVALUATION    0x00000004
#define SHADER_STAGE_GEOMETRY                   0x00000008
#define SHADER_STAGE_FRAGMENT                   0x00000010
#define SHADER_STAGE_COMPUTE                    0x00000020

#define ACCESS_INDIRECT_COMMAND_READ             0x00000001
#define ACCESS_INDEX_READ                        0x00000002
#define ACCESS_VERTEX_ATTRIBUTE_READ             0x00000004
#define ACCESS_UNIFORM_READ                      0x00000008
#define ACCESS_INPUT_ATTACHMENT_READ             0x00000010
#define ACCESS_SHADER_READ                       0x00000020
#define ACCESS_SHADER_WRITE                      0x00000040
#define ACCESS_COLOR_ATTACHMENT_READ             0x00000080
#define ACCESS_COLOR_ATTACHMENT_WRITE            0x00000100
#define ACCESS_DEPTH_STENCIL_ATTACHMENT_READ     0x00000200
#define ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE    0x00000400
#define ACCESS_TRANSFER_READ                     0x00000800
#define ACCESS_TRANSFER_WRITE                    0x00001000
#define ACCESS_HOST_READ                         0x00002000
#define ACCESS_HOST_WRITE                        0x00004000
#define ACCESS_MEMORY_READ                       0x00008000
#define ACCESS_MEMORY_WRITE                      0x00010000

#define IMAGE_ASPECT_COLOR      0x00000001
#define IMAGE_ASPECT_DEPTH      0x00000002
#define IMAGE_ASPECT_STENCIL    0x00000004

template<typename FlagBits>
bool VkEnumHasFlags(VkFlags Flags, FlagBits Bits)
{
	return (Flags & Bits) == Bits;
}

#endif