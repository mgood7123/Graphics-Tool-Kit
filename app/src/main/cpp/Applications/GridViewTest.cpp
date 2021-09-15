//
// Created by Matthew Good on 25/8/21.
//

#include "GridViewTest.h"

MAKE_APPLICATION_PUBLIC__SOURCE(GridViewTest);

void GridViewTest::onCreateViews() {
    ALLOCATE_VIEW(gridView, GridView);
    ALLOCATE_VIEW(A, RectanglePainter);
    ALLOCATE_VIEW(B, RectanglePainter);
    ALLOCATE_VIEW(C, RectanglePainter);
    ALLOCATE_VIEW(D, RectanglePainter);
    A->paddingInVirtualCanvas = 25;
    B->paddingInVirtualCanvas = 25;
    C->paddingInVirtualCanvas = 10;
    D->offsetInVirtualCanvas.y = 23;
    D->paddingInVirtualCanvas = 10;
    gridView->addView(A);
    gridView->addView(B);
    gridView->addView(C);
    gridView->addView(D);
    gridView->offsetInVirtualCanvas = 30;
    setContentView(gridView);
}
