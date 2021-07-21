//
// Created by Matthew Good on 30/6/21.
//

#include "RectangleView.h"

void RectangleView::onCreate(VertexEngine::TextureManager &textureManager) {
    Diligent::TextureLoadInfo loadInfo;
    loadInfo.IsSRGB = true;
    textureManager.createTextureFromFile(
            "A",
            "/Users/smallville7123/Documents/s1.png",
            loadInfo,
            diligentAppBase->m_pDevice
    );
    textureManager.createTextureFromFile(
            "B",
            "/Users/smallville7123/Documents/sticker2.jpg",
            loadInfo,
            diligentAppBase->m_pDevice
    );
    textureManager.createTextureFromFile(
            "C",
            "/Users/smallville7123/Documents/sticker.jpg",
            loadInfo,
            diligentAppBase->m_pDevice
    );
}

void RectangleView::onDraw(VertexEngine::Canvas &canvas) {
    canvas.fill("A");

//     from [x, y] to [x, y]
    canvas.planeAt(20, 20, 200, 200, "B");
//     from [x, y] to [x+width, y+width]
    canvas.plane(140, 140, 200, 200, blue);
    canvas.plane(160, 160, 80, 80, pink);

    canvas.subCanvas(200, 200, 90, 90).fill("C");
}

void RectangleView::onDestroy(VertexEngine::TextureManager &textureManager) {
    textureManager.deleteTexture("A");
    textureManager.deleteTexture("B");
    textureManager.deleteTexture("C");
}
