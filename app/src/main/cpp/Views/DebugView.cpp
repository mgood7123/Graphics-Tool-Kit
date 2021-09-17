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
    LinearLayout * x = new LinearLayout();
    x->setOrientation(LinearLayout::Horizontal);
    x->addView((new TextView("DebugView"))->setFontSize(100));
//    x->addView((new TextView("DebugView"))->setFontSize(100));
//    x->addView((new TextView("DebugView"))->setFontSize(100));
//    x->addView((new TextView("DebugView"))->setFontSize(100));
    linearLayout->addView(x);
    RootView::addView(linearLayout);
}

DebugView::~DebugView() {
    RootView::removeView(linearLayout);
}
