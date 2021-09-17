//
// Created by Matthew Good on 14/7/21.
//

#include "PainterView.h"

const char * PainterView::vertexShader = R"(
struct VSInput
{
    float3 Pos       : ATTRIB0;
    float4 Color     : ATTRIB1;
    float  IsTexture : ATTRIB2;
    float2 Texture   : ATTRIB3;
};

struct PSInput
{
    float4 Pos       : SV_POSITION;
    float4 Color     : COLOR0;
    float2 IsTexture : TEX_COORD;
    float2 Texture   : TEX_COORD;
};

void main(in  VSInput VSIn,
          out PSInput PSIn)
{
    PSIn.Pos   = float4(VSIn.Pos, 1.0);
    PSIn.Color = VSIn.Color;
    PSIn.IsTexture = float2(VSIn.IsTexture, 0);
    PSIn.Texture = VSIn.Texture;
}
)";

const char * PainterView::pixelShader = R"(
Texture2D    g_Texture;
SamplerState g_Texture_sampler;

struct PSInput
{
    float4 Pos       : SV_POSITION;
    float4 Color     : COLOR0;
    float2 IsTexture : TEX_COORD;
    float2 Texture   : TEX_COORD;
};
struct PSOutput
{
    float4 Color : SV_TARGET;
};
void main(in  PSInput  PSIn,
          out PSOutput PSOut)
{
    PSOut.Color = PSIn.IsTexture.x == 0.0 ? PSIn.Color : g_Texture.Sample(g_Texture_sampler, PSIn.Texture);
}
)";

void PainterView::createState() {
    VertBuffDesc.Name = "PainterView vertex buffer";
    VertBuffDesc.Usage = Diligent::USAGE_DEFAULT;
    VertBuffDesc.BindFlags = Diligent::BIND_VERTEX_BUFFER;
    VertBuffDesc.uiSizeInBytes = sizeof(float)*(VertexEngine::strideLength*chunkSize);

    IndBuffDesc.Name = "PainterView index buffer";
    IndBuffDesc.Usage = Diligent::USAGE_DEFAULT;
    IndBuffDesc.BindFlags = Diligent::BIND_INDEX_BUFFER;
    IndBuffDesc.uiSizeInBytes = sizeof(uint32_t)*chunkSize;

    getDiligentAppBase().m_pDevice->CreateBuffer(VertBuffDesc, &VBData, &vertexBuffer);
    getDiligentAppBase().m_pDevice->CreateBuffer(IndBuffDesc, &IBData, &indexBuffer);

    vertexEngine.textureManager->setDefaultDevices(
            getDiligentAppBase().m_pDevice.RawPtr(),
            getDiligentAppBase().m_pImmediateContext.RawPtr()
    );

    vertexEngine.textureManager->createSolidColorTexture(DUMMY_TEXTURE_KEY, {0,0,0,1});
    auto c = vertexEngine.textureManager->getTexture(DUMMY_TEXTURE_KEY)->third;
    dummyTextureColorRef = c.to_RGBA_array();
    dummyTextureView = c.texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);

    onCreateState(*vertexEngine.textureManager);
}

void PainterView::draw(DrawTools & drawTools, RenderTarget & screenRenderTarget, RenderTarget & renderTarget) {

    // TODO

    auto & app = getDiligentAppBase();

    renderTarget.bind(app.m_pImmediateContext);
    renderTarget.clearColorAndDepth(
            {0.350f, 0.350f, 0.350f, 1.0f},
            1.0f,
            app.m_pImmediateContext
    );

    // Bind the vertex and index buffers
    Diligent::Uint32   offset   = 0;
    Diligent::IBuffer* pBuffs[] = {vertexBuffer};
    app.m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
    app.m_pImmediateContext->SetIndexBuffer(indexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    vertexEngine.clear();
    VertexEngine::Canvas canvas(&vertexEngine, getCanvasWidth(), getCanvasHeight());
    onDraw(canvas);

    drawChunks(canvas.generateGL(), drawTools);
}

void PainterView::drawChunks(VertexEngine::GenerationInfo && info, DrawTools & drawTools) {
    auto & app = getDiligentAppBase();

    // This is an indexed draw call
    Diligent::DrawIndexedAttribs DrawAttrs;

    // Index type
    DrawAttrs.IndexType = Diligent::VT_UINT32;

    // Verify the state of vertex and index buffers
    DrawAttrs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;

    while (info.canGenerateChunk(chunkSize)) {
        VertexEngine::GenerationInfo chunk = info.generateChunk(chunkSize);

        if (chunk.isTexture) {
            if (chunk.textureIndex.has_value()) {
                size_t textureIndex = chunk.textureIndex.value();
                auto tex = vertexEngine.textureManager->getTexture(textureIndex);
                if (tex == nullptr) {
                    LOG_WARNING_MESSAGE(
                            "texture container referenced by index '", textureIndex, "' is nullptr"
                    );
                    chunk.fillColorDataAndDisableTextureFlag(dummyTextureColorRef);
                    shaderResourceVariable_Texture->Set(dummyTextureView);
                } else if (tex->third.texture == nullptr) {
                    LOG_WARNING_MESSAGE(
                            "texture referenced by key '", tex->first, "' is nullptr"
                    );
                    chunk.fillColorDataAndDisableTextureFlag(dummyTextureColorRef);
                    shaderResourceVariable_Texture->Set(dummyTextureView);
                } else {
                    auto v = tex->third.texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
                    shaderResourceVariable_Texture->Set(v);
                }
            } else {
                chunk.fillColorDataAndDisableTextureFlag(dummyTextureColorRef);
                shaderResourceVariable_Texture->Set(dummyTextureView);
            }
        } else {
            shaderResourceVariable_Texture->Set(dummyTextureView);
        }

        bindShaderResources(drawTools.pipelineManager);

        app.m_pImmediateContext->UpdateBuffer(
                vertexBuffer,
                0, chunk.sizeInBytesData,
                chunk.data,
                Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
        );

        app.m_pImmediateContext->UpdateBuffer(
                indexBuffer,
                0, chunk.sizeInBytesIndices,
                chunk.indices,
                Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
        );

        DrawAttrs.NumIndices = chunk.lengthIndices;

        app.m_pImmediateContext->DrawIndexed(DrawAttrs);
    }
    // LOG_INFO_MESSAGE("drawn ", info.chunksGenerated, " chunk", info.chunksGenerated == 1 ? "" : "s");
}

void PainterView::destroyState() {
    onDestroyState(*vertexEngine.textureManager);
    vertexEngine.textureManager->deleteTexture(DUMMY_TEXTURE_KEY);
    shaderResourceVariable_Texture.Release();
    vertexBuffer.Release();
    indexBuffer.Release();
}

void PainterView::onCreateState(VertexEngine::TextureManager &textureManager) {

}

void PainterView::onDraw(VertexEngine::Canvas & canvas) {

}

void PainterView::onDestroyState(VertexEngine::TextureManager &textureManager) {

}

void PainterView::onMeasure() {
    setMeasuredDimensions(MeasureSpec::MATCH_PARENT, MeasureSpec::MATCH_PARENT);
}

void PainterView::onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {
}

void PainterView::createPipeline(PipelineManager &pipelineManager) {
    auto & pso = pipelineManager.createPipeline(this, PIPELINE_KEY);
    pso.setType(Diligent::PIPELINE_TYPE_GRAPHICS);
    pso.setNumberOfTargets(1);
    pso.setFormat(getDiligentAppBase().m_pSwapChain.RawPtr());
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
        ShaderCI.Desc.Name       = "PainterView vertex shader";
        ShaderCI.Source          = vertexShader;
        getDiligentAppBase().m_pDevice->CreateShader(ShaderCI, &pVS);
    }

    // Create a pixel shader
    Diligent::IShader * pPS = nullptr;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "PainterView pixel shader";
        ShaderCI.Source          = pixelShader;
        getDiligentAppBase().m_pDevice->CreateShader(ShaderCI, &pPS);
    }
    pso.setShaders(pVS, pPS);

    // Define vertex shader input layout
    pso.setInputLayout(
            {
                    // Attribute 0 - vertex position
                    Diligent::LayoutElement{0, 0, 3, Diligent::VT_FLOAT32, Diligent::False},
                    // Attribute 1 - vertex color
                    Diligent::LayoutElement{1, 0, 4, Diligent::VT_FLOAT32, Diligent::False},
                    // Attribute 3 - Is Texture
                    Diligent::LayoutElement{2, 0, 1, Diligent::VT_FLOAT32, Diligent::False},
                    // Attribute 4 - Texture Coordinates
                    Diligent::LayoutElement{3, 0, 2, Diligent::VT_FLOAT32, Diligent::False}
            }
    );

    // define texture input layout
    pso.setResourceLayoutVariables(
            {
                    {
                            Diligent::SHADER_TYPE_PIXEL,
                            "g_Texture",
                            Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC
                    }
            }
    );

    Diligent::SamplerDesc SamLinearClampDesc
            {
                    Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_LINEAR,
                    Diligent::TEXTURE_ADDRESS_BORDER, Diligent::TEXTURE_ADDRESS_BORDER, Diligent::TEXTURE_ADDRESS_BORDER
            };
    SamLinearClampDesc.BorderColor[0] = 0;
    SamLinearClampDesc.BorderColor[1] = 0;
    SamLinearClampDesc.BorderColor[2] = 0;
    SamLinearClampDesc.BorderColor[3] = 1;
    pso.setResourceLayoutSamplers(
            {
                    {
                            Diligent::SHADER_TYPE_PIXEL,
                            "g_Texture",
                            SamLinearClampDesc
                    }
            }
    );

    pso.createPipelineState(getDiligentAppBase().m_pDevice);
    pso.createShaderBinding(true);

    auto * tex = pso.getVariableFromPixelShader("g_Texture");
    if (tex == nullptr) {
        LOG_ERROR_AND_THROW("failed to acquire variable 'g_Texture'");
    }
    shaderResourceVariable_Texture = tex;
}

void PainterView::switchToPipeline(PipelineManager &pipelineManager) {
    // Set the pipeline state in the immediate context
    pipelineManager.switchToPipeline(
            this, PIPELINE_KEY, getDiligentAppBase().m_pImmediateContext
    );
}

void PainterView::bindShaderResources(PipelineManager &pipelineManager) {
    pipelineManager.commitShaderResourceBinding(
            this, PIPELINE_KEY, getDiligentAppBase().m_pImmediateContext,
            Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
}

void PainterView::destroyPipeline(PipelineManager &pipelineManager) {
    pipelineManager.destroyPipeline(this, PIPELINE_KEY);
}

int PainterView::getCanvasWidth() {
    return 0;
}

int PainterView::getCanvasHeight() {
    return 0;
}
