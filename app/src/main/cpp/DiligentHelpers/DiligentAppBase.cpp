//
// Created by Matthew Good on 29/6/21.
//

#include "DiligentAppBase.h"
#include <EngineFactoryOpenGL.h>
#include <DiligentCore/Graphics/GraphicsTools/interface/CommonlyUsedStates.h>
#include <DiligentCore/Graphics/GraphicsTools/interface/ShaderMacroHelper.hpp>
#include <BasicMath.hpp>

DiligentAppBase::DiligentAppBase ()
{
    m_DeviceType = Diligent::RENDER_DEVICE_TYPE_GL;
}

DiligentAppBase::~DiligentAppBase ()
{
}

void DiligentAppBase::attachToContext (int32_t w, int32_t h)
{
    Diligent::EngineGLCreateInfo EngineCI;
    
    m_pEngineFactory = Diligent::GetEngineFactoryOpenGL();
    m_pEngineFactory.Cast<Diligent::IEngineFactoryOpenGL>(Diligent::IID_EngineFactoryOpenGL)
            ->CreateSwapChainGLAndAttachToActiveGLContext(
                    EngineCI, &m_pDevice, &m_pImmediateContext,
                    m_pSCDesc, &m_pSwapChain, w, h
            );
}

Diligent::ITexture * DiligentAppBase::createColorTexture() {
    auto desc = m_pSwapChain->GetDesc();
    Diligent::TextureDesc RTColorDesc;
    RTColorDesc.Type        = Diligent::RESOURCE_DIM_TEX_2D;
    RTColorDesc.Width       = desc.Width;
    RTColorDesc.Height      = desc.Height;
    RTColorDesc.MipLevels   = 1;
    RTColorDesc.Format      = desc.ColorBufferFormat;
    RTColorDesc.BindFlags   = Diligent::BIND_SHADER_RESOURCE | Diligent::BIND_RENDER_TARGET;
    Diligent::ITexture * pRTColor = nullptr;
    m_pDevice->CreateTexture(RTColorDesc, nullptr, &pRTColor);
    return pRTColor;
}

Diligent::ITexture * DiligentAppBase::createDepthTexture() {
    auto desc = m_pSwapChain->GetDesc();
    Diligent::TextureDesc RTDepthDesc;
    RTDepthDesc.Type        = Diligent::RESOURCE_DIM_TEX_2D;
    RTDepthDesc.Width       = desc.Width;
    RTDepthDesc.Height      = desc.Height;
    RTDepthDesc.MipLevels   = 1;
    RTDepthDesc.Format      = desc.DepthBufferFormat;
    RTDepthDesc.BindFlags   = Diligent::BIND_SHADER_RESOURCE | Diligent::BIND_DEPTH_STENCIL;
    Diligent::ITexture * pRTDepth = nullptr;
    m_pDevice->CreateTexture(RTDepthDesc, nullptr, &pRTDepth);
    return pRTDepth;
}

const char * DiligentAppBase::VS_RT = R"(
struct VSInput
{
    float3 Pos0   : ATTRIB0;
    float3 Pos1   : ATTRIB1;
    float3 Pos2   : ATTRIB2;
    float3 Pos3   : ATTRIB3;
    uint VertexID : SV_VertexID;
};

struct PSInput
{
    float4 Pos    : SV_POSITION;
    float2 UV     : TEX_COORD;
};

// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex
// shader output variable name must match exactly the name of the pixel shader input variable.
// If the variable has structure type (like in this example), the structure declarations must also be indentical.
void main(in  VSInput VSIn,
          out PSInput PSIn)
{
    float4 Pos[4];
    Pos[0] = float4(VSIn.Pos0, 1.0);
    Pos[1] = float4(VSIn.Pos1, 1.0);
    Pos[2] = float4(VSIn.Pos2, 1.0);
    Pos[3] = float4(VSIn.Pos3, 1.0);

    float2 UV[4];
    UV[0] = float2(+0.0, +1.0);
    UV[1] = float2(+0.0, +0.0);
    UV[2] = float2(+1.0, +1.0);
    UV[3] = float2(+1.0, +0.0);

    PSIn.Pos = Pos[VSIn.VertexID];
    PSIn.UV = UV[VSIn.VertexID];
}
)";

const char * DiligentAppBase::PS_RT = R"(
cbuffer Constants
{
    float g_Time;
    float g_Padding0;
    float g_Padding1;
    float g_Padding2;

    float4 g_UVPreTransform;    // Need to store this as float4, because
    float4 g_UVPreTransformInv; // there are layout problems with float2x2.
};

Texture2D    g_Texture;
SamplerState g_Texture_sampler; // By convention, texture samplers must use the '_sampler' suffix

struct PSInput
{
    float4 Pos   : SV_POSITION;
    float2 UV    : TEX_COORD;
};

struct PSOutput
{
    float4 Color : SV_TARGET;
};

// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex
// shader output variable name must match exactly the name of the pixel shader input variable.
// If the variable has structure type (like in this example), the structure declarations must also be indentical.
void main(in  PSInput  PSIn,
          out PSOutput PSOut)
{
#if defined(DESKTOP_GL) || defined(GL_ES)
    // Unlike other graphics API's, OpenGL has its texture coordinates origin at the bottom-left corner instead
    // of the top left. When sampling a regular texture, there is no visible difference since the texture data
    // is also uploaded starting from that lower-left corner. When sampling a framebuffer attachment, there is
    // a visible difference, though. Hence we need to flip the Y coordinate of the frame buffer attachment.
    float2 UV = float2(PSIn.UV.x, 1.0 - PSIn.UV.y);
#else
    float2 UV = PSIn.UV;
#endif

#if TRANSFORM_UV
    // If pre-transform is applied, then current UV coordinates are in pre-transformed space.
    // We need to transform them back to original space.
    float2x2 UVTransformMatrixInv;
    MATRIX_ELEMENT(UVTransformMatrixInv, 0, 0) = g_UVPreTransformInv.x;
    MATRIX_ELEMENT(UVTransformMatrixInv, 1, 0) = g_UVPreTransformInv.y;
    MATRIX_ELEMENT(UVTransformMatrixInv, 0, 1) = g_UVPreTransformInv.z;
    MATRIX_ELEMENT(UVTransformMatrixInv, 1, 1) = g_UVPreTransformInv.w;
    UV = mul(UV - float2(0.5, 0.5), UVTransformMatrixInv) + float2(0.5, 0.5);
#endif

    float2 DistortedUV = UV + float2(sin(UV.y*300.0)*0.1 * sin(g_Time*3.0), sin(UV.x*200.0)*0.02 * sin(g_Time*2.0));

#if TRANSFORM_UV
    // Now we need to transform DistortedUV back to pre-transformed space, because this is how
    // off-screen buffer was rendered
    float2x2 UVTransformMatrix;
    MATRIX_ELEMENT(UVTransformMatrix, 0, 0) = g_UVPreTransform.x;
    MATRIX_ELEMENT(UVTransformMatrix, 1, 0) = g_UVPreTransform.y;
    MATRIX_ELEMENT(UVTransformMatrix, 0, 1) = g_UVPreTransform.z;
    MATRIX_ELEMENT(UVTransformMatrix, 1, 1) = g_UVPreTransform.w;
    DistortedUV = mul(DistortedUV - float2(0.5, 0.5), UVTransformMatrix) + float2(0.5, 0.5);
#endif

    PSOut.Color = g_Texture.Sample(g_Texture_sampler, DistortedUV);
}
)";

void DiligentAppBase::createRT_OffScreen(PipelineManager & pipelineManager) {
    auto & pso = pipelineManager.createPipeline(PIPELINE_RT_KEY);
    pso.setType(Diligent::PIPELINE_TYPE_GRAPHICS);
    pso.setNumberOfTargets(1);
    pso.setFormat(m_pSwapChain);
    pso.setPrimitiveTopology(Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
    pso.setCullMode(Diligent::CULL_MODE_BACK);
    pso.setDepthTesting(false);

    Diligent::ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;

    // Create a vertex shader
    Diligent::RefCntAutoPtr<Diligent::IShader> pVS_RT;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Render Target Vertex Shader";
        ShaderCI.Source          = VS_RT;
        m_pDevice->CreateShader(ShaderCI, &pVS_RT);
    }


#if PLATFORM_ANDROID
    // Vulkan on mobile platforms may require handling surface pre-transforms
    const bool TransformUVCoords = m_pDevice->GetDeviceInfo().IsVulkanDevice();
#else
    constexpr bool TransformUVCoords = false;
#endif

    Diligent::ShaderMacroHelper Macros;
    Macros.AddShaderMacro("TRANSFORM_UV", TransformUVCoords);
    ShaderCI.Macros = Macros;

    // Create a pixel shader
    Diligent::RefCntAutoPtr<Diligent::IShader> pPS_RT;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Render Target Pixel Shader";
        ShaderCI.Source          = PS_RT;

        m_pDevice->CreateShader(ShaderCI, &pPS_RT);

        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        Diligent::BufferDesc CBDesc;
        CBDesc.Name           = "Render Target Pixel Shader Constants Buffer";
        CBDesc.uiSizeInBytes  = sizeof(Diligent::float4) + sizeof(Diligent::float2x2) * 2;
        CBDesc.Usage          = Diligent::USAGE_DYNAMIC;
        CBDesc.BindFlags      = Diligent::BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
        m_pDevice->CreateBuffer(CBDesc, nullptr, &m_PSConstants_RT);
    }
    
    // Define vertex shader input layout
    pso.setInputLayout(
        {
            Diligent::LayoutElement{0, 0, 3, Diligent::VT_FLOAT32, false},
            Diligent::LayoutElement{1, 0, 3, Diligent::VT_FLOAT32, false},
            Diligent::LayoutElement{2, 0, 3, Diligent::VT_FLOAT32, false},
            Diligent::LayoutElement{3, 0, 3, Diligent::VT_FLOAT32, false},
        }
    );
    
    pso.setShaders(pVS_RT, pPS_RT);
    
    // Define variable type that will be used by default
    pso.setDefaultResourceLayoutVariableType(Diligent::SHADER_RESOURCE_VARIABLE_TYPE_STATIC);

    // clang-format off
    // Shader variables should typically be mutable, which means they are expected
    // to change on a per-instance basis
    pso.setResourceLayoutVariables(
       {
            {
                Diligent::SHADER_TYPE_PIXEL,
                "g_Texture",
                Diligent::SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE
            }
        }
    );

    // clang-format off
    // Define immutable sampler for g_Texture. Immutable samplers should be used whenever possible
    pso.setResourceLayoutSamplers(
       {
            {
                Diligent::SHADER_TYPE_PIXEL,
                "g_Texture",
                Diligent::Sam_LinearClamp
            }
        }
    );
    
    pso.createPipelineState(m_pDevice);

    // Since we did not explcitly specify the type for Constants, default type
    // (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never change and are bound directly
    // to the pipeline state object.
    pso.getStaticVariableFromPixelShader("Constants")->Set(m_PSConstants_RT);
    
    using namespace Diligent;

    VertBuffDesc_RT.Name = "Render Target Vertex Buffer";
    VertBuffDesc_RT.Usage = Diligent::USAGE_DEFAULT;
    VertBuffDesc_RT.BindFlags = Diligent::BIND_VERTEX_BUFFER;
    
    
    int padding = 4;
    int inputCount = 3*4;
    
    
    VertBuffDesc_RT.uiSizeInBytes = sizeof(float)*inputCount*padding;
    m_pDevice->CreateBuffer(VertBuffDesc_RT, &VBData_RT, &vertexBuffer_RT);

    // clang-format off
    Uint32 Indices[] =
                   {
                           0, 1, 2, 3
                   };
    // clang-format on
    
    BufferDesc IndBuffDesc;
    IndBuffDesc.Name          = "Render Target Index Buffer";
    IndBuffDesc.Usage         = USAGE_IMMUTABLE;
    IndBuffDesc.BindFlags     = BIND_INDEX_BUFFER;
    IndBuffDesc.uiSizeInBytes = sizeof(Indices);
    BufferData IBData;
    IBData.pData    = Indices;
    IBData.DataSize = sizeof(Indices);
    m_pDevice->CreateBuffer(IndBuffDesc, &IBData, &indexBuffer_RT);
}

Diligent::ITextureView * DiligentAppBase::getColorRT_OffScreen() {
    return colorTV_RT;
}

Diligent::ITextureView * DiligentAppBase::getDepthRT_OffScreen() {
    return depthTV_RT;
}

void DiligentAppBase::resizeRT_OffScreen(PipelineManager & pipelineManager) {
    depth_texture.Attach(createDepthTexture());
    depthTV_RT = depth_texture->GetDefaultView(Diligent::TEXTURE_VIEW_DEPTH_STENCIL);

    color_texture.Attach(createColorTexture());
    colorTV_RT = color_texture->GetDefaultView(Diligent::TEXTURE_VIEW_RENDER_TARGET);
    
    auto k = pipelineManager.findPipeline(PIPELINE_RT_KEY);
    if (k.first != nullptr) {
        // We need to release and create a new SRB that references new off-screen render target SRV
        k.first->getBinding().Release();
        k.first->createShaderBinding(true);
        
        // Set render target color texture SRV in the SRB
        color_res = color_texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
        k.first->getVariableFromPixelShader("g_Texture")->Set(color_res);
    }
}

void DiligentAppBase::bindRT_Screen() {
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    m_pImmediateContext->SetRenderTargets(
        1, &pRTV, m_pSwapChain->GetDepthBufferDSV(),
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
}

void DiligentAppBase::clearColorRT_Screen(const float * color) {
    m_pImmediateContext->ClearRenderTarget(
                                           m_pSwapChain->GetCurrentBackBufferRTV(),
                                           color,
                                           Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

void DiligentAppBase::clearColorRT_Screen(const VertexEngine::Color4 & color) {
    auto colorArray = color.to_ARGB_array();
    clearColorRT_Screen(colorArray.data());
}

void DiligentAppBase::clearDepthRT_Screen(float depth) {
    m_pImmediateContext->ClearDepthStencil(m_pSwapChain->GetDepthBufferDSV(),
                                           Diligent::CLEAR_DEPTH_FLAG, depth, 0,
                                           Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

void DiligentAppBase::clearColorAndDepthRT_Screen(const float * color, float depth) {
    clearColorRT_Screen(color);
    clearDepthRT_Screen(depth);
}

void DiligentAppBase::clearColorAndDepthRT_Screen(const VertexEngine::Color4 & color, float depth) {
    auto colorArray = color.to_ARGB_array();
    clearColorAndDepthRT_Screen(colorArray.data(), depth);
}

void DiligentAppBase::draw_OffScreen_RT_To_Screen_RT(
                             PipelineManager & pipelineManager,
                             VertexEngine & engine, int x, int y, int w, int h
) {
    using namespace Diligent;
    pipelineManager.switchToPipeline(PIPELINE_RT_KEY, m_pImmediateContext);
    pipelineManager.commitShaderResourceBinding(
            PIPELINE_RT_KEY, m_pImmediateContext,
            RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );

    Uint32   offset_   = 0;
    IBuffer* pBuffs_[] = {vertexBuffer_RT};
    m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs_, &offset_, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    m_pImmediateContext->SetIndexBuffer(indexBuffer_RT, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
    int from_X = x;
    int from_Y = y;
    int to_X = x + w;
    int to_Y = y + h;
    
    auto a = engine.toNDC(from_X, to_Y, 0);
    auto b = engine.toNDC(from_X, from_Y, 0);
    auto c = engine.toNDC(to_X, to_Y, 0);
    auto d = engine.toNDC(to_X, from_Y, 0);
    
    float pos[] {
        a.x, a.y, a.z, // bottom left
        +0, +0, +0,    // padding
        +0, +0, +0,    // padding
        +0, +0, +0,    // padding
        +0, +0, +0,    // padding
        b.x, b.y, b.z, // top left
        +0, +0, +0,    // padding
        +0, +0, +0,    // padding
        +0, +0, +0,    // padding
        +0, +0, +0,    // padding
        c.x, c.y, c.z, // top right
        +0, +0, +0,    // padding
        +0, +0, +0,    // padding
        +0, +0, +0,    // padding
        +0, +0, +0,    // padding
        d.x, d.y, d.z  // bottom right
    };

    m_pImmediateContext->UpdateBuffer(
            vertexBuffer_RT,
            0, sizeof(pos),
            pos,
            Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
    
    DrawIndexedAttribs DrawAttrs_;     // This is an indexed draw call
    DrawAttrs_.IndexType  = VT_UINT32; // Index type
    DrawAttrs_.NumIndices = 4;
    // Verify the state of vertex and index buffers
    DrawAttrs_.Flags = DRAW_FLAG_VERIFY_ALL;
    m_pImmediateContext->DrawIndexed(DrawAttrs_);
}

void DiligentAppBase::destroyRT_OffScreen(PipelineManager & pipelineManager) {
    color_texture.Release();
    depth_texture.Release();
    vertexBuffer_RT.Release();
    indexBuffer_RT.Release();
    m_PSConstants_RT.Release();
    pipelineManager.destroyPipeline(PIPELINE_RT_KEY);
}

float DiligentAppBase::black[4] = { 0.0f,  0.0f,  0.0f, 1.0f };
