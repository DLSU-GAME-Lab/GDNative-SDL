// assets/shaders/sprite.vert
#version 450

// --- per-vertex input (matches your x,y,u,v layout) ---
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;

// --- output to fragment shader ---
layout(location = 0) out vec2 fragUV;

// --- uniform buffer: transform data pushed per draw call ---
layout(set = 1, binding = 0) uniform VertUniforms {
    vec2 position;   // sprite position in normalized device coords (-1 to 1)
    vec2 size;       // sprite size in NDC
} uVert;

void main()
{
    // scale and position the quad
    vec2 worldPos = inPos * uVert.size + uVert.position;
    gl_Position   = vec4(worldPos, 0.0, 1.0);
    fragUV        = inUV;
}