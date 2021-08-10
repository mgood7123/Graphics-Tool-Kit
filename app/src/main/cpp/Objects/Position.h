//
// Created by Matthew Good on 9/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_POSITION_H
#define GRAPHICAL_TOOL_KIT_POSITION_H

struct Position {
    enum {
        MATCH_PARENT = -1
    };

    int x;
    int y;
    int width;
    int height;

    constexpr inline
    Position() : Position(0, 0, MATCH_PARENT, MATCH_PARENT) {}

    constexpr inline
    Position(const int & value) : Position(value, value, value, value) {}

    constexpr inline
    Position(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
};

#endif //GRAPHICAL_TOOL_KIT_POSITION_H
