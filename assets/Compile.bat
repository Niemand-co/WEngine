dxc -spirv -T vs_6_0 -E vert OpaqueShader.hlsl -Fo OpaqueVert.spv
dxc -spirv -T ps_6_0 -E frag OpaqueShader.hlsl -Fo OpaqueFrag.spv
dxc -spirv -T vs_6_0 -E vert SkyboxShader.hlsl -Fo SkyboxShaderVert.spv
dxc -spirv -T ps_6_0 -E frag SkyboxShader.hlsl -Fo SkyboxShaderFrag.spv
dxc -spirv -T vs_6_0 -E vert OutlineShader.hlsl -Fo OutlineVert.spv
dxc -spirv -T ps_6_0 -E frag OutlineShader.hlsl -Fo OutlineFrag.spv
dxc -spirv -T vs_6_0 -E vert DepthOnlyShader.hlsl -Fo DepthOnlyVert.spv
dxc -spirv -T ps_6_0 -E frag DepthOnlyShader.hlsl -Fo DepthOnlyFrag.spv
PAUSE