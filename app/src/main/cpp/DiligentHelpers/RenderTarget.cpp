//
// Created by Matthew Good on 8/8/21.
//

#include <DiligentCore/Graphics/GraphicsTools/interface/CommonlyUsedStates.h>
#include <DiligentCore/Graphics/GraphicsTools/interface/ShaderMacroHelper.hpp>
#include <DiligentCore/Common/interface/BasicMath.hpp>
#include <VertexEngine/VertexEngine.h>
#include "RenderTarget.h"

const char * RenderTarget::VS = R"(
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

const char * RenderTarget::PS = R"(
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

int RenderTarget::getWidth() const {
    return width;
}

int RenderTarget::getHeight() const {
    return height;
}

RenderTarget::~RenderTarget() {
    if (this->PIPELINE_KEY != nullptr) {
        Log::Error_And_Throw("`destroy(", this->PIPELINE_KEY, ");` has not been called");
    }
}


void RenderTarget::create(const char * PIPELINE_KEY, PipelineManager & pipelineManager, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice) {
    if (this->PIPELINE_KEY != nullptr) {
        Log::Error_And_Throw("attempting to create a RenderTarget without first destroying existing RenderTarget, please call `destroy(", this->PIPELINE_KEY, ");` before calling `create(", PIPELINE_KEY, ");`");
    }
    this->PIPELINE_KEY = PIPELINE_KEY;
    auto & pso = pipelineManager.createPipeline(this, PIPELINE_KEY);
    pso.setType(Diligent::PIPELINE_TYPE_GRAPHICS);
    pso.setNumberOfTargets(1);
    pso.setFormat(swapChain);
    pso.setPrimitiveTopology(Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
    pso.setCullMode(Diligent::CULL_MODE_BACK);
    pso.setDepthTesting(false);
    pso.setScissorEnable(true);

    Diligent::ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;

    // Create a vertex shader
    Diligent::RefCntAutoPtr<Diligent::IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Render Target Vertex Shader";
        ShaderCI.Source          = VS;
        renderDevice->CreateShader(ShaderCI, &pVS);
    }


#if PLATFORM_ANDROID
    // Vulkan on mobile platforms may require handling surface pre-transforms
    const bool TransformUVCoords = renderDevice->GetDeviceInfo().IsVulkanDevice();
#else
    constexpr bool TransformUVCoords = false;
#endif

    Diligent::ShaderMacroHelper Macros;
    Macros.AddShaderMacro("TRANSFORM_UV", TransformUVCoords);
    ShaderCI.Macros = Macros;

    // Create a pixel shader
    Diligent::RefCntAutoPtr<Diligent::IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Render Target Pixel Shader";
        ShaderCI.Source          = PS;

        renderDevice->CreateShader(ShaderCI, &pPS);

        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        Diligent::BufferDesc CBDesc;
        CBDesc.Name           = "Render Target Pixel Shader Constants Buffer";
        CBDesc.uiSizeInBytes  = sizeof(Diligent::float4) + sizeof(Diligent::float2x2) * 2;
        CBDesc.Usage          = Diligent::USAGE_DYNAMIC;
        CBDesc.BindFlags      = Diligent::BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
        renderDevice->CreateBuffer(CBDesc, nullptr, &m_PSConstants);
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

    pso.setShaders(pVS, pPS);

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

    pso.createPipelineState(renderDevice);

    // Since we did not explcitly specify the type for Constants, default type
    // (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never change and are bound directly
    // to the pipeline state object.
    pso.getStaticVariableFromPixelShader("Constants")->Set(m_PSConstants);

    using namespace Diligent;

    VertBuffDesc.Name = "Render Target Vertex Buffer";
    VertBuffDesc.Usage = Diligent::USAGE_DEFAULT;
    VertBuffDesc.BindFlags = Diligent::BIND_VERTEX_BUFFER;


    int padding = 4;
    int inputCount = 3*4;


    VertBuffDesc.uiSizeInBytes = sizeof(float)*inputCount*padding;
    renderDevice->CreateBuffer(VertBuffDesc, &VBData, &vertexBuffer);

    // clang-format off
    Uint32 Indices[] =
            {
                    0, 1, 2, 3
            };
    // clang-format on

    IndBuffDesc.Name          = "Render Target Index Buffer";
    IndBuffDesc.Usage         = USAGE_IMMUTABLE;
    IndBuffDesc.BindFlags     = BIND_INDEX_BUFFER;
    IndBuffDesc.uiSizeInBytes = sizeof(Indices);
    IBData.pData    = Indices;
    IBData.DataSize = sizeof(Indices);
    renderDevice->CreateBuffer(IndBuffDesc, &IBData, &indexBuffer);
}

Diligent::ITextureView * RenderTarget::getColor() {
    return colorTV;
}

Diligent::ITextureView * RenderTarget::getDepth() {
    return depthTV;
}

Diligent::ITexture * RenderTarget::createColorTexture(Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice) {
    auto desc = swapChain->GetDesc();
    Diligent::TextureDesc RTColorDesc;
    RTColorDesc.Type        = Diligent::RESOURCE_DIM_TEX_2D;
    RTColorDesc.Width       = desc.Width;
    RTColorDesc.Height      = desc.Height;
    RTColorDesc.MipLevels   = 1;
    RTColorDesc.Format      = desc.ColorBufferFormat;
    RTColorDesc.BindFlags   = Diligent::BIND_SHADER_RESOURCE | Diligent::BIND_RENDER_TARGET;
    Diligent::ITexture * pRTColor = nullptr;
    renderDevice->CreateTexture(RTColorDesc, nullptr, &pRTColor);
    return pRTColor;
}

Diligent::ITexture * RenderTarget::createDepthTexture(Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice) {
    auto desc = swapChain->GetDesc();
    Diligent::TextureDesc RTDepthDesc;
    RTDepthDesc.Type        = Diligent::RESOURCE_DIM_TEX_2D;
    RTDepthDesc.Width       = desc.Width;
    RTDepthDesc.Height      = desc.Height;
    RTDepthDesc.MipLevels   = 1;
    RTDepthDesc.Format      = desc.DepthBufferFormat;
    RTDepthDesc.BindFlags   = Diligent::BIND_SHADER_RESOURCE | Diligent::BIND_DEPTH_STENCIL;
    Diligent::ITexture * pRTDepth = nullptr;
    renderDevice->CreateTexture(RTDepthDesc, nullptr, &pRTDepth);
    return pRTDepth;
}

void RenderTarget::resize(PipelineManager & pipelineManager, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice) {
    width = swapChain->GetDesc().Width;
    height = swapChain->GetDesc().Height;
    depth_texture.Attach(createDepthTexture(swapChain, renderDevice));
    depthTV = depth_texture->GetDefaultView(Diligent::TEXTURE_VIEW_DEPTH_STENCIL);

    color_texture.Attach(createColorTexture(swapChain, renderDevice));
    colorTV = color_texture->GetDefaultView(Diligent::TEXTURE_VIEW_RENDER_TARGET);

    auto k = pipelineManager.findPipeline(this, PIPELINE_KEY);
    if (k.first != nullptr) {
        // We need to release and create a new SRB that references new off-screen render target SRV
        k.first->getBinding().Release();
        k.first->createShaderBinding(true);

        // Set render target color texture SRV in the SRB
        color_res = color_texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
        k.first->getVariableFromPixelShader("g_Texture")->Set(color_res);
    }
}

void RenderTarget::bind(Diligent::IDeviceContext * deviceContext) {
    bind(colorTV, depthTV, deviceContext);
}

void RenderTarget::bind(Diligent::ITextureView * color, Diligent::ITextureView * depth, Diligent::IDeviceContext * deviceContext) {
    deviceContext->SetRenderTargets(1, &color, depth, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    Diligent::Rect r {0, 0, width, height};
    deviceContext->SetScissorRects(1, &r, width, height);
}

void RenderTarget::clearColor(const float * color, Diligent::IDeviceContext * deviceContext) {
    clearColor(color, colorTV, deviceContext);
}

void RenderTarget::clearColor(const float * color, Diligent::ITextureView * colorTV, Diligent::IDeviceContext * deviceContext) {
    deviceContext->ClearRenderTarget(
            colorTV, color,
            Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
}

void RenderTarget::clearColor(const VertexEngine::Color4 & color, Diligent::IDeviceContext * deviceContext) {
    clearColor(color, colorTV, deviceContext);
}

void RenderTarget::clearColor(const VertexEngine::Color4 & color, Diligent::ITextureView * colorTV, Diligent::IDeviceContext * deviceContext) {
    auto colorArray = color.to_RGBA_array();
    clearColor(colorArray.data(), colorTV, deviceContext);
}

void RenderTarget::clearDepth(float depth, Diligent::IDeviceContext * deviceContext) {
    clearDepth(depth, depthTV, deviceContext);
}

void RenderTarget::clearDepth(float depth, Diligent::ITextureView * depthTV, Diligent::IDeviceContext * deviceContext) {
    deviceContext->ClearDepthStencil(depthTV,
                                     Diligent::CLEAR_DEPTH_FLAG, depth, 0,
                                     Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

void RenderTarget::clearColorAndDepth(const float * color, float depth, Diligent::IDeviceContext * deviceContext) {
    clearColorAndDepth(color, depth, colorTV, depthTV, deviceContext);
}

void RenderTarget::clearColorAndDepth(const float *color, float depth, Diligent::ITextureView *colorTV, Diligent::ITextureView *depthTV, Diligent::IDeviceContext *deviceContext) {
    clearColor(color, colorTV, deviceContext);
    clearDepth(depth, depthTV, deviceContext);
}

void RenderTarget::clearColorAndDepth(const VertexEngine::Color4 & color, float depth, Diligent::IDeviceContext * deviceContext) {
    clearColorAndDepth(color, depth, colorTV, depthTV, deviceContext);
}

void RenderTarget::clearColorAndDepth(const VertexEngine::Color4 & color, float depth, Diligent::ITextureView *colorTV, Diligent::ITextureView *depthTV, Diligent::IDeviceContext * deviceContext) {
    auto colorArray = color.to_RGBA_array();
    clearColorAndDepth(colorArray.data(), depth, colorTV, depthTV, deviceContext);
}

void RenderTarget::clip(const Position & position, Diligent::IDeviceContext * deviceContext) {
    clip(position.x, position.y, position.width, position.height, deviceContext);
}

void RenderTarget::clip(const float & x, const float & y, const float & w, const float & h, Diligent::IDeviceContext * deviceContext) {
    Diligent::Rect r {
        static_cast<Diligent::Int32>(x),
        static_cast<Diligent::Int32>(y),
        static_cast<Diligent::Int32>(w),
        static_cast<Diligent::Int32>(h),
    };
    
    deviceContext->SetScissorRects(1, &r, width, height);
}

void RenderTarget::draw(
        DrawTools & drawTools, const Position & position, Diligent::IDeviceContext * deviceContext
) {
    draw(drawTools, position.x, position.y, position.width, position.height, deviceContext);
}

void RenderTarget::draw(
        DrawTools & drawTools, const int & x, const int & y, const int & w, const int & h, Diligent::IDeviceContext * deviceContext
) {
    drawTools.pipelineManager.switchToPipeline(this, PIPELINE_KEY, deviceContext);
    drawTools.pipelineManager.commitShaderResourceBinding(
            this, PIPELINE_KEY, deviceContext,
            Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );

    Diligent::Uint32   offset_   = 0;
    Diligent::IBuffer* pBuffs_[] = {vertexBuffer};
    deviceContext->SetVertexBuffers(0, 1, pBuffs_, &offset_, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
    deviceContext->SetIndexBuffer(indexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    int from_X = x;
    int from_Y = y;
    int to_X = x + (w == Position::MATCH_PARENT ? drawTools.pixelToNDC.width : w);
    int to_Y = y + (h == Position::MATCH_PARENT ? drawTools.pixelToNDC.height : h);

    auto a = drawTools.pixelToNDC.toNDC<int, float>(from_X, to_Y, 0);
    auto b = drawTools.pixelToNDC.toNDC<int, float>(from_X, from_Y, 0);
    auto c = drawTools.pixelToNDC.toNDC<int, float>(to_X, to_Y, 0);
    auto d = drawTools.pixelToNDC.toNDC<int, float>(to_X, from_Y, 0);

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

    deviceContext->UpdateBuffer(
            vertexBuffer,
            0, sizeof(pos),
            pos,
            Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );

    Diligent::DrawIndexedAttribs DrawAttrs_;     // This is an indexed draw call
    DrawAttrs_.IndexType  = Diligent::VT_UINT32; // Index type
    DrawAttrs_.NumIndices = 4;
    // Verify the state of vertex and index buffers
    DrawAttrs_.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
    deviceContext->DrawIndexed(DrawAttrs_);
}

void RenderTarget::destroy(PipelineManager & pipelineManager) {
    color_texture.Release();
    depth_texture.Release();
    vertexBuffer.Release();
    indexBuffer.Release();
    m_PSConstants.Release();
    pipelineManager.destroyPipeline(this, PIPELINE_KEY);
    PIPELINE_KEY = nullptr;
}

float RenderTarget::black[4] = { 0.0f,  0.0f,  0.0f, 1.0f };
