//
// Created by Matthew Good on 9/8/21.
//

#include "ObjectGroup.h"

void ObjectGroup::setDiligentAppBase(DiligentAppBase *diligentAppBase) {
    ObjectBase::setDiligentAppBase(diligentAppBase);
    auto array = getChildren();
    for (ChildInfo *child : array) {
        if (child->object == this) continue; // skip self
        child->object->setDiligentAppBase(diligentAppBase);
    }
    setDiligentAppBaseCalled = true;
}

void ObjectGroup::createPipeline(PipelineManager &pipelineManager) {
    auto array = getChildren();
    for (ChildInfo *child : array) {
        if (child->object == this) continue; // skip self
        child->object->createPipeline(pipelineManager);
    }
    pipelineManagerUsed = &pipelineManager;
    destroyPipelineCalled = false;
    createPipelineCalled = true;
}

void ObjectGroup::destroyPipeline(PipelineManager &pipelineManager) {
    auto array = getChildren();
    for (ChildInfo *child : array) {
        if (child->object == this) continue; // skip self
        child->object->destroyPipeline(pipelineManager);
    }
    pipelineManagerUsed = &pipelineManager;
    createPipelineCalled = false;
    destroyPipelineCalled = true;
}

void ObjectGroup::create() {
    auto array = getChildren();
    for (ChildInfo *child : array) {
        if (child->object == this) continue; // skip self
        child->object->create();
    }
    destroyCalled = false;
    createCalled = true;
}

void ObjectGroup::resize(PipelineManager &pipelineManager) {
    resizeWidth = getDiligentAppBase().swapChainWidth;
    resizeHeight = getDiligentAppBase().swapChainHeight;
    auto array = getChildren();
    for (ChildInfo *child : array) {
        if (child->object == this) continue; // skip self
        child->object->resize(pipelineManager);
    }
    pipelineManagerUsed = &pipelineManager;
    resizeCalled = true;
}

bool ObjectGroup::onTouchEvent(MultiTouch &touch) {
//    auto & app = getDiligentAppBase();
    long count = touch.getTouchCount();
    auto array = getChildren();
    for (ChildInfo *child : array) {
        if (child->object == this) continue; // skip self
        for (long l = 0; l < count; l++) {
            MultiTouch::TouchData copy = touch.getTouchAt(touch.getTouchIndex());
            bool touch_is_inside_view = false;
            if (copy.x > child->positionInWindow.x) {
                if (copy.y > child->positionInWindow.y) {
                    if (copy.x < child->positionInWindow.width) {
                        if (copy.y < child->positionInWindow.height) {
                            touch_is_inside_view = true;
                        }
                    }
                }
            }
            Log::Debug("TOUCH IS ", touch_is_inside_view ? "INSIDE" : "OUTSIDE", " VIEW");
            if (touch_is_inside_view) {
                if (copy.state == MultiTouch::TouchState::TOUCH_DOWN) {
                    child->object->focus = true;
                    if (child->object->onTouchEvent(touch)) {
                        return true;
                    }
                }
            }
            if (copy.state == MultiTouch::TouchState::TOUCH_UP) {
                child->object->focus = false;
            }
        }
    }
    return false;
}

void ObjectGroup::destroy() {
    auto array = getChildren();
    for (ChildInfo *child : array) {
        removeChild(child);
    }
    createCalled = false;
    destroyCalled = true;
}

bool ObjectGroup::hasPhysics() {
    bool hasPhysics_ = false;
    auto array = getChildren();
    for (ChildInfo *child : array) {
        if (child->object == this) continue; // skip self
        if (child->object->hasPhysics()) {
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
    for (ChildInfo *child : array) {
        if (child->object == this) continue; // skip self
        child->object->physics(timeEngine);
    }
}

size_t ObjectGroup::getChildCount() const {
    return children.table->objectCount();
}

AutoResizingArray<ObjectGroup::ChildInfo *> ObjectGroup::getChildren() const {
    // TODO: optimize
    AutoResizingArray<ChildInfo *> array;
    auto it = children.table->getIterator();
    while (it.hasNext()) array.add(it.next()->resource.get<ChildInfo*>());
    return array;
}

void ObjectGroup::addChild(ObjectBase * obj) {
    if (obj != this && obj->parent != nullptr) {
        Log::Error_And_Throw("cannot add an Object that has a parent, please call `ObjectGroup::removeFromParent(obj);` before calling `addChild(obj);`");
    }
    addChild({obj, obj != this});
}

void ObjectGroup::addChild(const ChildInfo & childInfo) {
    if (childInfo.object != this && childInfo.object->parent != nullptr) {
        Log::Error_And_Throw("cannot add an Object that has a parent, please call `ObjectGroup::removeFromParent(childInfo.object);` before calling `addChild(childInfo);`");
    }
    
    ChildInfo childCopy(childInfo, false);

    children.newObject(ObjectTypeNone, ObjectFlagNone, childInfo);

    if (childCopy.object == this) return;

    childCopy.object->parent = this;

    if (setDiligentAppBaseCalled) childCopy.object->setDiligentAppBase(&getDiligentAppBase());
    if (!destroyPipelineCalled) {
        if (createPipelineCalled) childCopy.object->createPipeline(*pipelineManagerUsed);
    }
    if (!destroyCalled) {
        if (createCalled) childCopy.object->create();
        if (resizeCalled) childCopy.object->resize(*pipelineManagerUsed);
    }

    if (hasPhysicsCalled) childCopy.object->hasPhysics();
}

void ObjectGroup::addChild(ChildInfo * childInfo) {
    if (childInfo->object != this && childInfo->object->parent != nullptr) {
        Log::Error_And_Throw("cannot add an Object that has a parent, please call `ObjectGroup::detachFromParent(childInfo->object);` before calling `addChild(childInfo);`");
    }
    addChild(*childInfo);
}

void ObjectGroup::removeChild(ObjectBase *obj) {
    // child->object is destroyed here
    auto it = children.table->getIterator();
    while (it.hasNext()) {
        ChildInfo * child = it.next()->resource.get<ChildInfo *>();
        if (child->object == obj) {
            removeChild(child);
            return;
        }
    }
}

void ObjectGroup::removeChild(ChildInfo *childInfo) {
    removeChild(*childInfo);
}

void ObjectGroup::removeChild(ChildInfo & childInfo) {
    // child->object is destroyed here
    if (childInfo.object != this) {
        if (createCalled && !destroyCalled) childInfo.object->destroy();
        if (!destroyPipelineCalled) childInfo.object->destroyPipeline(*pipelineManagerUsed);
    }
    children.table->remove(&childInfo);
}

void ObjectGroup::detachChild(ObjectBase *obj) {
    // child->object is destroyed here
    auto it = children.table->getIterator();
    while (it.hasNext()) {
        ChildInfo * child = it.next()->resource.get<ChildInfo *>();
        if (child->object == obj) {
            detachChild(child);
            return;
        }
    }
}

void ObjectGroup::detachChild(ChildInfo *childInfo) {
    detachChild(*childInfo);
}

void ObjectGroup::detachChild(ChildInfo & childInfo) {
    // child->object is destroyed here
    if (childInfo.object != this) {
        if (createCalled && !destroyCalled) childInfo.object->destroy();
        if (!destroyPipelineCalled) childInfo.object->destroyPipeline(*pipelineManagerUsed);
    }
    // disable allocation and remove
    children.table->removeWithoutDeallocating(&childInfo);
    childInfo.object->parent = nullptr;
}
