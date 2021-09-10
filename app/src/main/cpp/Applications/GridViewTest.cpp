//
// Created by Matthew Good on 25/8/21.
//

#include "GridViewTest.h"

MAKE_APPLICATION_PUBLIC__SOURCE(GridViewTest);

void GridViewTest::onCreateViews() {
    ALLOCATE_VIEW(rootView, RootView);
    ALLOCATE_VIEW(linearLayout, GridView);
    ALLOCATE_VIEW(A, RectanglePainter);
    ALLOCATE_VIEW(B, RectanglePainter);
    ALLOCATE_VIEW(C, RectanglePainter);
    ALLOCATE_VIEW(D, RectanglePainter);
    A->padding = 10;
    B->padding = 10;
    C->padding = 10;
    D->padding = 10;
    linearLayout->addView(A);
    linearLayout->addView(B);
    linearLayout->addView(C);
    linearLayout->addView(D);
    rootView->addView(linearLayout);
    rootView->padding = 20;
    setContentView(rootView);
}
