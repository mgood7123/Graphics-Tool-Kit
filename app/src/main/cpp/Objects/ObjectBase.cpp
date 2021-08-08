//
// Created by Matthew Good on 30/6/21.
//

#include "ObjectBase.h"

void ObjectBase::setDiligentAppBase(DiligentAppBase *dab) {
    diligentAppBase = dab;
}

DiligentAppBase &ObjectBase::getDiligentAppBase() {
    return *diligentAppBase;
}

void ObjectBase::createPipeline(PipelineManager & pipelineManager) {}

void ObjectBase::switchToPipeline(PipelineManager & pipelineManager) {}

void ObjectBase::bindShaderResources(PipelineManager & pipelineManager) {}

void ObjectBase::destroyPipeline(PipelineManager & pipelineManager) {}

void ObjectBase::create ()
{}

void ObjectBase::resize(PipelineManager & pipelineManager)
{}

void ObjectBase::draw (DrawTools & drawTools, RenderTarget & renderTarget)
{}

bool ObjectBase::onTouchEvent(MultiTouch &multiTouch) {
    return false;
}

void ObjectBase::destroy ()
{}

bool ObjectBase::hasPhysics()
{
    return false;
}

void ObjectBase::physics (const TimeEngine & timeEngine)
{}

double ObjectBase::degreesToRadians(double y)
{
    return y * (M_PI/180);
}
