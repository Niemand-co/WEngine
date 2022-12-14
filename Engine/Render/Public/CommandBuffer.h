#pragma once

class RenderTarget;

class CommandBuffer
{
public:

	CommandBuffer();

	~CommandBuffer() = default;

	void SetRenderTarget(const RenderTarget& renderTarget);

	void ClearRenderTarget(bool bClearColor, bool bClearDepthStencil, const Vector4& clearColor, const Vector2& clearDepthStencil = { 1, 1 });

private:

	

};