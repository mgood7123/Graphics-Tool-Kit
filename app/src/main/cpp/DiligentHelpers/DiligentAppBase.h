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
#include <DiligentLog/Primitives/interface/Errors.hpp>
#include <DiligentTools/TextureLoader/interface/Image.h>
#include <DiligentTools/Imgui/interface/ImGuiImplDiligent.hpp>
#include <DiligentCore/Common/interface/Timer.hpp>
#include <TimeEngine.h>

class DiligentAppBase
{
public:
    Diligent::SwapChainDesc                                        m_pSCDesc;
    Diligent::RefCntAutoPtr<Diligent::IPipelineState>              m_pPSO;
    Diligent::RENDER_DEVICE_TYPE                                   m_DeviceType;
    Diligent::RefCntAutoPtr<Diligent::IEngineFactory>              m_pEngineFactory;
    Diligent::RefCntAutoPtr<Diligent::IRenderDevice>               m_pDevice;
    Diligent::RefCntAutoPtr<Diligent::IDeviceContext>              m_pImmediateContext;
    std::vector<Diligent::RefCntAutoPtr<Diligent::IDeviceContext>> m_pDeferredContexts;
    Diligent::RefCntAutoPtr<Diligent::ISwapChain>                  m_pSwapChain;
    Diligent::ImGuiImplDiligent *                                  m_pImGui = nullptr;

    float  m_fSmoothFPS                   = 0;
    double m_LastFPSTime                  = 0;
    Diligent::Uint32 m_NumFramesRendered  = 0;
    Diligent::Uint32 m_CurrentFrameNumber = 0;
    
    DiligentAppBase ();
    ~DiligentAppBase ();
    
    void attachToContext (int32_t w, int32_t h);
};


#endif //GRAPHICAL_TOOL_KIT_DILIGENTAPPBASE_H
