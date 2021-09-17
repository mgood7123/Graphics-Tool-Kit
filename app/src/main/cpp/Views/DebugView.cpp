//
// Created by Matthew Good on 4/9/21.
//

#include "Views.h"

void DebugView::addView(View * view) {
    if (linearLayout->getChildCount() == 2) {
        Log::Error_And_Throw("DebugView only supports a single child");
    }
    if (contentView != nullptr) {
        linearLayout->removeView(contentView);
    }
    contentView = view;
    linearLayout->addView(contentView, new LinearLayout::LinearLayoutParams(8));
}

const char *DebugView::getPipelineKeyA() {
    return "DebugView A";
}

const char *DebugView::getPipelineKeyB() {
    return "DebugView B";
}

DebugView::DebugView() {
    linearLayout = new LinearLayout;
    linearLayout->addView(new TouchDetectorPainter(), new LinearLayout::LinearLayoutParams(
            LayoutParams::GRAVITY_LEFT,
            1
    ));
    RootView::addView(linearLayout);
}

DebugView::~DebugView() {
    RootView::removeView(linearLayout);
}
