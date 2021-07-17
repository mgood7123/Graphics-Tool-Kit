//
// Created by Matthew Good on 30/6/21.
//

#include "RectangleView.h"

const char * RectangleView::rectangle_VS = R"(
struct VSInput
{
    float3 Pos   : ATTRIB0;
    float4 Color : ATTRIB1;
};

struct PSInput
{
    float4 Pos   : SV_POSITION;
    float4 Color : COLOR0;
};

void main(in  VSInput VSIn,
          out PSInput PSIn)
{
    PSIn.Pos   = float4(VSIn.Pos, 1.0);
    PSIn.Color = VSIn.Color;
}
)";

const char * RectangleView::rectangle_PS = R"(
struct PSInput
{
    float4 Pos   : SV_POSITION;
    float4 Color : COLOR0;
};
struct PSOutput
{
    float4 Color : SV_TARGET;
};
void main(in  PSInput  PSIn,
          out PSOutput PSOut)
{
    PSOut.Color = PSIn.Color;
}
)";

void RectangleView::create() {
    
    // create a pipeline
    // Pipeline state object encompasses configuration of all GPU stages
    
    Diligent::GraphicsPipelineStateCreateInfo PSOCreateInfo;
    
    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "Simple rectangle PSO";
    
    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = Diligent::PIPELINE_TYPE_GRAPHICS;
    
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets             = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]                = diligentAppBase->m_pSwapChain->GetDesc().ColorBufferFormat;
    // Use the depth buffer format from the swap chain
    PSOCreateInfo.GraphicsPipeline.DSVFormat                    = diligentAppBase->m_pSwapChain->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology            = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // No back face culling for this tutorial
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = Diligent::CULL_MODE_NONE;
    // Disable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
    
    // create shaders
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
        ShaderCI.Desc.Name       = "Rectangle vertex shader";
        ShaderCI.Source          = rectangle_VS;
        diligentAppBase->m_pDevice->CreateShader(ShaderCI, &pVS);
    }
    
    // Create a pixel shader
    Diligent::RefCntAutoPtr<Diligent::IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Rectangle pixel shader";
        ShaderCI.Source          = rectangle_PS;
        diligentAppBase->m_pDevice->CreateShader(ShaderCI, &pPS);
    }

    // Define vertex shader input layout
    Diligent::LayoutElement LayoutElems[] =
            {
                    // Attribute 0 - vertex position
                    Diligent::LayoutElement{0, 0, 3, Diligent::VT_FLOAT32, Diligent::False},
                    // Attribute 1 - vertex color
                    Diligent::LayoutElement{1, 0, 4, Diligent::VT_FLOAT32, Diligent::False}
            };
    // clang-format on
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(LayoutElems);

    // Finally, create the pipeline state
    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;
    diligentAppBase->m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &diligentAppBase->m_pPSO);

    // generate vertex and index info
    VertexEngine vertexEngine = createVertexEngine(400, 400);

    // Layout of this structure matches the one we defined in the pipeline state
    HANDLE colorID = vertexEngine.addDataBufferStatic(4);
    HANDLE posID   = vertexEngine.addDataBuffer(3);
    vertexEngine.order({posID, colorID});

    // vector of HANDLE objects are returned
    // we currently have no direct use for these handles
    // since our data does not need to be modified
    vertexEngine.addData({
        {colorID, { 0,  1,  0,  1}},
        {posID,   {-1,  1,  0    }},
        {posID,   { 1,  1,  0    }},
        {posID,   { 1, -1,  0    }},
        {posID,   {-1, -1,  0    }}
    });

    VertexEngine::GenerationInfo bufferData = vertexEngine.generateGL();

    // Create a vertex buffer that stores cube vertices
    Diligent::BufferDesc VertBuffDesc;
    VertBuffDesc.Name          = "Rectangle vertex buffer";
    VertBuffDesc.Usage         = Diligent::USAGE_IMMUTABLE;
    VertBuffDesc.BindFlags     = Diligent::BIND_VERTEX_BUFFER;
    VertBuffDesc.uiSizeInBytes = bufferData.sizeInBytes;
    Diligent::BufferData VBData;
    VBData.pData    = bufferData.data;
    VBData.DataSize = bufferData.sizeInBytes;
    diligentAppBase->m_pDevice->CreateBuffer(VertBuffDesc, &VBData, &m_CubeVertexBuffer);

    // clang-format off
    Diligent::Uint32 Indices[] =
    {
            0, 1, 2,
            2, 3, 0
    };
    // clang-format on

    Diligent::BufferDesc IndBuffDesc;
    IndBuffDesc.Name          = "Rectangle index buffer";
    IndBuffDesc.Usage         = Diligent::USAGE_IMMUTABLE;
    IndBuffDesc.BindFlags     = Diligent::BIND_INDEX_BUFFER;
    IndBuffDesc.uiSizeInBytes = sizeof(Indices);
    Diligent::BufferData IBData;
    IBData.pData    = Indices;
    IBData.DataSize = sizeof(Indices);
    diligentAppBase->m_pDevice->CreateBuffer(IndBuffDesc, &IBData, &m_CubeIndexBuffer);
}

void RectangleView::draw ()
{
    // Clear the back buffer
    const float ClearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
    // Let the engine perform required state transitions
    
    auto* pRTV = diligentAppBase->m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = diligentAppBase->m_pSwapChain->GetDepthBufferDSV();
    diligentAppBase->m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    diligentAppBase->m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    diligentAppBase->m_pImmediateContext->ClearDepthStencil(pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Bind vertex and index buffers
    Diligent::Uint32   offset   = 0;
    Diligent::IBuffer* pBuffs[] = {m_CubeVertexBuffer};
    diligentAppBase->m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
    diligentAppBase->m_pImmediateContext->SetIndexBuffer(m_CubeIndexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Set the pipeline state in the immediate context
    diligentAppBase->m_pImmediateContext->SetPipelineState(diligentAppBase->m_pPSO);

    Diligent::DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
    DrawAttrs.IndexType  = Diligent::VT_UINT32; // Index type
    DrawAttrs.NumIndices = 6;
    // Verify the state of vertex and index buffers
    DrawAttrs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
    diligentAppBase->m_pImmediateContext->DrawIndexed(DrawAttrs);
}

void RectangleView::destroy() {
    this->m_CubeVertexBuffer.Release();
    this->m_CubeIndexBuffer.Release();
}