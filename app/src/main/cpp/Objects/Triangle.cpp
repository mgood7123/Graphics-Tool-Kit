//
// Created by Matthew Good on 30/6/21.
//

#include "Triangle.h"

const char * Triangle::triangle_VS = R"(
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

const char * Triangle::triangle_PS = R"(
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

void Triangle::createPipeline(PipelineManager & pipelineManager) {
    auto & pso = pipelineManager.createPipeline("Triangle");
    pso.setType(Diligent::PIPELINE_TYPE_GRAPHICS);
    pso.setNumberOfTargets(1);
    pso.setFormat(diligentAppBase->m_pSwapChain);
    pso.setPrimitiveTopology(Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    pso.setCullMode(Diligent::CULL_MODE_NONE);
    pso.setDepthTesting(false);

    // create shaders
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
        ShaderCI.Desc.Name       = "Triangle vertex shader";
        ShaderCI.Source          = triangle_VS;
        diligentAppBase->m_pDevice->CreateShader(ShaderCI, &pVS);
    }

    // Create a pixel shader
    Diligent::IShader * pPS = nullptr;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Triangle pixel shader";
        ShaderCI.Source          = triangle_PS;
        diligentAppBase->m_pDevice->CreateShader(ShaderCI, &pPS);
    }

    // Finally, create the pipeline state
    pso.setShaders(pVS, pPS);
    pso.createPipelineState(diligentAppBase->m_pDevice);
}

void Triangle::switchToPipeline(PipelineManager & pipelineManager) {
    pipelineManager.switchToPipeline(
            PIPELINE_KEY, diligentAppBase->m_pImmediateContext
    );
}

void Triangle::bindShaderResources(PipelineManager & pipelineManager) {
    pipelineManager.commitShaderResourceBinding(
            PIPELINE_KEY, diligentAppBase->m_pImmediateContext,
            Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
}

void Triangle::destroyPipeline(PipelineManager & pipelineManager) {
    pipelineManager.destroyPipeline(PIPELINE_KEY);
}

void Triangle::draw (PipelineManager & pipelineManager)
{
    // Clear the back buffer
    const float ClearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
    // Let the engine perform required state transitions

    auto* pRTV = diligentAppBase->getColorRT_OffScreen();
    auto* pDSV = diligentAppBase->getDepthRT_OffScreen();
    diligentAppBase->m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    diligentAppBase->m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    diligentAppBase->m_pImmediateContext->ClearDepthStencil(pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    Diligent::DrawAttribs drawAttrs;
    drawAttrs.NumVertices = 3; // We will render 3 vertices
    diligentAppBase->m_pImmediateContext->Draw(drawAttrs);

    VertexEngine v(500, 500);
    int w = 300;
    int h = 200;
    diligentAppBase->bindRT_Screen();
    diligentAppBase->clearColorAndDepthRT_Screen(DiligentAppBase::black, 1);
    diligentAppBase->draw_OffScreen_RT_To_Screen_RT(pipelineManager, v, 0, 0, w, h);
}
