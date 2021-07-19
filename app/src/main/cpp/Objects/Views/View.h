//
// Created by Matthew Good on 14/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_VIEW_H
#define GRAPHICAL_TOOL_KIT_VIEW_H

#include <ObjectBase.h>

/**
 * an empty View, this does nothing by itself
 */
class View : public ObjectBase {
private:
    VertexEngine vertexEngine;
    int chunkSize = 3*50;
    int canvas_width = 400;
    int canvas_height = 400;

    void drawChunks(VertexEngine::GenerationInfo &info);

    Diligent::BufferData VBData;
    Diligent::BufferDesc VertBuffDesc;
    Diligent::BufferData IBData;
    Diligent::BufferDesc IndBuffDesc;

    static const char * vertexShader;
    static const char * pixelShader;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                vertexBuffer;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                indexBuffer;

public:
    void create() override final;
    void draw() override final;
    void destroy() override final;
    virtual void onCreate();
    virtual void onDraw(VertexEngine::Canvas & canvas);
    virtual void onDestroy();
};

#endif //GRAPHICAL_TOOL_KIT_VIEW_H
