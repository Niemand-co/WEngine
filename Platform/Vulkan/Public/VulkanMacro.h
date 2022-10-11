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

#define    FEATURE_GEOMETRY_SHADER                        0x0001
#define    FEATURE_TESSELATION_SHADER                     0x0002

#define MEMORY_PROPERTY_DEVICE_LOCAL        0x0001
#define MEMORY_PROPERTY_HOST_VISIBLE        0x0002
#define MEMORY_PROPERTY_HOST_COHERENT       0x0004
#define MEMORY_PROPERTY_HOST_CACHED         0x0008
#define MEMORY_PROPERTY_LAZILY_ALLOCATED    0x0010
#define MEMORY_PROPERTY_PROTECTED           0x0020
#define MEMORY_PROPERTY_DEVICE_COHERENT     0x0040
#define MEMORY_PROPERTY_DEVICE_UNCACHED     0x0080
#define MEMORY_PROPERTY_RDMA_CAPABLE        0x0100

#define BUFFER_USAGE_TRANSFER_SRC             0x00000001
#define BUFFER_USAGE_TRANSFER_DST             0x00000002
#define BUFFER_USAGE_UNIFORM_TEXEL_BUFFER     0x00000004
#define BUFFER_USAGE_STORAGE_TEXEL_BUFFER     0x00000008
#define BUFFER_USAGE_UNIFORM_BUFFER           0x00000010
#define BUFFER_USAGE_STORAGE_BUFFER           0x00000020
#define BUFFER_USAGE_INDEX_BUFFER             0x00000040
#define BUFFER_USAGE_VERTEX_BUFFER            0x00000080
#define BUFFER_USAGE_INDIRECT_BUFFER          0x00000100
#define BUFFER_USAGE_SHADER_DEVICE_ADDRESS    0x00020000

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

#endif