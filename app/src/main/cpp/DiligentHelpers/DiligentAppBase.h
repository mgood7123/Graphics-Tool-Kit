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
#include <DiligentCore/Primitives/interface/Errors.hpp>
#include <DiligentTools/TextureLoader/interface/Image.h>

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
