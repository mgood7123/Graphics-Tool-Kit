//
// Created by Matthew Good on 8/8/21.
//

#include "CubeOnTriangle.h"

void CubeOnTriangle::setDiligentAppBase(DiligentAppBase *diligentAppBase) {
    ObjectBase::setDiligentAppBase(diligentAppBase);
    triangle.setDiligentAppBase(diligentAppBase);
    cube.setDiligentAppBase(diligentAppBase);
}

void CubeOnTriangle::createPipeline(PipelineManager &pipelineManager) {
    triangle.createPipeline(pipelineManager);
    cube.createPipeline(pipelineManager);
    auto & app = getDiligentAppBase();
    rt.create(PIPELINE_KEY, pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void CubeOnTriangle::destroyPipeline(PipelineManager &pipelineManager) {
    triangle.destroyPipeline(pipelineManager);
    cube.destroyPipeline(pipelineManager);
    rt.destroy(pipelineManager);
}

void CubeOnTriangle::create() {
    width = MATCH_PARENT;
    height = MATCH_PARENT;
    triangle.create();
    cube.create();
}

void CubeOnTriangle::resize(PipelineManager &pipelineManager) {
    triangle.resize(pipelineManager);
    cube.resize(pipelineManager);
    auto & app = getDiligentAppBase();
    rt.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void CubeOnTriangle::draw(DrawTools & drawTools, RenderTarget & renderTarget) {
    auto & app = getDiligentAppBase();

    // draw triangle to render target `rt`
    triangle.switchToPipeline(drawTools.pipelineManager);
    triangle.bindShaderResources(drawTools.pipelineManager);
    triangle.draw(drawTools, rt);
    
    // draw rt to render target
    renderTarget.bind(app.m_pImmediateContext);
    renderTarget.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
    drawTools.pixelToNDC.resize(400, 400);
    rt.draw(drawTools, 0, 0, 200, 200, app.m_pImmediateContext);

    // draw cube to render target `rt`
    cube.switchToPipeline(drawTools.pipelineManager);
    cube.bindShaderResources(drawTools.pipelineManager);
    cube.draw(drawTools, rt);

    // draw rt to render target
    renderTarget.bind(app.m_pImmediateContext);
    // dont clear, use existing contents
    drawTools.pixelToNDC.resize(400, 400);
    rt.draw(drawTools, 200, 200, 200, 200, app.m_pImmediateContext);
}

bool CubeOnTriangle::onTouchEvent(MultiTouch &touch) {
    return ObjectBase::onTouchEvent(touch);
}

void CubeOnTriangle::destroy() {
    triangle.destroy();
    cube.destroy();
}

bool CubeOnTriangle::hasPhysics() {
    return triangle.hasPhysics()
    || cube.hasPhysics()
    ;
}

void CubeOnTriangle::physics(const TimeEngine &timeEngine) {
    triangle.physics(timeEngine);
    cube.physics(timeEngine);
}
