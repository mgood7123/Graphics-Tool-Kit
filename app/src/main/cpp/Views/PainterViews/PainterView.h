//
// Created by Matthew Good on 14/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_PAINTERVIEW_H
#define GRAPHICAL_TOOL_KIT_PAINTERVIEW_H

#include <Views/View.h>

/**
 * an empty Painter View, this by itself does nothing
 */
class PainterView : public View {
private:
    VertexEngine vertexEngine;
    int chunkSize = 3;
    const char * DUMMY_TEXTURE_KEY = "DEFAULT_WHEN_NO_TEXTURE";

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
    void draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override final;
    void destroy() override final;

    virtual int getCanvasWidth();
    virtual int getCanvasHeight();
    virtual void onCreate(VertexEngine::TextureManager & textureManager);
    virtual void onDraw(VertexEngine::Canvas & canvas);
    virtual void onDestroy(VertexEngine::TextureManager & textureManager);

    static constexpr const char * PIPELINE_KEY = "PainterView";
};

#endif //GRAPHICAL_TOOL_KIT_PAINTER_H
