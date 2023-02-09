dxc -spirv -T vs_6_0 -E VSMain OpaqueShader.hlsl -Fo OpaqueVert.vs
dxc -spirv -T ps_6_0 -E PSMain OpaqueShader.hlsl -Fo OpaqueFrag.ps
dxc -spirv -T vs_6_0 -E VSMain SkyboxShader.hlsl -Fo SkyboxShaderVert.vs
dxc -spirv -T ps_6_0 -E PSMain SkyboxShader.hlsl -Fo SkyboxShaderFrag.ps
dxc -spirv -T vs_6_0 -E VSMain OutlineShader.hlsl -Fo OutlineVert.vs
dxc -spirv -T ps_6_0 -E PSMain OutlineShader.hlsl -Fo OutlineFrag.ps
dxc -spirv -T vs_6_0 -E VSMain DepthOnlyShader.hlsl -Fo DepthOnlyVert.vs
dxc -spirv -T ps_6_0 -E PSMain DepthOnlyShader.hlsl -Fo DepthOnlyFrag.ps
dxc -spirv -T vs_6_0 -E VSMain DebugDraw.hlsl -Fo DebugDrawVert.vs
dxc -spirv -T ps_6_0 -E PSMain DebugDraw.hlsl -Fo DebugDrawFrag.ps
PAUSE