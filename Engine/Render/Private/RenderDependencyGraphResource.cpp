#include "pch.h"
#include "Render/Public/RenderDependencyGraphResource.h"

bool WRDGResourceState::IsMergeAllowed(const WRDGResourceState& Previous, const WRDGResourceState& Next)
{
    if (Previous.Access != Next.Access)
    {
        if (WEngine::EnumHasFlags(Previous.Access, EAccess::ReadOnly) && WEngine::EnumHasFlags(Next.Access, EAccess::Writable))
        {
            return false;
        }

        if (WEngine::EnumHasFlags(Previous.Access, EAccess::WriteOnly) && WEngine::EnumHasFlags(Next.Access, EAccess::Readable))
        {
            return false;
        }
    }

    if (WEngine::EnumHasFlags(Previous.Access, EAccess::UAV) || WEngine::EnumHasFlags(Next.Access, EAccess::UAV))
    {
        return false;
    }

    if (Previous.Pipeline != Next.Pipeline)
    {
        return false;
    }

    return true;
}
