//
// Created by Matthew Good on 30/6/21.
//

#include "RectanglePainter.h"

void RectanglePainter::onCreate(VertexEngine::TextureManager &textureManager) {
    mousePosition = 0;
#if PLATFORM_ANDROID
#else
    auto pathA = "/Users/smallville7123/Documents/s1.png";
    auto pathB = "/Users/smallville7123/Documents/sticker2.jpg";
    auto pathC = "/Users/smallville7123/Documents/sticker.jpg";

    Diligent::TextureLoadInfo loadInfo;
    loadInfo.IsSRGB = true;
    textureManager.createTextureFromFile("A", pathA, loadInfo);
    textureManager.createTextureFromFile("B", pathB, loadInfo);
    textureManager.createTextureFromFile("C", pathC, loadInfo);
#endif
}

int RectanglePainter::getCanvasWidth() {
    return 400;
}

int RectanglePainter::getCanvasHeight() {
    return 400;
}

void RectanglePainter::onDraw(VertexEngine::Canvas &canvas) {
#if PLATFORM_ANDROID
#else
    canvas.fill("A");

//     from [x, y] to [x, y]
    canvas.planeAt(20, 20, 200, 200, "B");
#endif
//     from [x, y] to [x+width, y+width]
    canvas.plane(140, 140, 200, 200, blue);
    canvas.plane(160, 160, 80, 80, pink);

    canvas.plane(mousePosition.x-40, mousePosition.y-40, 80, 80, red);
    canvas.plane(200, 200, 90, 90, "C");

#if PLATFORM_ANDROID
#else
//    canvas.subCanvas(200, 200, 90, 90).fill("C");
#endif
}

void RectanglePainter::onDestroy(VertexEngine::TextureManager &textureManager) {
#if PLATFORM_ANDROID
#else
    textureManager.deleteTexture("A");
    textureManager.deleteTexture("B");
    textureManager.deleteTexture("C");
#endif
}

bool RectanglePainter::onTouchEvent(MultiTouch &touch) {
    auto t = touch.getTouchAt(touch.getTouchIndex());
    mousePosition = ((Position(t.x, t.y) - getAbsolutePosition().topLeft) / getRelativePosition().topLeft) * Position(getCanvasWidth(), getCanvasHeight());
    Log::Debug("mouse = ", mousePosition.toString());
    return true;
}
