// ShaderPipeline.cpp
#include "ShaderPipeline.h"
#include <SDL3/SDL.h>
#include <fstream>
#include <vector>
#include <iostream>

// -------------------------------------------------------------------
// Helper: read a .spv binary file into a byte buffer
// -------------------------------------------------------------------
static std::vector<Uint8> loadSPV(const char* path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[ShaderPipeline] Could not open: %s", path);
        return {};
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<Uint8> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[ShaderPipeline] Could not read: %s", path);
        return {};
    }

    SDL_Log("[ShaderPipeline] Loaded %s (%zu bytes)", path, buffer.size());
    return buffer;
}

// -------------------------------------------------------------------
// Helper: create one shader stage from a byte buffer
// -------------------------------------------------------------------
static SDL_GPUShader* createShader(
    SDL_GPUDevice* device,
    const std::vector<Uint8>& code,
    SDL_GPUShaderStage        stage,
    Uint32                    numSamplers,
    Uint32                    numUniformBuffers)
{
    if (code.empty()) return nullptr;

    SDL_GPUShaderCreateInfo info = {};
    info.code = code.data();
    info.code_size = code.size();
    info.entrypoint = "main";
    info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    info.stage = stage;
    info.num_samplers = numSamplers;
    info.num_uniform_buffers = numUniformBuffers;

    SDL_GPUShader* shader = SDL_CreateGPUShader(device, &info);
    if (!shader)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[ShaderPipeline] SDL_CreateGPUShader failed: %s", SDL_GetError());
    }
    return shader;
}

// -------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------
ShaderPipeline::ShaderPipeline(
    SDL_GPUDevice* device,
    const char* vertSpvPath,
    const char* fragSpvPath)
    : pDevice(device), pPipeline(nullptr)
{
    SDL_Log("[ShaderPipeline] Creating pipeline from %s + %s",
        vertSpvPath, fragSpvPath);

    // 1. load spv bytecode
    std::vector<Uint8> vertCode = loadSPV(vertSpvPath);
    std::vector<Uint8> fragCode = loadSPV(fragSpvPath);

    if (vertCode.empty() || fragCode.empty())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[ShaderPipeline] Failed to load shader files, aborting.");
        return;
    }

    // 2. create shader stages
    // vert: no samplers, 1 uniform buffer (push constants)
    // frag: 2 samplers (diffuse + normal map), 1 uniform buffer
    SDL_GPUShader* pVertShader = createShader(device, vertCode,
        SDL_GPU_SHADERSTAGE_VERTEX, 0, 1);
    SDL_GPUShader* pFragShader = createShader(device, fragCode,
        SDL_GPU_SHADERSTAGE_FRAGMENT, 2, 1);

    if (!pVertShader || !pFragShader)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[ShaderPipeline] Shader stage creation failed, aborting.");
        if (pVertShader) SDL_ReleaseGPUShader(device, pVertShader);
        if (pFragShader) SDL_ReleaseGPUShader(device, pFragShader);
        return;
    }

    // 3. vertex layout: each vertex = { float x, y, u, v }
    SDL_GPUVertexBufferDescription vertexBinding = {};
    vertexBinding.slot = 0;
    vertexBinding.pitch = sizeof(float) * 4;
    vertexBinding.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    vertexBinding.instance_step_rate = 0;

    // position (location = 0)
    SDL_GPUVertexAttribute vertexAttribs[2] = {};
    vertexAttribs[0].location = 0;
    vertexAttribs[0].buffer_slot = 0;
    vertexAttribs[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
    vertexAttribs[0].offset = 0;
    // uv (location = 1)
    vertexAttribs[1].location = 1;
    vertexAttribs[1].buffer_slot = 0;
    vertexAttribs[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
    vertexAttribs[1].offset = sizeof(float) * 2;

    // 4. alpha blending
    SDL_GPUColorTargetBlendState blend = {};
    blend.enable_blend = true;
    blend.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    blend.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    blend.color_blend_op = SDL_GPU_BLENDOP_ADD;
    blend.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    blend.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    blend.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
    blend.color_write_mask = SDL_GPU_COLORCOMPONENT_R |
        SDL_GPU_COLORCOMPONENT_G |
        SDL_GPU_COLORCOMPONENT_B |
        SDL_GPU_COLORCOMPONENT_A;

    SDL_GPUColorTargetDescription colorTarget = {};
    colorTarget.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    colorTarget.blend_state = blend;

    // 5. assemble pipeline
    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.vertex_shader = pVertShader;
    pipelineInfo.fragment_shader = pFragShader;

    pipelineInfo.vertex_input_state.vertex_buffer_descriptions = &vertexBinding;
    pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
    pipelineInfo.vertex_input_state.vertex_attributes = vertexAttribs;
    pipelineInfo.vertex_input_state.num_vertex_attributes = 2;

    pipelineInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    pipelineInfo.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
    pipelineInfo.rasterizer_state.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;

    pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

    pipelineInfo.target_info.color_target_descriptions = &colorTarget;
    pipelineInfo.target_info.num_color_targets = 1;
    pipelineInfo.target_info.has_depth_stencil_target = false;

    // 6. create
    pPipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
    if (!pPipeline)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[ShaderPipeline] SDL_CreateGPUGraphicsPipeline failed: %s", SDL_GetError());
    }
    else
    {
        SDL_Log("[ShaderPipeline] Pipeline created successfully.");
    }

    // 7. pipeline holds its own reference, release shader handles
    SDL_ReleaseGPUShader(device, pVertShader);
    SDL_ReleaseGPUShader(device, pFragShader);
}

// -------------------------------------------------------------------
// Destructor
// -------------------------------------------------------------------
ShaderPipeline::~ShaderPipeline()
{
    if (pPipeline && pDevice)
    {
        SDL_ReleaseGPUGraphicsPipeline(pDevice, pPipeline);
        pPipeline = nullptr;
    }
}

// -------------------------------------------------------------------
// bind: activate this pipeline for the current render pass
// -------------------------------------------------------------------
void ShaderPipeline::bind(SDL_GPURenderPass* pass)
{
    if (!pPipeline)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[ShaderPipeline] bind() called with null pipeline.");
        return;
    }
    SDL_BindGPUGraphicsPipeline(pass, pPipeline);
}
// -------------------------------------------------------------------
// pushUniforms: upload per-draw data to both shader stages
// -------------------------------------------------------------------
void ShaderPipeline::pushUniforms(SDL_GPUCommandBuffer* cmd, const ShaderUniforms& u)
{
    SDL_PushGPUVertexUniformData(cmd, 0, &u, sizeof(ShaderUniforms));
    SDL_PushGPUFragmentUniformData(cmd, 0, &u, sizeof(ShaderUniforms));
}




// -------------------------------------------------------------------
// isValid
// -------------------------------------------------------------------
bool ShaderPipeline::isValid() const
{
    return pPipeline != nullptr;
}