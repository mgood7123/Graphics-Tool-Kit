//
// Created by Matthew Good on 1/7/21.
//

#include <DiligentCore/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include "Cube.h"

void Cube::createPipeline(PipelineManager & pipelineManager) {
    auto & pso = pipelineManager.createPipeline(this, PIPELINE_KEY);
    pso.setType(Diligent::PIPELINE_TYPE_GRAPHICS);
    pso.setNumberOfTargets(1);
    pso.setFormat(getDiligentAppBase().m_pSwapChain);
    pso.setPrimitiveTopology(Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    pso.setCullMode(Diligent::CULL_MODE_BACK);
    pso.setDepthTesting(true);

    Diligent::ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;

    // Create a vertex shader
    Diligent::IShader * pVS = nullptr;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Cube VS";
        ShaderCI.Source          = cube_VS;
        getDiligentAppBase().m_pDevice->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        Diligent::BufferDesc CBDesc;
        CBDesc.Name           = "VS constants CB";
        CBDesc.uiSizeInBytes  = sizeof(Diligent::float4x4);
        CBDesc.Usage          = Diligent::USAGE_DYNAMIC;
        CBDesc.BindFlags      = Diligent::BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
        getDiligentAppBase().m_pDevice->CreateBuffer(CBDesc, nullptr, &m_VSConstants);
    }

    // Create a pixel shader
    Diligent::IShader * pPS = nullptr;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Cube PS";
        ShaderCI.Source          = cube_PS;
        getDiligentAppBase().m_pDevice->CreateShader(ShaderCI, &pPS);
    }

    // clang-format off
    pso.setInputLayout(
            {
                    // Attribute 0 - vertex position
                    Diligent::LayoutElement{0, 0, 3, Diligent::VT_FLOAT32, false},
                    // Attribute 1 - vertex color
                    Diligent::LayoutElement{1, 0, 4, Diligent::VT_FLOAT32, false}
            }
    );

    pso.setShaders(pVS, pPS);
    pso.setDefaultResourceLayoutVariableType(Diligent::SHADER_RESOURCE_VARIABLE_TYPE_STATIC);
    pso.createPipelineState(getDiligentAppBase().m_pDevice);
    pso.getStaticVariableFromVertexShader("Constants")->Set(m_VSConstants);
    pso.createShaderBinding(true);
}

void Cube::switchToPipeline(PipelineManager &pipelineManager) {
    pipelineManager.switchToPipeline(
            this, PIPELINE_KEY, getDiligentAppBase().m_pImmediateContext
    );
}

void Cube::bindShaderResources(PipelineManager &pipelineManager) {
    pipelineManager.commitShaderResourceBinding(
            this, PIPELINE_KEY, getDiligentAppBase().m_pImmediateContext,
            Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
}

void Cube::destroyPipeline(PipelineManager &pipelineManager) {
    m_VSConstants.Release();
    pipelineManager.destroyPipeline(this, PIPELINE_KEY);
}

void Cube::create ()
{
    using namespace Diligent;
    obj_y = 0.0;
    auto radY = degreesToRadians(obj_y);
    CubeModelTransform = float4x4::RotationY(radY) * float4x4::RotationX(-PI_F * 0.1f);
    CreateVertexBuffer();
    CreateIndexBuffer();
}

bool Cube::hasPhysics()
{
    return true;
}

void Cube::physics (const TimeEngine & timeEngine)
{
    using namespace Diligent;

    obj_y += 70.0 * timeEngine.delta;
    auto radY = degreesToRadians(obj_y);
    CubeModelTransform = float4x4::RotationY(radY) * float4x4::RotationX(-PI_F * 0.1f);
}

void Cube::computeViewModel() {
    using namespace Diligent;
    // Camera is at (0, 0, -5) looking along the Z axis
    float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);


    // Get pretransform matrix that rotates the scene according the surface orientation
    float4x4 SrfPreTransform;
    const auto& SCDesc = getDiligentAppBase().m_pSwapChain->GetDesc();
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
    Proj.SetNearFarClipPlanes(NearPlane, FarPlane, getDiligentAppBase().m_pDevice->GetDeviceInfo().IsGLDevice());

    // Compute world-view-projection matrix
    m_WorldViewProjMatrix = CubeModelTransform * View * SrfPreTransform * Proj;
}

void Cube::resize(PipelineManager & pipelineManager) {
}

void Cube::draw (DrawTools & drawTools, RenderTarget & renderTarget)
{
    using namespace Diligent;

    auto & app = getDiligentAppBase();

    renderTarget.bind(app.m_pImmediateContext);
    renderTarget.clearColorAndDepth(
            {0.350f, 0.350f, 0.350f, 1.0f},
            1.0f,
            app.m_pImmediateContext
    );

    {
        computeViewModel();
        // Map the buffer and write current world-view-projection matrix
        MapHelper<float4x4> CBConstants(app.m_pImmediateContext, m_VSConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        *CBConstants = m_WorldViewProjMatrix.Transpose();
    }
    
    // Bind vertex and index buffers
    Uint32   offset   = 0;
    IBuffer* pBuffs[] = {m_CubeVertexBuffer};
    app.m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    app.m_pImmediateContext->SetIndexBuffer(m_CubeIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
    DrawAttrs.IndexType  = VT_UINT32; // Index type
    DrawAttrs.NumIndices = 36;
    // Verify the state of vertex and index buffers
    DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
    app.m_pImmediateContext->DrawIndexed(DrawAttrs);
    

//    // draw a checker pattern
//
//    drawTools.pixelToNDC.resize(500, 500);
//    int w = 100;
//    int h = 100;
//    getDiligentAppBase().bindRT_Screen();
//    getDiligentAppBase().clearColorAndDepthRT_Screen(DiligentAppBase::black, 1);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 0, 0, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 0, 200, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 0, 400, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 200, 0, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 200, 200, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 200, 400, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 400, 0, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 400, 200, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 400, 400, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 100, 100, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 100, 300, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 300, 100, w, h);
//    getDiligentAppBase().draw_OffScreen_RT_To_Screen_RT(drawTools, 300, 300, w, h);
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
    getDiligentAppBase().m_pDevice->CreateBuffer(VertBuffDesc, &VBData, &m_CubeVertexBuffer);
    
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
    getDiligentAppBase().m_pDevice->CreateBuffer(IndBuffDesc, &IBData, &m_CubeIndexBuffer);
}

void Cube::destroy() {
    m_CubeVertexBuffer.Release();
    m_CubeIndexBuffer.Release();
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
