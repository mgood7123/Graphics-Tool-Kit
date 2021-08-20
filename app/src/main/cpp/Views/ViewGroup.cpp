//
// Created by Matthew Good on 9/8/21.
//

#include "ViewGroup.h"

void ViewGroup::setDiligentAppBase(DiligentAppBase *diligentAppBase) {
    View::setDiligentAppBase(diligentAppBase);
    auto array = getChildren();
    for (View *view : array) {
        if (view == this) continue; // skip self
        view->setDiligentAppBase(diligentAppBase);
    }
    setDiligentAppBaseCalled = true;
}

void ViewGroup::createPipeline(PipelineManager &pipelineManager) {
    auto array = getChildren();
    for (View *view : array) {
        if (view == this) continue; // skip self
        view->createPipeline(pipelineManager);
    }
    pipelineManagerUsed = &pipelineManager;
    destroyPipelineCalled = false;
    createPipelineCalled = true;
    onCreatePipeline(pipelineManager);
}

void ViewGroup::destroyPipeline(PipelineManager &pipelineManager) {
    auto array = getChildren();
    for (View *view : array) {
        if (view == this) continue; // skip self
        view->destroyPipeline(pipelineManager);
    }
    pipelineManagerUsed = &pipelineManager;
    createPipelineCalled = false;
    destroyPipelineCalled = true;
    onDestroyPipeline(pipelineManager);
}

void ViewGroup::resize(PipelineManager &pipelineManager) {
    resizeWidth = getDiligentAppBase().swapChainWidth;
    resizeHeight = getDiligentAppBase().swapChainHeight;
    auto array = getChildren();
    for (View *view : array) {
        if (view == this) continue; // skip self
        view->resize(pipelineManager);
    }
    pipelineManagerUsed = &pipelineManager;
    resizeCalled = true;
    onResize(pipelineManager);
}

void ViewGroup::create() {
    auto array = getChildren();
    for (View *view : array) {
        if (view == this) continue; // skip self
        view->create();
    }
    destroyCalled = false;
    createCalled = true;
    onCreate();
}

bool ViewGroup::onTouchEvent(MultiTouch &touch) {
    auto array = getChildren();
    MultiTouch::TouchData copy = touch.getTouchAt(touch.getTouchIndex());
    Log::Debug("touch : x: ", copy.x, ", y: ", copy.y);
    Rectangle absolutePosition;
    if (parent == nullptr) absolutePosition = getAbsolutePosition();
    for (View *view : array) {
        if (view == this) continue; // skip self
        bool touch_is_inside_view = false;
        if (parent != nullptr) {
            absolutePosition = view->getAbsolutePosition();
        }
        if (copy.x > absolutePosition.topLeft.x) {
            if (copy.y > absolutePosition.topLeft.y) {
                if (copy.x < absolutePosition.bottomRight.x) {
                    if (copy.y < absolutePosition.bottomRight.y) {
                        touch_is_inside_view = true;
                    }
                }
            }
        }
        if (touch_is_inside_view) {
            if (copy.state == MultiTouch::TouchState::TOUCH_DOWN) {
                identity = copy.identity;
//                Log::Debug("identity:      ", identity);
//                Log::Debug("copy.identity: ", copy.identity);
                view->focus = true;
//                Log::Info("FOCUS GAIN");
                if (view->onTouchEvent(touch)) {
                    return true;
                }
            }
        }
        if (
            view->focus
            && copy.state == MultiTouch::TouchState::TOUCH_MOVE
            && view->onTouchEvent(touch)
        ) {
            return true;
        }
//        Log::Debug("identity:      ", identity);
//        Log::Debug("copy.identity: ", copy.identity);
//        Log::Debug("state:         ", MultiTouch::stateToString(copy.state));
        if (
            copy.identity == identity &&
            (
             copy.state == MultiTouch::TouchState::TOUCH_UP
             || copy.state == MultiTouch::TouchState::TOUCH_CANCELLED
            )
        ) {
            if (view->focus) {
                identity = 0;
//                Log::Info("FOCUS LOST");
                view->focus = false;
                if (view->onTouchEvent(touch)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void ViewGroup::destroy() {
    auto array = getChildren();
    for (View *view : array) {
        removeView(view);
    }
    createCalled = false;
    destroyCalled = true;
    onDestroy();
}

bool ViewGroup::hasPhysics() {
    bool hasPhysics_ = false;
    auto array = getChildren();
    for (View *view : array) {
        if (view == this) continue; // skip self
        if (view->hasPhysics()) {
            hasPhysics_ = true;
            break;
        }
    }
    hasPhysicsCalled = true;
    return hasPhysics_;
}

void ViewGroup::physics(const TimeEngine &timeEngine) {
    // TODO: support mulitple time engines
    auto array = getChildren();
    for (View *view : array) {
        if (view == this) continue; // skip self
        view->physics(timeEngine);
    }
}

size_t ViewGroup::getChildCount() const {
    return children.table->objectCount();
}

View * ViewGroup::viewAt(size_t index) const {
    size_t count = 0;
    auto it = children.table->getIterator();
    while (it.hasNext()) {
        Object * obj = it.next();
        if (count != index) continue;
        return obj->resource.get<View*>();
    }
    return nullptr;
}


AutoResizingArray<View *> ViewGroup::getChildren() const {
    // TODO: optimize
    AutoResizingArray<View *> array;
    auto it = children.table->getIterator();
    while (it.hasNext()) array.add(it.next()->resource.get<View*>());
    return array;
}

void ViewGroup::drawRenderTarget(DrawTools &tools, Diligent::IDeviceContext *deviceContext) {
}

void ViewGroup::addView(View * view) {
    if (view != this && view->parent != nullptr) {
        Log::Error_And_Throw("cannot add a View that has a parent, please call `ViewGroup::removeFromParent(view);` before calling `addView(view);`");
    }
    
    children.newObject(
        ObjectTypeNone,
        view != this ? ObjectFlagAutoDeallocateResource : ObjectFlagNone,
        view
    );

    if (view == this) return;

    view->parent = this;

    if (setDiligentAppBaseCalled) view->setDiligentAppBase(&getDiligentAppBase());
    if (!destroyPipelineCalled) {
        if (createPipelineCalled) view->createPipeline(*pipelineManagerUsed);
    }
    if (!destroyCalled) {
        if (createCalled) view->create();
        if (resizeCalled) view->resize(*pipelineManagerUsed);
    }

    if (hasPhysicsCalled) view->hasPhysics();
}

void ViewGroup::removeView(View *view) {
    // child->view is destroyed here
    auto it = children.table->getIterator();
    while (it.hasNext()) {
        if (it.next()->resource.get<View *>() == view) {
            if (view != this) {
                if (createCalled && !destroyCalled) view->destroy();
                if (!destroyPipelineCalled) view->destroyPipeline(*pipelineManagerUsed);
            }
            children.table->remove(view);
            return;
        }
    }
}

void ViewGroup::detachView(View *view) {
    // child->view is destroyed here
    auto it = children.table->getIterator();
    while (it.hasNext()) {
        if (it.next()->resource.get<View *>() == view) {
            if (view != this) {
                if (createCalled && !destroyCalled) view->destroy();
                if (!destroyPipelineCalled) view->destroyPipeline(*pipelineManagerUsed);
            }
            // disable allocation and remove
            children.table->removeWithoutDeallocating(view);
            if (view != this) {
                view->parent = nullptr;
            }
            return;
        }
    }
}

void ViewGroup::onCreatePipeline(PipelineManager &pipelineManager) {

}

void ViewGroup::onDestroyPipeline(PipelineManager &pipelineManager) {

}

void ViewGroup::onResize(PipelineManager &pipelineManager) {

}

void ViewGroup::onCreate() {

}

void ViewGroup::onDestroy() {

}
