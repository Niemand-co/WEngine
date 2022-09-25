dxc -spirv -T vs_6_0 -E vert Test.hlsl -Fo vert.spv
dxc -spirv -T ps_6_0 -E frag Test.hlsl -Fo frag.spv
PAUSE