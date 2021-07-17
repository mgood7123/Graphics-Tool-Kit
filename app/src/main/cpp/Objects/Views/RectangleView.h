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
    void create() override;
    void draw() override;
    void destroy() override;
};


#endif //GRAPHICAL_TOOL_KIT_RECTANGLE_H
