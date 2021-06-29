//
// Created by Matthew Good on 29/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_DILIGENTAPPBASE_H
#define GRAPHICAL_TOOL_KIT_DILIGENTAPPBASE_H


#include <RenderDevice.h>
#include <RefCntAutoPtr.hpp>
#include <SwapChain.h>
#include "../DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL/include/pch.h" // EGL
#include "../DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL/include/RenderDeviceGLImpl.hpp"

class DiligentAppBase
{
public:
    Diligent::SwapChainDesc                           m_pSCDesc;
    Diligent::RefCntAutoPtr<Diligent::IRenderDevice>  m_pDevice;
    Diligent::RefCntAutoPtr<Diligent::IDeviceContext> m_pImmediateContext;
    Diligent::RefCntAutoPtr<Diligent::ISwapChain>     m_pSwapChain;
    Diligent::RefCntAutoPtr<Diligent::IPipelineState> m_pPSO;
    Diligent::RENDER_DEVICE_TYPE                      m_DeviceType;

    DiligentAppBase ();
    ~DiligentAppBase ();
    
    void attachToContext (int32_t w, int32_t h);
};


#endif //GRAPHICAL_TOOL_KIT_DILIGENTAPPBASE_H
