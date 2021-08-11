//
// Created by Matthew Good on 14/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_PAINTER_H
#define GRAPHICAL_TOOL_KIT_PAINTER_H

#include <Objects/ObjectBase.h>

/**
 * an empty Painter, this does nothing by itself
 */
class Painter : public ObjectBase {
private:
    VertexEngine vertexEngine;
    int chunkSize = 3;
    const char * DUMMY_TEXTURE_KEY = "DEFAULT_WHEN_NO_TEXTURE";

    int canvas_width = 400;
    int canvas_height = 400;

    void drawChunks(VertexEngine::GenerationInfo && info, DrawTools & tools);


    static const char * vertexShader;
    static const char * pixelShader;
    Diligent::BufferData VBData;
    Diligent::BufferDesc VertBuffDesc;
    Diligent::BufferData IBData;
    Diligent::BufferDesc IndBuffDesc;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                vertexBuffer;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                indexBuffer;

    std::array<float, 4> dummyTextureColorRef;
    Diligent::ITextureView * dummyTextureView;

protected:
    Diligent::RefCntAutoPtr<Diligent::IShaderResourceVariable> shaderResourceVariable_Texture;

public:

    void createPipeline(PipelineManager &pipelineManager) override final;

    void switchToPipeline(PipelineManager &pipelineManager) override final;

    void bindShaderResources(PipelineManager &pipelineManager) override final;

    void destroyPipeline(PipelineManager &pipelineManager) override final;

    void create() override final;
    void draw(DrawTools & drawTools, RenderTarget & renderTarget) override final;
    void destroy() override final;
    virtual void onCreate(VertexEngine::TextureManager & textureManager);
    virtual void onDraw(VertexEngine::Canvas & canvas);
    virtual void onDestroy(VertexEngine::TextureManager & textureManager);

    static constexpr const char * PIPELINE_KEY = "View";
};

#endif //GRAPHICAL_TOOL_KIT_PAINTER_H
