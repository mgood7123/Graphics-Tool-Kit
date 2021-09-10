//
// Created by Matthew Good on 8/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_RENDERTARGET_H
#define GRAPHICAL_TOOL_KIT_RENDERTARGET_H

#include "DrawTools.h"
#include <Views/Rectangle.h>

/* TODO: texture chunk loader :
    //
    // draw large texture as smaller textures
    //
    auto* pixels = new unsigned char[512*512*4];
    unsigned int width = 512;
    unsigned int height = 512;
    auto * quarter_TL = RenderTarget::createColorTexture(
            256, 256, swapChain, renderDevice
    );
    auto * quarter_TR = RenderTarget::createColorTexture(
            256, 256, swapChain, renderDevice
    );
    auto* pixelsTopLeft = new unsigned char[256*256*4];
    auto* pixelsTopRight = new unsigned char[256*256*4];
    copyPixels(pixels, pixelsTopLeft, 0, 0, 256, 256);
    copyPixels(pixels, pixelsTopRight, 256, 256, 256, 256);
    context->UpdateTexture(
            quarter_TL, 0, 0, {0, 256, 0, 256},
            {pixelsTopLeft, width * 4},
            Diligent::RESOURCE_STATE_TRANSITION_MODE_NONE, Diligent::RESOURCE_STATE_TRANSITION_MODE_NONE
    );
    context->UpdateTexture(
            quarter_TR, 0, 0, {0, 256, 0, 256},
            {pixelsTopRight, width * 4},
            Diligent::RESOURCE_STATE_TRANSITION_MODE_NONE, Diligent::RESOURCE_STATE_TRANSITION_MODE_NONE
    );
 */

class RenderTarget {
    static const char * VS;
    static const char * PS;
    int width = 0;
    int height = 0;
    Diligent::RefCntAutoPtr<Diligent::ITexture> color_texture;
    Diligent::RefCntAutoPtr<Diligent::ITexture> depth_texture;
    Diligent::ITextureView * color_res = nullptr;
    Diligent::ITextureView * colorTV = nullptr;
    bool colorTV_wrapped = false;
    Diligent::ITextureView * depthTV = nullptr;
    bool depthTV_wrapped = false;
    Diligent::RefCntAutoPtr<Diligent::IBuffer> m_PSConstants;
    Diligent::BufferData VBData;
    Diligent::BufferDesc VertBuffDesc;
    Diligent::BufferData IBData;
    Diligent::BufferDesc IndBuffDesc;
    Diligent::RefCntAutoPtr<Diligent::IBuffer> vertexBuffer;
    Diligent::RefCntAutoPtr<Diligent::IBuffer> indexBuffer;
    const char * PIPELINE_KEY = nullptr;
    
public:
    ~RenderTarget();
    int getWidth() const;
    int getHeight() const;
    void create(const char * PIPELINE_KEY, PipelineManager & pipelineManager, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    Diligent::ITextureView * getColor();
    Diligent::ITextureView * getDepth();
    static Diligent::ITexture * createColorTexture(Diligent::Uint32 width, Diligent::Uint32 height, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    static Diligent::ITexture * createColorTexture(Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    static Diligent::ITexture * createDepthTexture(Diligent::Uint32 width, Diligent::Uint32 height, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    static Diligent::ITexture * createDepthTexture(Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    void resize(PipelineManager & pipelineManager, int width, int height, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    void resize(PipelineManager & pipelineManager, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice);
    void wrap(Diligent::ITextureView * color, Diligent::ITextureView * depth);
    void bind(Diligent::IDeviceContext * deviceContext) const;
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
    void clipAbsolutePosition(const Rectangle & position, Diligent::IDeviceContext * deviceContext);
    void clipAbsolutePosition(const float & x, const float & y, const float & w, const float & h, Diligent::IDeviceContext * deviceContext);
    void drawAbsolutePosition(DrawTools & drawTools, const Rectangle & position, Diligent::IDeviceContext * deviceContext);
    void drawAbsolutePosition(DrawTools & drawTools, const int & x, const int & y, const int & w, const int & h, Diligent::IDeviceContext * deviceContext);
    
    void drawAbsolutePositionAndClipToBoundaries(
            DrawTools & drawTools,
            const Rectangle & absolutePosition,
            Diligent::IDeviceContext * deviceContext
    );
    
    void drawToRenderTarget(
           DrawTools &drawTools,
           const RenderTarget &renderTarget,
           Diligent::IDeviceContext *deviceContext
    );

    void destroy(PipelineManager & pipelineManager);

    static float black[4];

    bool exists();
};

#endif //GRAPHICAL_TOOL_KIT_RENDERTARGET_H
