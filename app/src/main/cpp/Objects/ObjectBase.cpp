//
// Created by Matthew Good on 30/6/21.
//

#include "ObjectBase.h"

void ObjectBase::create ()
{}

void ObjectBase::draw ()
{}

void ObjectBase::drawChunks(VertexEngine::GenerationInfo &info, size_t chunkSize)
{}

void ObjectBase::destroy ()
{}

void ObjectBase::physics (const TimeEngine & timeEngine)
{}

double ObjectBase::degreesToRadians(double y)
{
    return y * (M_PI/180);
}
