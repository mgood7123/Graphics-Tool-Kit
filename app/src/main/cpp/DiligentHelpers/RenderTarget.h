//
// Created by Matthew Good on 8/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_RENDERTARGET_H
#define GRAPHICAL_TOOL_KIT_RENDERTARGET_H

#include "DrawTools.h"

class RenderTarget {
    static const char * VS;
    static const char * PS;
    Diligent::RefCntAutoPtr<Diligent::ITexture> color_texture;
    Diligent::RefCntAutoPtr<Diligent::ITexture> depth_texture;
    Diligent::ITextureView * color_res = nullptr;
    Diligent::ITextureView * colorTV = nullptr;
    Diligent::ITextureView * depthTV = nullptr;
    Diligent::RefCntAutoPtr<Diligent::IBuffer> m_PSConstants;
    Diligent::BufferData VBData;
    Diligent::BufferDesc VertBuffDesc;
    Diligent::BufferData IBData;
    Diligent::BufferDesc IndBuffDesc;
    Diligent::RefCntAutoPtr<Diligent::IBuffer> vertexBuffer;
    Diligent::RefCntAutoPtr<Diligent::IBuffer> indexBuffer;
    const char * PIPELINE_KEY;
public:
    void create(const char * PIPELINE_KEY, PipelineManager & pipelineManager, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    Diligent::ITextureView * getColor();
    Diligent::ITextureView * getDepth();
    static Diligent::ITexture * createColorTexture(Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    static Diligent::ITexture * createDepthTexture(Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    void resize(PipelineManager & pipelineManager, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    void bind(Diligent::IDeviceContext * deviceContext);
    static void bind(Diligent::ITextureView * color, Diligent::ITextureView * depth, Diligent::IDeviceContext * deviceContext);
    void clearColor(const float * color, Diligent::IDeviceContext * deviceContext);
    static void clearColor(const float * color, Diligent::ITextureView * colorTV, Diligent::IDeviceContext * deviceContext);
    void clearColor(const VertexEngine::Color4 & color, Diligent::IDeviceContext * deviceContext);
    static void clearColor(const VertexEngine::Color4 & color, Diligent::ITextureView * colorTV, Diligent::IDeviceContext * deviceContext);
    void clearDepth(float depth, Diligent::IDeviceContext * deviceContext);
    static void clearDepth(float depth, Diligent::ITextureView * depthTV, Diligent::IDeviceContext * deviceContext);
    void clearColorAndDepth(const float * color, float depth, Diligent::IDeviceContext * deviceContext);
    static void clearColorAndDepth(const float * color, float depth, Diligent::ITextureView * colorTV, Diligent::ITextureView * depthTV, Diligent::IDeviceContext * deviceContext);
    void clearColorAndDepth(const VertexEngine::Color4 & color, float depth, Diligent::IDeviceContext * deviceContext);
    static void clearColorAndDepth(const VertexEngine::Color4 & color, float depth, Diligent::ITextureView * colorTV, Diligent::ITextureView * depthTV, Diligent::IDeviceContext * deviceContext);
    void draw(DrawTools & drawTools, int x, int y, int w, int h, Diligent::IDeviceContext * deviceContext);
    void destroy(PipelineManager & pipelineManager);

    static float black[4];
};

#endif //GRAPHICAL_TOOL_KIT_RENDERTARGET_H
