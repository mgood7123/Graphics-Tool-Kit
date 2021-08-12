//
// Created by Matthew Good on 8/8/21.
//

#include "CubeOnTriangle.h"

void CubeOnTriangle::createPipeline(PipelineManager &pipelineManager) {
    ObjectGroup::createPipeline(pipelineManager);
    auto & app = getDiligentAppBase();
    rt.create(PIPELINE_KEY, pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.create(PIPELINE_KEY2, pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void CubeOnTriangle::destroyPipeline(PipelineManager &pipelineManager) {
    ObjectGroup::destroyPipeline(pipelineManager);
    rt.destroy(pipelineManager);
    rt2.destroy(pipelineManager);
}

void CubeOnTriangle::create() {
    ObjectGroup::create();

    // we know how large we will be drawing our objects
//    auto tri = new Triangle();
//    tri->padding = 20;
//    tri->position.width = 80;
//    addChild(tri);
//
//    auto cube = new Cube();
//    cube->cropping = 23;
//    cube->padding = 40;
//    addChild(cube);

//    auto painter = new RectanglePainter();
//    painter->padding = 60;
//    addChild(new RectanglePainter());
//    addChild(new RectanglePainter());
    addChild(new RectanglePainter());
}

void CubeOnTriangle::resize(PipelineManager &pipelineManager) {
    ObjectGroup::resize(pipelineManager);
    auto & app = getDiligentAppBase();
    rt.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void CubeOnTriangle::draw(DrawTools & drawTools, RenderTarget & renderTarget) {
    auto & app = getDiligentAppBase();

    auto array = getChildren();
    bool first = true;
    size_t count = 0;
    for (ChildInfo *childInfo : array) {
        ObjectBase * obj = childInfo->object;
        
        // if obj == this
        //     draw render target to temporary render target
        //     and then draw temporary render target to render target
        //     this avoids artifacts that might occur
        //     then the render target is drawing to itself
        // otherwise
        //     draw the object to rt and then draw rt to render target
        if (obj != this) {
            // draw object to render target `rt`
            obj->switchToPipeline(drawTools.pipelineManager);
            obj->bindShaderResources(drawTools.pipelineManager);
            obj->draw(drawTools, rt);
        }

        // draw rt to render target
        renderTarget.bind(app.m_pImmediateContext);
        
        // clear on first draw
        if (first) {
            renderTarget.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
            first = false;
        }
        
        int full = 400;
        int half = full/2;

        // draw as a 4x4 grid
        drawTools.pixelToNDC.resize(full, full);

        LOOP:
        switch (count) {
            case 0:
                if (obj->position.x < half && obj->position.y < half) {
                    if (obj == this) {
                        rt2.bind(app.m_pImmediateContext);
                        rt2.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
                        renderTarget.draw(drawTools, 0, 0, full, full, app.m_pImmediateContext);
                        renderTarget.bind(app.m_pImmediateContext);
                    }
                    drawAndClipToBoundaries(
                            childInfo, obj == this ? rt2 : rt, drawTools,
                            {0, 0, half, half},
                            app.m_pImmediateContext
                    );
                }
                break;
            case 1:
                if (obj->position.x < half && obj->position.y < half) {
                    if (obj == this) {
                        rt2.bind(app.m_pImmediateContext);
                        rt2.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
                        renderTarget.draw(drawTools, 0, 0, full, full, app.m_pImmediateContext);
                        renderTarget.bind(app.m_pImmediateContext);
                    }
                    drawAndClipToBoundaries(
                            childInfo, obj == this ? rt2 : rt, drawTools,
                            {half, 0, half, half},
                            app.m_pImmediateContext
                    );
                }
                break;
            case 2:
                if (obj->position.x < half && obj->position.y < half) {
                    if (obj == this) {
                        rt2.bind(app.m_pImmediateContext);
                        rt2.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
                        renderTarget.draw(drawTools, 0, 0, full, full, app.m_pImmediateContext);
                        renderTarget.bind(app.m_pImmediateContext);
                    }
                    drawAndClipToBoundaries(
                            childInfo, obj == this ? rt2 : rt, drawTools,
                            {0, half, half, half},
                            app.m_pImmediateContext
                    );
                }
                break;
            case 3:
                if (obj->position.x < half && obj->position.y < half) {
                    if (obj == this) {
                        rt2.bind(app.m_pImmediateContext);
                        rt2.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
                        renderTarget.draw(drawTools, 0, 0, full, full, app.m_pImmediateContext);
                        renderTarget.bind(app.m_pImmediateContext);
                    }

                    drawAndClipToBoundaries(
                            childInfo, obj == this ? rt2 : rt, drawTools,
                            {half, half, half, half},
                            app.m_pImmediateContext
                    );
                }
                break;
            default:
                // wrap around
                count = 0;
                goto LOOP;
        }
        count++;
    }
}

bool CubeOnTriangle::onTouchEvent(MultiTouch &touch) {
    return ObjectGroup::onTouchEvent(touch);
}

void CubeOnTriangle::destroy() {
    ObjectGroup::destroy();
}
