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
    linearLayout->addView(A);
    linearLayout->addView(B);
    linearLayout->addView(C);
    setContentView(linearLayout);
}
