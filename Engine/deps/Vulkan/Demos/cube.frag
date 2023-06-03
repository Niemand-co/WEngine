#version 400
#ifdef GL_ARB_shading_language_420pack
#extension GL_ARB_shading_language_420pack : require
#endif

layout(set = 0, binding = 1) uniform sampler2D tex;

in vec3 frag_pos;
layout(location = 0) out vec4 uFragColor;
in vec4 texcoord;

void main()
{
    vec3 dX = dFdx(frag_pos);
    vec3 dY = dFdy(frag_pos);
    vec3 normal = normalize(cross(dX, dY));
    float light = max(0.0, dot(vec3(0.4239999949932098388671875, 0.5659999847412109375, 0.7070000171661376953125), normal));
    uFragColor = texture(tex, texcoord.xy) * light;
}

