//
// Created by Matthew Good on 14/7/21.
//

#include "View.h"

const char * View::vertexShader = R"(
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

const char * View::pixelShader = R"(
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

void View::create() {
    VertBuffDesc.Name = "Rectangle vertex buffer";
    VertBuffDesc.Usage = Diligent::USAGE_DEFAULT;
    VertBuffDesc.BindFlags = Diligent::BIND_VERTEX_BUFFER;
    VertBuffDesc.uiSizeInBytes = sizeof(float)*(VertexEngine::strideLength*chunkSize);

    IndBuffDesc.Name = "Rectangle index buffer";
    IndBuffDesc.Usage = Diligent::USAGE_DEFAULT;
    IndBuffDesc.BindFlags = Diligent::BIND_INDEX_BUFFER;
    IndBuffDesc.uiSizeInBytes = sizeof(uint32_t)*chunkSize;

    diligentAppBase->m_pDevice->CreateBuffer(VertBuffDesc, &VBData, &vertexBuffer);
    diligentAppBase->m_pDevice->CreateBuffer(IndBuffDesc, &IBData, &indexBuffer);

    vertexEngine.resize(canvas_width, canvas_height);

    vertexEngine.textureManager.setDefaultDevices(
            diligentAppBase->m_pDevice.RawPtr(),
            diligentAppBase->m_pImmediateContext.RawPtr()
    );

    vertexEngine.textureManager.createSolidColorTexture(DUMMY_TEXTURE_KEY, {0,0,0,1});
    auto c = vertexEngine.textureManager.getTexture(DUMMY_TEXTURE_KEY)->third;
    dummyTextureColorRef = c.to_RGBA_array();
    dummyTextureView = c.texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);

    onCreate(vertexEngine.textureManager);
}

void View::draw (PipelineManager & pipelineManager) {
    // Clear the back buffer
    const float ClearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
    // Let the engine perform required state transitions

    auto* pRTV = diligentAppBase->getColorRT_OffScreen();
    auto* pDSV = diligentAppBase->getDepthRT_OffScreen();
    diligentAppBase->m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    diligentAppBase->m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    diligentAppBase->m_pImmediateContext->ClearDepthStencil(pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Bind the vertex and index buffers
    Diligent::Uint32   offset   = 0;
    Diligent::IBuffer* pBuffs[] = {vertexBuffer};
    diligentAppBase->m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
    diligentAppBase->m_pImmediateContext->SetIndexBuffer(indexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    vertexEngine.clear();
    VertexEngine::Canvas canvas(&vertexEngine, 400, 400);
    onDraw(canvas);

    VertexEngine::GenerationInfo generationInfo = vertexEngine.generateGL();

    drawChunks(generationInfo);
    diligentAppBase->bindRT_Screen();
    diligentAppBase->clearColorAndDepthRT_Screen({0,0,0,1}, 1);
    diligentAppBase->draw_OffScreen_RT_To_Screen_RT(pipelineManager, vertexEngine, 0, 0, vertexEngine.getWidth(), vertexEngine.getHeight());
}

void View::drawChunks(VertexEngine::GenerationInfo &info) {
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
                auto tex = vertexEngine.textureManager.getTexture(textureIndex);
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

        bindShaderResources(diligentAppBase->pipelineManager);

        diligentAppBase->m_pImmediateContext->UpdateBuffer(
                vertexBuffer,
                0, chunk.sizeInBytesData,
                chunk.data,
                Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
        );

        diligentAppBase->m_pImmediateContext->UpdateBuffer(
                indexBuffer,
                0, chunk.sizeInBytesIndices,
                chunk.indices,
                Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
        );

        DrawAttrs.NumIndices = chunk.lengthIndices;

        diligentAppBase->m_pImmediateContext->DrawIndexed(DrawAttrs);
    }
    // LOG_INFO_MESSAGE("drawn ", info.chunksGenerated, " chunk", info.chunksGenerated == 1 ? "" : "s");
}

void View::destroy() {
    onDestroy(vertexEngine.textureManager);
    vertexEngine.textureManager.deleteTexture(DUMMY_TEXTURE_KEY);
    shaderResourceVariable_Texture.Release();
    vertexBuffer.Release();
    indexBuffer.Release();
}

void View::onCreate(VertexEngine::TextureManager &textureManager) {

}

void View::onDraw(VertexEngine::Canvas & canvas) {

}

void View::onDestroy(VertexEngine::TextureManager &textureManager) {

}

void View::createPipeline(PipelineManager &pipelineManager) {
    auto & pso = pipelineManager.createPipeline(PIPELINE_KEY);
    pso.setType(Diligent::PIPELINE_TYPE_GRAPHICS);
    pso.setNumberOfTargets(1);
    pso.setFormat(diligentAppBase->m_pSwapChain.RawPtr());
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
        ShaderCI.Desc.Name       = "View vertex shader";
        ShaderCI.Source          = vertexShader;
        diligentAppBase->m_pDevice->CreateShader(ShaderCI, &pVS);
    }

    // Create a pixel shader
    Diligent::IShader * pPS = nullptr;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "View pixel shader";
        ShaderCI.Source          = pixelShader;
        diligentAppBase->m_pDevice->CreateShader(ShaderCI, &pPS);
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

    pso.createPipelineState(diligentAppBase->m_pDevice);
    pso.createShaderBinding(true);

    auto * tex = pso.getVariableFromPixelShader("g_Texture");
    if (tex == nullptr) {
        LOG_ERROR_AND_THROW("failed to acquire variable 'g_Texture'");
    }
    shaderResourceVariable_Texture = tex;
}

void View::switchToPipeline(PipelineManager &pipelineManager) {
    // Set the pipeline state in the immediate context
    pipelineManager.switchToPipeline(
            PIPELINE_KEY, diligentAppBase->m_pImmediateContext
    );
}

void View::bindShaderResources(PipelineManager &pipelineManager) {
    pipelineManager.commitShaderResourceBinding(
            PIPELINE_KEY, diligentAppBase->m_pImmediateContext,
            Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
}

void View::destroyPipeline(PipelineManager &pipelineManager) {
    pipelineManager.destroyPipeline(PIPELINE_KEY);
}
