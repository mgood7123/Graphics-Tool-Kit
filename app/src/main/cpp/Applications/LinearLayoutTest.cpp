//
// Created by Matthew Good on 25/8/21.
//

#include "LinearLayoutTest.h"

MAKE_APPLICATION_PUBLIC__SOURCE(LinearLayoutTest);

void LinearLayoutTest::onCreateViews() {
    ALLOCATE_VIEW(linearLayout, LinearLayout);
    ALLOCATE_VIEW(A, TouchDetectorPainter);
    ALLOCATE_VIEW(B, RectanglePainter);
    ALLOCATE_VIEW(C, TouchDetectorPainter);
    linearLayout->addView(A, new LinearLayout::LinearLayoutParams(1));
    linearLayout->addView(B, new LinearLayout::LinearLayoutParams(4));
    linearLayout->addView(C, new LinearLayout::LinearLayoutParams(1));
    setContentView(linearLayout);
}
