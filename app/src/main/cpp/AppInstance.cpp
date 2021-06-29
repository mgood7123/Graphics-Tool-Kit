//
// Created by Matthew Good on 29/6/21.
//

#include "AppInstance.h"

class TriangleObject
{
public:
    static const char * triangle_VS;
    static const char * triangle_PS;
    static void create(DiligentAppBase * base);
    static void draw(DiligentAppBase * base);
};

AppInstance::AppInstance ()
{
}

AppInstance::~AppInstance ()
{
}

void AppInstance::onEglSetup (JNIEnv * jenv, jobject classInstance, jstring name, jstring signature)
{
    AppInstanceAndroidBase::onEglSetup(jenv, classInstance, name, signature);
}

void AppInstance::surfaceChanged (int w, int h)
{
    if (m_pSwapChain)
    {
        m_pSwapChain->Resize(w, h);
    }
    else
    {
        attachToContext(w, h);
        TriangleObject::create(this);
    }
}

bool AppInstance::onTouchEvent (JNIEnv * jenv, jfloatArray motionEventData)
{
    return AppInstanceAndroidBase::onTouchEvent(jenv, motionEventData);
}

void AppInstance::onDraw ()
{
    TriangleObject::draw(this);
    swapBuffers();
}

void AppInstance::onEglTearDown ()
{
    m_pImmediateContext->Flush();
    m_pImmediateContext.Release();
    m_pSwapChain.Release();
    m_pDevice.Release();
    m_pPSO.Release();
    AppInstanceAndroidBase::onEglTearDown();
}

void AppInstance::swapBuffers ()
{
    AppInstanceAndroidBase::swapBuffers();
}



// TriangleObject

const char * TriangleObject::triangle_VS = R"(
struct PSInput
{
    float4 Pos   : SV_POSITION;
    float3 Color : COLOR;
};
void main(in  uint    VertId : SV_VertexID,
          out PSInput PSIn)
{
    float4 Pos[3];
    Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
    Pos[1] = float4( 0.0, +0.5, 0.0, 1.0);
    Pos[2] = float4(+0.5, -0.5, 0.0, 1.0);
    float3 Col[3];
    Col[0] = float3(1.0, 0.0, 0.0); // red
    Col[1] = float3(0.0, 1.0, 0.0); // green
    Col[2] = float3(0.0, 0.0, 1.0); // blue
    PSIn.Pos   = Pos[VertId];
    PSIn.Color = Col[VertId];
}
)";

const char * TriangleObject::triangle_PS = R"(
struct PSInput
{
    float4 Pos   : SV_POSITION;
    float3 Color : COLOR;
};
struct PSOutput
{
    float4 Color : SV_TARGET;
};
void main(in  PSInput  PSIn,
          out PSOutput PSOut)
{
    PSOut.Color = float4(PSIn.Color.rgb, 1.0);
}
)";

void TriangleObject::create(DiligentAppBase * base) {
    
    // create a pipeline
    // Pipeline state object encompasses configuration of all GPU stages
    
    Diligent::GraphicsPipelineStateCreateInfo PSOCreateInfo;
    
    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "Simple triangle PSO";
    
    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = Diligent::PIPELINE_TYPE_GRAPHICS;
    
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets             = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]                = base->m_pSwapChain->GetDesc().ColorBufferFormat;
    // Use the depth buffer format from the swap chain
    PSOCreateInfo.GraphicsPipeline.DSVFormat                    = base->m_pSwapChain->GetDesc().DepthBufferFormat;
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
        ShaderCI.Desc.Name       = "Triangle vertex shader";
        ShaderCI.Source          = triangle_VS;
        base->m_pDevice->CreateShader(ShaderCI, &pVS);
    }
    
    // Create a pixel shader
    Diligent::RefCntAutoPtr<Diligent::IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Triangle pixel shader";
        ShaderCI.Source          = triangle_PS;
        base->m_pDevice->CreateShader(ShaderCI, &pPS);
    }
    
    // Finally, create the pipeline state
    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;
    base->m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &base->m_pPSO);
}

void TriangleObject::draw (DiligentAppBase * base)
{
    // Clear the back buffer
    const float ClearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
    // Let the engine perform required state transitions
    
    auto* pRTV = base->m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = base->m_pSwapChain->GetDepthBufferDSV();
    base->m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    base->m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    base->m_pImmediateContext->ClearDepthStencil(pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    
    // Set the pipeline state in the immediate context
    base->m_pImmediateContext->SetPipelineState(base->m_pPSO);

    // Typically we should now call CommitShaderResources(), however shaders in this example don't
    // use any resources.
    
    Diligent::DrawAttribs drawAttrs;
    drawAttrs.NumVertices = 3; // We will render 3 vertices
    base->m_pImmediateContext->Draw(drawAttrs);
}
