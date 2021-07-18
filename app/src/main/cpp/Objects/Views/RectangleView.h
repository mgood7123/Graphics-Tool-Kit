//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_RECTANGLEVIEW_H
#define GRAPHICAL_TOOL_KIT_RECTANGLEVIEW_H

#include "View.h"
#include <DiligentCore/Common/interface/BasicMath.hpp>
#include <DiligentCore/Graphics/GraphicsEngine/interface/Buffer.h>

class RectangleView : public View
{
public:
    static const char * rectangle_VS;
    static const char * rectangle_PS;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_CubeVertexBuffer;
    Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_CubeIndexBuffer;
    std::vector<float> green = {0, 1, 0, 1};
    std::vector<float> blue = {0, 0, 1, 1};
    std::vector<float> pink = {1, 0, 1, 1};

    Diligent::BufferData VBData;
    Diligent::BufferDesc VertBuffDesc;
    Diligent::BufferData IBData;
    Diligent::BufferDesc IndBuffDesc;

    int chunkSize = 3*50;

    void create() override;
    void draw() override;
    void drawChunks(VertexEngine::GenerationInfo &info, size_t chunkSize) override;
    void destroy() override;
};


#endif //GRAPHICAL_TOOL_KIT_RECTANGLE_H
