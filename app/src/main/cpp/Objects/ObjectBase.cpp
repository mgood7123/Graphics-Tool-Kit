//
// Created by Matthew Good on 30/6/21.
//

#include "ObjectBase.h"

const Position ObjectBase::ZERO_POSITION = {0, 0, Position::MATCH_PARENT, Position::MATCH_PARENT};
const Position ObjectBase::NO_CROPPING = {0, 0, 0, 0};
const Position ObjectBase::NO_PADDING = {0, 0, 0, 0};
const Position ObjectBase::NO_MARGIN = {0, 0, 0, 0};

void ObjectBase::setTag(const char *name) {
    tag = name;
}

const char * ObjectBase::getTag() {
    return tag;
}

void ObjectBase::setDiligentAppBase(DiligentAppBase * diligentAppBase) {
    this->diligentAppBase = diligentAppBase;
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
