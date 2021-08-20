//
// Created by Matthew Good on 29/6/21.
//

#include "DiligentAppBase.h"
#include <EngineFactoryOpenGL.h>

DiligentAppBase::DiligentAppBase ()
{
    m_DeviceType = Diligent::RENDER_DEVICE_TYPE_GL;
    swapChainWidth = 0;
    swapChainHeight = 0;
}

DiligentAppBase::~DiligentAppBase ()
{
}

void DiligentAppBase::attachToContext (int32_t w, int32_t h)
{
    Diligent::EngineGLCreateInfo EngineCI;
    
    m_pEngineFactory = Diligent::GetEngineFactoryOpenGL();
    m_pEngineFactory.Cast<Diligent::IEngineFactoryOpenGL>(Diligent::IID_EngineFactoryOpenGL)
            ->CreateSwapChainGLAndAttachToActiveGLContext(
                    EngineCI, &m_pDevice, &m_pImmediateContext,
                    m_pSwapChainCreationDescription, &m_pSwapChain, w, h
            );
}
