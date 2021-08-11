//
// Created by Matthew Good on 9/8/21.
//

#include "ObjectGroup.h"

void ObjectGroup::setDiligentAppBase(DiligentAppBase *diligentAppBase) {
    ObjectBase::setDiligentAppBase(diligentAppBase);
    auto array = getChildren();
    for (ObjectBase *obj : array) {
        if (obj == this) continue; // skip self
        obj->setDiligentAppBase(diligentAppBase);
    }
    setDiligentAppBaseCalled = true;
}

void ObjectGroup::createPipeline(PipelineManager &pipelineManager) {
    auto array = getChildren();
    for (ObjectBase *obj : array) {
        if (obj == this) continue; // skip self
        obj->createPipeline(pipelineManager);
    }
    pipelineManagerUsed = &pipelineManager;
    destroyPipelineCalled = false;
    createPipelineCalled = true;
}

void ObjectGroup::destroyPipeline(PipelineManager &pipelineManager) {
    auto array = getChildren();
    for (ObjectBase *obj : array) {
        if (obj == this) continue; // skip self
        obj->destroyPipeline(pipelineManager);
    }
    pipelineManagerUsed = &pipelineManager;
    createPipelineCalled = false;
    destroyPipelineCalled = true;
}

void ObjectGroup::create() {
    auto array = getChildren();
    for (ObjectBase *obj : array) {
        if (obj == this) continue; // skip self
        obj->create();
    }
    destroyCalled = false;
    createCalled = true;
}

void ObjectGroup::resize(PipelineManager &pipelineManager) {
    auto array = getChildren();
    for (ObjectBase *obj : array) {
        if (obj == this) continue; // skip self
        obj->resize(pipelineManager);
    }
    pipelineManagerUsed = &pipelineManager;
    resizeCalled = true;
}

bool ObjectGroup::onTouchEvent(MultiTouch &touch) {
    // To incorporate translated multi touch, we need to convert this
    // to a group
//    auto desc = getDiligentAppBase().m_pSwapChain->GetDesc();
//    float currentWidth = desc.Width;
//    float currentHeight = desc.Height;
    long count = touch.getTouchCount();
    auto array = getChildren();
    for (ObjectBase *obj : array) {
        if (obj == this) continue; // skip self
        for (long l = 0; l < count; l++) {
            MultiTouch::TouchData copy = touch.getTouchAt(touch.getTouchIndex());
            if (copy.state == MultiTouch::TouchState::TOUCH_DOWN) {
                if (copy.x >= 0 && copy.x <= copy.x + 200) {
    //                copy.x
                }
            }
            // compute triangle
    //        {
    //            float newWidth = 200;
    //            float newHeight = 200;
    //            copy.x = (copy.x/currentWidth)*newWidth;
    //            copy.y = (copy.y/currentHeight)*newHeight;
    //        }
        }
        if (obj->onTouchEvent(touch)) {
            return true;
        }
    }
    return false;
}

void ObjectGroup::destroy() {
    auto array = getChildren();
    for (ObjectBase *obj : array) {
        if (obj != this) removeChild(obj);
    }
    createCalled = false;
    destroyCalled = true;
}

bool ObjectGroup::hasPhysics() {
    bool hasPhysics_ = false;
    auto array = getChildren();
    for (ObjectBase *obj : array) {
        if (obj == this) continue; // skip self
        if (obj->hasPhysics()) {
            hasPhysics_ = true;
            break;
        }
    }
    hasPhysicsCalled = true;
    return hasPhysics_;
}

void ObjectGroup::physics(const TimeEngine &timeEngine) {
    // TODO: support mulitple time engines
    auto array = getChildren();
    for (ObjectBase *obj : array) {
        if (obj == this) continue; // skip self
        obj->physics(timeEngine);
    }
}

size_t ObjectGroup::getChildCount() const {
    return children.table->objectCount();
}

AutoResizingArray<ObjectBase *> ObjectGroup::getChildren() const {
    // TODO: optimize
    AutoResizingArray<ObjectBase*> array;
    auto it = children.table->getIterator();
    while (it.hasNext()) array.add(it.next()->resource.get<ObjectBase*>());
    return array;
}

void ObjectGroup::addChild(ObjectBase * obj) {
    if (obj == this) {
        // do not deallocate myself when removed
        children.newObject(ObjectTypeNone, ObjectFlagNone, obj);
        return;
    }
    children.newObject(ObjectTypeNone, ObjectFlagAutoDeallocateResource, obj);
    if (setDiligentAppBaseCalled) obj->setDiligentAppBase(&getDiligentAppBase());
    if (!destroyPipelineCalled) {
        if (createPipelineCalled) obj->createPipeline(*pipelineManagerUsed);
    }
    if (!destroyCalled) {
        if (createCalled) obj->create();
        if (resizeCalled) obj->resize(*pipelineManagerUsed);
    }

    if (hasPhysicsCalled) obj->hasPhysics();
}

void ObjectGroup::removeChild(ObjectBase *obj) {
    // obj is destroyed here
    if (createCalled && !destroyCalled) obj->destroy();
    if (!destroyPipelineCalled) obj->destroyPipeline(*pipelineManagerUsed);
    children.table->remove(obj);
}

ObjectGroup::~ObjectGroup() {
}
