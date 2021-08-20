//
// Created by Matthew Good on 1/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_CUBE_H
#define GRAPHICAL_TOOL_KIT_CUBE_H

#include <DiligentCore/Common/interface/BasicMath.hpp>
#include <DiligentCore/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/Buffer.h>
#include "View.h"

class CubeView : public View
{
public:
    static const char * cube_VS;
    static const char * cube_PS;

    void createPipeline(PipelineManager & pipelineManager) override;
    void switchToPipeline(PipelineManager & pipelineManager) override;
    void bindShaderResources(PipelineManager & pipelineManager) override;
    void destroyPipeline(PipelineManager & pipelineManager) override;

    void create() override;
    void resize(PipelineManager & pipelineManager) override;
    bool hasPhysics() override;
    void physics(const TimeEngine & timeEngine) override;
    void draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;
    void destroy() override;

    void CreateVertexBuffer();
    void CreateIndexBuffer();
    
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_CubeVertexBuffer;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_CubeIndexBuffer;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_VSConstants;
    Diligent::float4x4                                        m_WorldViewProjMatrix;
    double                                                    obj_y;
    Diligent::float4x4                                        CubeModelTransform;

    static constexpr const char *PIPELINE_KEY = "CubeView";

    void computeViewModel();
};


#endif //GRAPHICAL_TOOL_KIT_CUBE_H
