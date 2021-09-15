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

// if you have a viewgroup with A and B children,
// the viewgroup will receive a MotionEvent with two pointers,
// and is then responsible for splitting them up and only
// dispatching the relevant pointer to each view.
bool ViewGroup::onTouchEvent(MultiTouch &touch) {
    if (log_TouchEnterExit) {
        Log::Info(getTag(), ": ", "onTouchEvent enter");
    }
    bool handled = false;
    auto * t_ = &touch.getTouchAt(touch.getTouchIndex());
    if (log_TouchPosition) {
        Log::Info(getTag(), ": ", "touch position ", Position(t_->x, t_->y));
    }

    Rectangle absolutePosition;

    bool hasCancellation = false;

    auto split = getChildren();
    for (View *view : split) {
        absolutePosition = view->getAbsolutePosition();
        auto it = touch.getIterator();
        while (it.hasNext()) {
            MultiTouch::TouchData *touchData = it.next();

            // if a cancellation event occurs, send cancellation to all views as last event
            if (touchData->state == MultiTouch::TouchState::TOUCH_CANCELLED) {
                hasCancellation = true;
            }
            if (touchData->state == MultiTouch::TouchState::TOUCH_DOWN) {
                if (touchData->x > absolutePosition.topLeft.x) {
                    if (touchData->y > absolutePosition.topLeft.y) {
                        if (touchData->x < absolutePosition.bottomRight.x) {
                            if (touchData->y < absolutePosition.bottomRight.y) {
                                if (tracked_views.objectCount() == 0) {
                                    if (log_TouchEvents) {
                                        Log::Info(getTag(), ": ", "tracking view at ", absolutePosition.toString());
                                    }
                                    // obtain a reference, do not obtain a copy
                                    MultiTouch &x = tracked_views.newObject(ObjectTypeNone,
                                                                            ObjectFlagAutoDeallocateResource,
                                                                            new TrackingData(view,
                                                                                             {}))->resource.get<TrackingData *>()->second;
                                    if (log_TouchPointers) {
                                        x.debug = true;
                                    }
                                    x.setMaxSupportedTouches(touch.getMaxSupportedTouches());
                                    // store copy of touch data
                                    x.addTouch(*touchData);
                                    view->focus = true;
                                    handled = true;
                                } else {
                                    bool found = false;
                                    auto i = tracked_views.table->getIterator();
                                    while (i.hasNext()) {
                                        TrackingData *td = i.next()->resource.get<TrackingData *>();
                                        if (td->first == view) {
                                            found = true;

                                            bool exists = false;

                                            // check if touch exists
                                            auto it2 = td->second.getIterator();
                                            while (it2.hasNext()) {
                                                if (it2.next()->identity == touchData->identity) {
                                                    exists = true;
                                                    break;
                                                }
                                            }

                                            // store copy of touch data if it doesnt exist
                                            if (!exists) {
                                                if (log_TouchEvents) {
                                                    Log::Info(getTag(), ": ",
                                                              "touch with identity ",
                                                              touchData->identity,
                                                              " does not exist, adding");
                                                }
                                                td->second.addTouch(*touchData);
                                                handled = true;
                                            } else {
                                                if (log_TouchEvents) {
                                                    Log::Info(getTag(), ": ",
                                                              "touch with identity ",
                                                              touchData->identity,
                                                              " already exists, skipping");
                                                }
                                            }
                                            break;
                                        }
                                    }
                                    if (!found) {
                                        if (log_TouchEvents) {
                                            Log::Info(getTag(), ": ", "tracking view at ", absolutePosition.toString());
                                        }
                                        // obtain a reference, do not obtain a copy
                                        MultiTouch &x = tracked_views.newObject(ObjectTypeNone,
                                                                                ObjectFlagAutoDeallocateResource,
                                                                                new TrackingData(
                                                                                        view,
                                                                                        {}))->resource.get<TrackingData *>()->second;
                                        if (log_TouchPointers) {
                                            x.debug = true;
                                        }
                                        x.setMaxSupportedTouches(touch.getMaxSupportedTouches());
                                        // store copy of touch data
                                        x.addTouch(*touchData);
                                        view->focus = true;
                                        handled = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // for each view, update the associated touch pointer
    auto i = tracked_views.table->getIterator();
    while (i.hasNext()) {
        TrackingData *td = i.next()->resource.get<TrackingData *>();
        // obtain a reference, do not obtain a copy
        MultiTouch &multiTouch = td->second;

        MultiTouch::TouchData *cancel = nullptr;

        bool matches = false;
        bool lastUp = false;

        // touch up/down are guaranteed to always be sent as separate events
        std::vector<MultiTouch::TouchData *> pointer_ups;

        // check if we match any associated touch pointers
        auto it = multiTouch.getIterator();
        while (it.hasNext()) {
            MultiTouch::TouchData *touchData = it.next();

            // if cancelling, pick first touch as cancellation point
            if (hasCancellation && cancel == nullptr) cancel = touchData;

            // check against available touch pointers
            auto it2 = touch.getIterator();
            while (it2.hasNext()) {
                MultiTouch::TouchData *touchData2 = it2.next();

                // if touch matches view's associated touch
                if (touchData2->identity == touchData->identity) {
                    matches = true;

                    // update touch data
                    if (touchData2->state == MultiTouch::TouchState::TOUCH_MOVE) {
                        multiTouch.moveTouch(*touchData2);
                    }

                    // handle touch up
                    if (touchData2->state == MultiTouch::TouchState::TOUCH_UP) {
                        pointer_ups.push_back(touchData2);
                    }
                }
            }
        }

        // skip this view if no touches match this view
        // can this ever happen?
        if (!matches) {
            Log::Error(getTag(), ": ", "DOES NOT MATCH! THIS MIGHT BE A BUG!");
            continue;
        }

        // issue pointer up event after pointer move
        if (pointer_ups.size() > 1) {
            Log::Error("pointer ups is greater than 1, THIS SHOULD NOT HAPPEN!!!");
        }
        for (MultiTouch::TouchData *touchUp : pointer_ups) {
            multiTouch.removeTouch(*touchUp);

            // if touch count is 1, the last touch has gone up
            lastUp = multiTouch.getTouchCount() == 1;
        }

        // issue touch event

        if (td->first->onTouchEvent(multiTouch)) {
            handled = true;
        }

        // issue cancellation as last event

        if (hasCancellation) {
            multiTouch.cancelTouch(*cancel);
            if (td->first->onTouchEvent(multiTouch)) {
                handled = true;
            }
        }

        // if the last touch has gone up, untrack the view
        if (lastUp) {
            if (log_TouchEvents) {
                Log::Info(getTag(), ": ", "untracking view at ", td->first->getAbsolutePosition().toString());
            }
            if (!td->first->focus) {
                Log::Error_And_Throw(getTag(), ": ",
                                     "cannot loose focus for a view that has no focus");
            }
            td->first->focus = false;

            // TrackingData has no internal allocation
            // it is safe to delete primary allocation
            tracked_views.deleteObject(td);
        }
    }
    if (log_TouchEnterExit) {
        Log::Info(getTag(), ": ", "onTouchEvent exit");
    }
    return handled;
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
    return children.objectCount();
}

View * ViewGroup::getChildAt(size_t index) const {
    // TODO: optimize
    size_t count = 0;
    auto it = children.table->getIterator();
    while (it.hasNext()) {
        Object * obj = it.next();
        if (count != index) {
            count++;
            continue;
        }
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
