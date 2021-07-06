//
// Created by Matthew Good on 1/7/21.
//

#include <DiligentCore/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include "Cube.h"

void Cube::create ()
{
    CreatePipelineState();
    CreateVertexBuffer();
    CreateIndexBuffer();
    
}

void Cube::draw ()
{
    using namespace Diligent;
    
    double CurrTime = 1;
    
    auto* pRTV = diligentAppBase->m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = diligentAppBase->m_pSwapChain->GetDepthBufferDSV();
    diligentAppBase->m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Apply rotation
    float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(CurrTime) * 1.0f) * float4x4::RotationX(-PI_F * 0.1f);
    
    // Camera is at (0, 0, -5) looking along the Z axis
    float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);
    
    
    // Get pretransform matrix that rotates the scene according the surface orientation
    float4x4 SrfPreTransform;
    const auto& SCDesc = diligentAppBase->m_pSwapChain->GetDesc();
    switch (SCDesc.PreTransform)
    {
        case SURFACE_TRANSFORM_ROTATE_90:
            // The image content is rotated 90 degrees clockwise.
            SrfPreTransform = float4x4::RotationArbitrary(float3{0, 0, 1}, -PI_F / 2.f);
        
        case SURFACE_TRANSFORM_ROTATE_180:
            // The image content is rotated 180 degrees clockwise.
            SrfPreTransform = float4x4::RotationArbitrary(float3{0, 0, 1}, -PI_F);
        
        case SURFACE_TRANSFORM_ROTATE_270:
            // The image content is rotated 270 degrees clockwise.
            SrfPreTransform = float4x4::RotationArbitrary(float3{0, 0, 1}, -PI_F * 3.f / 2.f);
        
        case SURFACE_TRANSFORM_OPTIMAL:
            UNEXPECTED("SURFACE_TRANSFORM_OPTIMAL is only valid as parameter during swap chain initialization.");
            SrfPreTransform = float4x4::Identity();
        
        case SURFACE_TRANSFORM_HORIZONTAL_MIRROR:
        case SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90:
        case SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180:
        case SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270:
            UNEXPECTED("Mirror transforms are not supported");
            SrfPreTransform = float4x4::Identity();
        
        default:
            SrfPreTransform = float4x4::Identity();
    }
    
    // Get projection matrix adjusted to the current screen orientation
    float FOV = PI_F / 4.0f;
    float NearPlane = 0.1f;
    float FarPlane = 100.f;
    
    float AspectRatio = static_cast<float>(SCDesc.Width) / static_cast<float>(SCDesc.Height);
    float XScale, YScale;
    if (SCDesc.PreTransform == SURFACE_TRANSFORM_ROTATE_90 ||
        SCDesc.PreTransform == SURFACE_TRANSFORM_ROTATE_270 ||
        SCDesc.PreTransform == SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90 ||
        SCDesc.PreTransform == SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270)
    {
        // When the screen is rotated, vertical FOV becomes horizontal FOV
        XScale = 1.f / std::tan(FOV / 2.f);
        // Aspect ratio is inversed
        YScale = XScale * AspectRatio;
    }
    else
    {
        YScale = 1.f / std::tan(FOV / 2.f);
        XScale = YScale / AspectRatio;
    }
    
    float4x4 Proj;
    Proj._11 = XScale;
    Proj._22 = YScale;
    Proj.SetNearFarClipPlanes(NearPlane, FarPlane, diligentAppBase->m_pDevice->GetDeviceInfo().IsGLDevice());
    
    // Compute world-view-projection matrix
    m_WorldViewProjMatrix = CubeModelTransform * View * SrfPreTransform * Proj;
    
    // Clear the back buffer
    const float ClearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
    diligentAppBase->m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    diligentAppBase->m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    
    {
        // Map the buffer and write current world-view-projection matrix
        MapHelper<float4x4> CBConstants(diligentAppBase->m_pImmediateContext, m_VSConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        *CBConstants = m_WorldViewProjMatrix.Transpose();
    }
    
    // Bind vertex and index buffers
    Uint32   offset   = 0;
    IBuffer* pBuffs[] = {m_CubeVertexBuffer};
    diligentAppBase->m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    diligentAppBase->m_pImmediateContext->SetIndexBuffer(m_CubeIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    
    // Set the pipeline state
    diligentAppBase->m_pImmediateContext->SetPipelineState(m_pPSO);
    // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
    // makes sure that resources are transitioned to required states.
    diligentAppBase->m_pImmediateContext->CommitShaderResources(m_pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    
    DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
    DrawAttrs.IndexType  = VT_UINT32; // Index type
    DrawAttrs.NumIndices = 36;
    // Verify the state of vertex and index buffers
    DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
    diligentAppBase->m_pImmediateContext->DrawIndexed(DrawAttrs);
}

void Cube::CreatePipelineState ()
{
    using namespace Diligent;
    
    // Pipeline state object encompasses configuration of all GPU stages
    
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    
    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "Cube PSO";
    
    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
    
    // clang-format off
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets             = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]                = diligentAppBase->m_pSwapChain->GetDesc().ColorBufferFormat;
    // Set depth buffer format which is the format of the swap chain's back buffer
    PSOCreateInfo.GraphicsPipeline.DSVFormat                    = diligentAppBase->m_pSwapChain->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // Cull back faces
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;
    // Enable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
    // clang-format on
    
    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
    
    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;
    
    // Create a vertex shader
    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Cube VS";
        ShaderCI.Source          = cube_VS;
        diligentAppBase->m_pDevice->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        BufferDesc CBDesc;
        CBDesc.Name           = "VS constants CB";
        CBDesc.uiSizeInBytes  = sizeof(float4x4);
        CBDesc.Usage          = USAGE_DYNAMIC;
        CBDesc.BindFlags      = BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        diligentAppBase->m_pDevice->CreateBuffer(CBDesc, nullptr, &m_VSConstants);
    }
    
    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Cube PS";
        ShaderCI.Source          = cube_PS;
        diligentAppBase->m_pDevice->CreateShader(ShaderCI, &pPS);
    }
    
    // clang-format off
    // Define vertex shader input layout
    LayoutElement LayoutElems[] =
                          {
                                  // Attribute 0 - vertex position
                                  LayoutElement{0, 0, 3, VT_FLOAT32, False},
                                  // Attribute 1 - vertex color
                                  LayoutElement{1, 0, 4, VT_FLOAT32, False}
                          };
    // clang-format on
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(LayoutElems);
    
    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;
    
    // Define variable type that will be used by default
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    
    diligentAppBase->m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
    
    // Since we did not explcitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never
    // change and are bound directly through the pipeline state object.
    m_pPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_VSConstants);
    
    // Create a shader resource binding object and bind all static resources in it
    m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);
    
}

void Cube::CreateVertexBuffer ()
{
    using namespace Diligent;
    
    // Layout of this structure matches the one we defined in the pipeline state
    struct Vertex
    {
        float3 pos;
        float4 color;
    };
    
    // Cube vertices
    
    //      (-1,+1,+1)________________(+1,+1,+1)
    //               /|              /|
    //              / |             / |
    //             /  |            /  |
    //            /   |           /   |
    //(-1,-1,+1) /____|__________/(+1,-1,+1)
    //           |    |__________|____|
    //           |   /(-1,+1,-1) |    /(+1,+1,-1)
    //           |  /            |   /
    //           | /             |  /
    //           |/              | /
    //           /_______________|/
    //        (-1,-1,-1)       (+1,-1,-1)
    //
    
    // clang-format off
    Vertex CubeVerts[8] =
                   {
                           {float3(-1,-1,-1), float4(1,0,0,1)},
                           {float3(-1,+1,-1), float4(0,1,0,1)},
                           {float3(+1,+1,-1), float4(0,0,1,1)},
                           {float3(+1,-1,-1), float4(1,1,1,1)},
                    
                           {float3(-1,-1,+1), float4(1,1,0,1)},
                           {float3(-1,+1,+1), float4(0,1,1,1)},
                           {float3(+1,+1,+1), float4(1,0,1,1)},
                           {float3(+1,-1,+1), float4(0.2f,0.2f,0.2f,1)},
                   };
    // clang-format on
    
    // Create a vertex buffer that stores cube vertices
    BufferDesc VertBuffDesc;
    VertBuffDesc.Name          = "Cube vertex buffer";
    VertBuffDesc.Usage         = USAGE_IMMUTABLE;
    VertBuffDesc.BindFlags     = BIND_VERTEX_BUFFER;
    VertBuffDesc.uiSizeInBytes = sizeof(CubeVerts);
    BufferData VBData;
    VBData.pData    = CubeVerts;
    VBData.DataSize = sizeof(CubeVerts);
    diligentAppBase->m_pDevice->CreateBuffer(VertBuffDesc, &VBData, &m_CubeVertexBuffer);
    
}

void Cube::CreateIndexBuffer ()
{
    using namespace Diligent;
    
    // clang-format off
    Uint32 Indices[] =
                   {
                           2,0,1, 2,3,0,
                           4,6,5, 4,7,6,
                           0,7,4, 0,3,7,
                           1,0,4, 1,4,5,
                           1,5,2, 5,6,2,
                           3,6,7, 3,2,6
                   };
    // clang-format on
    
    BufferDesc IndBuffDesc;
    IndBuffDesc.Name          = "Cube index buffer";
    IndBuffDesc.Usage         = USAGE_IMMUTABLE;
    IndBuffDesc.BindFlags     = BIND_INDEX_BUFFER;
    IndBuffDesc.uiSizeInBytes = sizeof(Indices);
    BufferData IBData;
    IBData.pData    = Indices;
    IBData.DataSize = sizeof(Indices);
    diligentAppBase->m_pDevice->CreateBuffer(IndBuffDesc, &IBData, &m_CubeIndexBuffer);
}

const char * Cube::cube_VS = "cbuffer Constants\n"
                             "{\n"
                             "    float4x4 g_WorldViewProj;\n"
                             "};\n"
                             "\n"
                             "// Vertex shader takes two inputs: vertex position and color.\n"
                             "// By convention, Diligent Engine expects vertex shader inputs to be \n"
                             "// labeled 'ATTRIBn', where n is the attribute number.\n"
                             "struct VSInput\n"
                             "{\n"
                             "    float3 Pos   : ATTRIB0;\n"
                             "    float4 Color : ATTRIB1;\n"
                             "};\n"
                             "\n"
                             "struct PSInput \n"
                             "{ \n"
                             "    float4 Pos   : SV_POSITION; \n"
                             "    float4 Color : COLOR0; \n"
                             "};\n"
                             "\n"
                             "// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex\n"
                             "// shader output variable name must match exactly the name of the pixel shader input variable.\n"
                             "// If the variable has structure type (like in this example), the structure declarations must also be indentical.\n"
                             "void main(in  VSInput VSIn,\n"
                             "          out PSInput PSIn) \n"
                             "{\n"
                             "    PSIn.Pos   = mul( float4(VSIn.Pos,1.0), g_WorldViewProj);\n"
                             "    PSIn.Color = VSIn.Color;\n"
                             "}";

const char * Cube::cube_PS = "struct PSInput \n"
                             "{ \n"
                             "    float4 Pos   : SV_POSITION; \n"
                             "    float4 Color : COLOR0; \n"
                             "};\n"
                             "\n"
                             "struct PSOutput\n"
                             "{ \n"
                             "    float4 Color : SV_TARGET; \n"
                             "};\n"
                             "\n"
                             "// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex\n"
                             "// shader output variable name must match exactly the name of the pixel shader input variable.\n"
                             "// If the variable has structure type (like in this example), the structure declarations must also be indentical.\n"
                             "void main(in  PSInput  PSIn,\n"
                             "          out PSOutput PSOut)\n"
                             "{\n"
                             "    PSOut.Color = PSIn.Color; \n"
                             "}";