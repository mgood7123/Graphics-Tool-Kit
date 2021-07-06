//
// Created by Matthew Good on 1/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_CUBE_H
#define GRAPHICAL_TOOL_KIT_CUBE_H

#include <DiligentCore/Common/interface/BasicMath.hpp>
#include <DiligentCore/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/Buffer.h>
#include "ObjectBase.h"

class Cube : public ObjectBase
{
public:
    static const char * cube_VS;
    static const char * cube_PS;
    void create() override;
    void draw() override;
    
    void CreatePipelineState();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    
    Diligent::RefCntAutoPtr<Diligent::IPipelineState>         m_pPSO;
    Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> m_pSRB;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_CubeVertexBuffer;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_CubeIndexBuffer;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_VSConstants;
    Diligent::float4x4                                        m_WorldViewProjMatrix;
};


#endif //GRAPHICAL_TOOL_KIT_CUBE_H
