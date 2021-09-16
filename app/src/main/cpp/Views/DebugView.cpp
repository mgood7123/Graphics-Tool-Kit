//
// Created by Matthew Good on 4/9/21.
//

#include "DebugView.h"

void DebugView::addView(View * view) {
    if (getChildCount() == 1) {
        Log::Error_And_Throw("DebugView only supports a single child");
    }
    if (contentView != nullptr) {
        linearLayout->removeView(contentView);
    }
    contentView = view;
    linearLayout->addView(contentView);
}

const char *DebugView::getPipelineKeyA() {
    return "DebugView A";
}

const char *DebugView::getPipelineKeyB() {
    return "DebugView B";
}

void DebugView::onCreate() {
    linearLayout = new LinearLayout;
}

void DebugView::onDestroy() {
    delete linearLayout;
}
