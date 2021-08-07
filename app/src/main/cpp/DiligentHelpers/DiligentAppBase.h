//
// Created by Matthew Good on 29/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_DILIGENTAPPBASE_H
#define GRAPHICAL_TOOL_KIT_DILIGENTAPPBASE_H

#include <DiligentCore/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/EngineFactory.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/SwapChain.h>
#include <DiligentCore/Graphics/GraphicsTools/interface/ScreenCapture.hpp>
#include <MultiTouch/DiligentLog/Primitives/interface/Errors.hpp>
#include <DiligentTools/TextureLoader/interface/Image.h>
#include <DiligentTools/Imgui/interface/ImGuiImplDiligent.hpp>
#include <DiligentCore/Common/interface/Timer.hpp>
#include <TimeEngine/TimeEngine.h>
#include <VertexEngine/VertexEngine.h>
#include "PipelineManager.h"

class DiligentAppBase
{
    static constexpr const char * PIPELINE_RT_KEY = "DiligentAppBase RT";
    static const char * VS_RT;
    static const char * PS_RT;
    Diligent::RefCntAutoPtr<Diligent::ITexture> color_texture;
    Diligent::RefCntAutoPtr<Diligent::ITexture> depth_texture;
    Diligent::ITextureView * color_res = nullptr;
    Diligent::ITextureView * colorTV_RT = nullptr;
    Diligent::ITextureView * depthTV_RT = nullptr;
    Diligent::RefCntAutoPtr<Diligent::IBuffer> m_PSConstants_RT;
    Diligent::BufferData VBData_RT;
    Diligent::BufferDesc VertBuffDesc_RT;
    Diligent::BufferData IBData_RT;
    Diligent::BufferDesc IndBuffDesc_RT;
    Diligent::RefCntAutoPtr<Diligent::IBuffer> vertexBuffer_RT;
    Diligent::RefCntAutoPtr<Diligent::IBuffer> indexBuffer_RT;
    
public:
    Diligent::SwapChainDesc                                        m_pSCDesc;
    Diligent::RENDER_DEVICE_TYPE                                   m_DeviceType;
    Diligent::RefCntAutoPtr<Diligent::IEngineFactory>              m_pEngineFactory;
    Diligent::RefCntAutoPtr<Diligent::IRenderDevice>               m_pDevice;
    Diligent::RefCntAutoPtr<Diligent::IDeviceContext>              m_pImmediateContext;
    std::vector<Diligent::RefCntAutoPtr<Diligent::IDeviceContext>> m_pDeferredContexts;
    Diligent::RefCntAutoPtr<Diligent::ISwapChain>                  m_pSwapChain;
    Diligent::ImGuiImplDiligent *                                  m_pImGui = nullptr;
    PipelineManager pipelineManager;
    
    DiligentAppBase ();
    ~DiligentAppBase ();
    
    void attachToContext (int32_t w, int32_t h);
    Diligent::ITexture * createColorTexture();
    Diligent::ITexture * createDepthTexture();
    
    static float black[4];
    void createRT_OffScreen(PipelineManager & pipelineManager);
    void resizeRT_OffScreen(PipelineManager & pipelineManager);
    void bindRT_Screen();
    void clearColorRT_Screen(const float * color);
    void clearColorRT_Screen(const VertexEngine::Color4 & color);
    void clearDepthRT_Screen(float depth);
    void clearColorAndDepthRT_Screen(const float * color, float depth);
    void clearColorAndDepthRT_Screen(const VertexEngine::Color4 & color, float depth);
    void draw_OffScreen_RT_To_Screen_RT(
                PipelineManager & pipelineManager, VertexEngine & engine,
                int x, int y, int w, int h
    );
    void destroyRT_OffScreen(PipelineManager & pipelineManager);
        
    Diligent::ITextureView * getColorRT_OffScreen();
    Diligent::ITextureView * getDepthRT_OffScreen();
};


#endif //GRAPHICAL_TOOL_KIT_DILIGENTAPPBASE_H
