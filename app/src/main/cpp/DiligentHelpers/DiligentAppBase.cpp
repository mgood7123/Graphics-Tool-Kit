//
// Created by Matthew Good on 29/6/21.
//

#include "DiligentAppBase.h"
#include <EngineFactoryOpenGL.h>

DiligentAppBase::DiligentAppBase ()
{
    m_DeviceType = Diligent::RENDER_DEVICE_TYPE_GL;
}

DiligentAppBase::~DiligentAppBase ()
{
}

void DiligentAppBase::attachToContext (int32_t w, int32_t h)
{
    Diligent::EngineGLCreateInfo EngineCI;
    
    Diligent::GetEngineFactoryOpenGL()
            ->CreateSwapChainGLAndAttachToActiveGLContext(
                    EngineCI, &m_pDevice, &m_pImmediateContext,
                    m_pSCDesc, &m_pSwapChain, w, h
            );
}