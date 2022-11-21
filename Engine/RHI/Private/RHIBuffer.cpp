#include "pch.h"
#include "RHI/Public/RHIBuffer.h"
#include "Render/Descriptor/Public/RHIUpdateResourceDescriptor.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIGPU.h"

RHIBuffer::RHIBuffer()
{
    m_pInfos = nullptr;
    m_dynamicAlignment = -1;
}

void RHIBuffer::SetDataSize(size_t size)
{
    RE_ASSERT(size <= m_dynamicAlignment, "Out of Alignment.");
    m_dataSize = size;
        
}

BufferResourceInfo* RHIBuffer::GetBufferInfo()
{
    if (m_pInfos == nullptr)
    {
        if (!m_isDynamic)
        {
            *m_pInfos = { this, 0, m_dataSize };
        }
        else
        {
            RE_ASSERT(m_dynamicAlignment > 0, "Dynamic Buffer Not Be Set.");
            m_pInfos = (BufferResourceInfo*)WEngine::Allocator::Get()->Allocate(m_size * sizeof(BufferResourceInfo));
            for (unsigned int i = 0; i < m_size; ++i)
            {
                m_pInfos[i] = { this, i * m_dynamicAlignment, m_dataSize };
            }
        }
    }

    return m_pInfos;
}