//
// Created by Matthew Good on 8/8/21.
//

#include "GridView.h"

void GridView::onCreatePipeline(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.create(PIPELINE_KEY, pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.create(PIPELINE_KEY2, pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void GridView::onDestroyPipeline(PipelineManager &pipelineManager) {
    rt.destroy(pipelineManager);
    rt2.destroy(pipelineManager);
}

void GridView::onCreate() {
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
//    painter->padding = 20;
//    addChild(painter);
//    painter = new RectanglePainter();
//    painter->padding = 40;
//    addChild(painter);
//    painter = new RectanglePainter();
//    painter->padding = 60;
//    addChild(painter);
//    painter = new RectanglePainter();
//    painter->padding = 80;
//    addChild(painter);
}

void GridView::onResize(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void GridView::draw(DrawTools & drawTools, RenderTarget & screenRenderTarget, RenderTarget & renderTarget) {
    auto & app = getDiligentAppBase();

    rt.bind(app.m_pImmediateContext);
    rt.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);

    auto array = getChildren();
    size_t count = 0;
    for (View *view : array) {
        // IMPORTANT: compute draw position before actually drawing anything!

        int full_w = 600;
        int full_h = 600;
        int half_w = full_w/2;
        int half_h = full_h/2;
        
        Rectangle drawPosition;

        LOOP:
        switch (count) {
            case 0:
                if (view->position.topLeft.x < half_w && view->position.topLeft.y < half_h) {
                    drawPosition = {0, 0, half_w, half_h};
                }
                break;
            case 1:
                if (view->position.topLeft.x < half_w && view->position.topLeft.y < half_h) {
                    drawPosition = {half_w, 0, half_w, half_h};
                }
                break;
            case 2:
                if (view->position.topLeft.x < half_w && view->position.topLeft.y < half_h) {
                    drawPosition = {0, half_h, half_w, half_h};
                }
                break;
            case 3:
                if (view->position.topLeft.x < half_w && view->position.topLeft.y < half_h) {
                    drawPosition = {half_w, half_h, half_w, half_h};
                }
                break;
            default:
                // wrap around
                count = 0;
                goto LOOP;
        }
        
        // if obj == this
        //     draw render target to temporary render target
        //     and then draw temporary render target to render target
        //     this avoids artifacts that might occur
        //     when the render target is drawing to itself
        // otherwise
        //     draw the object to rt and then draw rt to render target

        drawTools.pixelToNDC.resize(full_w, full_h);

        if (view == this) {
            rt2.bind(app.m_pImmediateContext);
            rt2.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
            renderTarget.drawAbsolutePosition(drawTools, {0, 0, renderTarget.getWidth(), renderTarget.getHeight()}, app.m_pImmediateContext);
        } else {
            // draw object to render target `rt`
            rt2.bind(app.m_pImmediateContext);
            rt2.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
            view->switchToPipeline(drawTools.pipelineManager);
            view->bindShaderResources(drawTools.pipelineManager);
            view->buildCoordinates(drawPosition, drawTools, renderTarget);
            view->draw(drawTools, screenRenderTarget, rt2);
        }

        // children have likely resized the pixel grid, restore it
        drawTools.pixelToNDC.resize(full_w, full_h);
        
        rt.bind(app.m_pImmediateContext);
        rt2.drawAbsolutePositionAndClipToBoundaries(
            drawTools, view->getAbsolutePosition(), app.m_pImmediateContext
        );
        count++;
    }
    
    rt.drawToRenderTarget(drawTools, renderTarget, app.m_pImmediateContext);
}

void GridView::drawRenderTarget(DrawTools &tools, Diligent::IDeviceContext *deviceContext) {
    Rectangle x;
    x = {0, 0, rt.getWidth(), rt.getHeight()};
    rt.drawAbsolutePositionAndClipToBoundaries(tools, x, deviceContext);
}


bool GridView::onTouchEvent(MultiTouch &touch) {
    return ViewGroup::onTouchEvent(touch);
}

void GridView::onDestroy() {
}
