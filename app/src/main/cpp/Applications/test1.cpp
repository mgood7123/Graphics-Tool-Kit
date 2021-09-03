//
// Created by Matthew Good on 25/8/21.
//

#include "test1.h"

MAKE_APPLICATION_PUBLIC__SOURCE(test1);

void test1::onCreateViews() {
    RootView * a;
    RootView * b;
    GridView * c;

    a = new RootView();
    a->setTag("root");
    a->padding = 25;
    setContentView(a);

    b = new RootView();
    b->setTag("root child");
    a->addView(b);

    c = new GridView();
    c->setTag("grid");
    c->addView(new TouchDetectorPainter());
    auto * x = new TouchDetectorPainter();
    x->padding = 50;
    c->addView(x);
    c->addView(new TouchDetectorPainter());
    c->addView(new RectanglePainter());
    b->addView(c);
}
