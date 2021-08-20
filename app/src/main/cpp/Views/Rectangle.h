//
// Created by Matthew Good on 9/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_RECTANGLE_H
#define GRAPHICAL_TOOL_KIT_RECTANGLE_H

#include "Position.h"

class Rectangle {

    template<typename T>
    std::string to_string(const T & t) const {
        std::string str{std::to_string(t)};
        int offset{1};
        if (str.find_last_not_of('0') == str.find('.')) { offset = 0; }
        str.erase(str.find_last_not_of('0') + offset, std::string::npos);
        return str;
    }

    template<typename T>
    std::string toString(const T & x_, const T & y_, const T & w_, const T & h_) const {
        std::string str = "{";
        str += to_string(x_);
        str += ", ";
        str += to_string(y_);
        str += ", ";
        str += to_string(w_);
        str += ", ";
        str += to_string(h_);
        str += "}";
        return str;
    }
public:
    enum POSITION_ENUM {
        MATCH_PARENT = -1
    };

    Position topLeft;
    Position bottomRight;

    constexpr inline
    Rectangle() : Rectangle(0, 0, MATCH_PARENT, MATCH_PARENT) {}

    constexpr inline
    Rectangle(const int & value) : Rectangle(value, value, value, value) {}

    constexpr inline
    Rectangle(const float & value) : Rectangle(value, value, value, value) {}

#define POSITION_CONSTRUCTOR(T1, T2, T3, T4) \
constexpr inline \
Rectangle(const T1 & topLeft_x, const T2 & topLeft_y, const T3 & bottomRight_x, \
         const T4 & bottomRight_y) : \
            topLeft(topLeft_x, topLeft_y), bottomRight(bottomRight_x, bottomRight_y) {}

    POSITION_CONSTRUCTOR(int, int, int, int)
    POSITION_CONSTRUCTOR(float, float, float, float)

    POSITION_CONSTRUCTOR(float, int, int, int)
    POSITION_CONSTRUCTOR(int, float, float, float)

    POSITION_CONSTRUCTOR(float, float, int, int)
    POSITION_CONSTRUCTOR(int, int, float, float)

    POSITION_CONSTRUCTOR(float, float, float, int)
    POSITION_CONSTRUCTOR(int, int, int, float)

    POSITION_CONSTRUCTOR(int, float, int, int)
    POSITION_CONSTRUCTOR(float, int, float, float)

    POSITION_CONSTRUCTOR(int, float, float, int)
    POSITION_CONSTRUCTOR(float, int, int, float)

    POSITION_CONSTRUCTOR(int, int, float, int)
    POSITION_CONSTRUCTOR(float, float, int, float)

    POSITION_CONSTRUCTOR(int, float, int, float)
    POSITION_CONSTRUCTOR(float, int, float, int)

#undef POSITION_CONSTRUCTOR

    inline
    std::string toString() const {
        return toString(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
    }

    friend std::ostream &operator<<(std::ostream &os, const Rectangle &position) {
        os << position.toString();
        return os;
    }

    inline
    std::pair<float, float> toPercentage() const {
        return {topLeft.x / bottomRight.x, topLeft.y / bottomRight.y};
    }

    inline
    bool isValid() const {
        return topLeft.x != bottomRight.x && topLeft.x != bottomRight.y
            && topLeft.x <= bottomRight.x && topLeft.y <= bottomRight.y;
    }

    inline
    bool isMatchParent() const {
        return bottomRight.x == MATCH_PARENT || bottomRight.y == MATCH_PARENT;
    }
    
    inline
    Rectangle resizeBy(const Rectangle & value) const {
        return {
            topLeft.x + value.topLeft.x,
            topLeft.y + value.topLeft.y,
            bottomRight.x - value.bottomRight.x,
            bottomRight.y - value.bottomRight.y
        };
    }

    inline
    Rectangle addPadding(const Rectangle & padding) const {
        return resizeBy(
            {
                padding.topLeft.x,
                padding.topLeft.y,
                -padding.bottomRight.x,
                -padding.bottomRight.y
            }
        );
    }

    inline
    Rectangle roundUpToInteger() const {
        return {
            roundf(topLeft.x),
            roundf(topLeft.y),
            roundf(bottomRight.x),
            roundf(bottomRight.y)
        };
    }

    inline
    Rectangle roundDownToInteger() const {
        return {
            roundf(topLeft.x - 0.5f),
            roundf(topLeft.y - 0.5f),
            roundf(bottomRight.x - 0.5f),
            roundf(bottomRight.y - 0.5f)
        };
    }

    inline
    const Rectangle operator+(const Rectangle &rhs) const {
        return {
            topLeft.x + rhs.topLeft.x,
            topLeft.y + rhs.topLeft.y,
            bottomRight.x + rhs.bottomRight.x,
            bottomRight.y + rhs.bottomRight.y,
        };
    }

    inline
    const Rectangle operator-(const Rectangle &rhs) const {
        return {
                topLeft.x - rhs.topLeft.x,
                topLeft.y - rhs.topLeft.y,
                bottomRight.x - rhs.bottomRight.x,
                bottomRight.y - rhs.bottomRight.y,
        };
    }

    inline
    const Rectangle operator*(const Rectangle &rhs) const {
        return {
                topLeft.x * rhs.topLeft.x,
                topLeft.y * rhs.topLeft.y,
                bottomRight.x * rhs.bottomRight.x,
                bottomRight.y * rhs.bottomRight.y,
        };
    }

    inline
    const Rectangle operator/(const Rectangle &rhs) const {
        return {
                topLeft.x / rhs.topLeft.x,
                topLeft.y / rhs.topLeft.y,
                bottomRight.x / rhs.bottomRight.x,
                bottomRight.y / rhs.bottomRight.y,
        };
    }

    inline
    Rectangle& operator+=(const Rectangle &rhs) {
        *this = *this + rhs;
        return *this;
    }

    inline
    Rectangle& operator-=(const Rectangle &rhs) {
        *this = *this - rhs;
        return *this;
    }

    inline
    Rectangle& operator*=(const Rectangle &rhs) {
        *this = *this * rhs;
        return *this;
    }

    inline
    Rectangle& operator/=(const Rectangle &rhs) {
        *this = *this / rhs;
        return *this;
    }

    inline
    bool operator==(const Rectangle &rhs) const {
        return topLeft.x == rhs.topLeft.x &&
               topLeft.y == rhs.topLeft.y &&
               bottomRight.x == rhs.bottomRight.x &&
               bottomRight.y == rhs.bottomRight.y;
    }

    inline
    bool operator!=(const Rectangle &rhs) const {
        return !(rhs == *this);
    }

    inline
    bool operator<(const Rectangle &rhs) const {
        if (topLeft.x < rhs.topLeft.x)
            return true;
        if (rhs.topLeft.x < topLeft.x)
            return false;
        if (topLeft.y < rhs.topLeft.y)
            return true;
        if (rhs.topLeft.y < topLeft.y)
            return false;
        if (bottomRight.x < rhs.bottomRight.x)
            return true;
        if (rhs.bottomRight.x < bottomRight.x)
            return false;
        return bottomRight.y < rhs.bottomRight.y;
    }

    inline
    bool operator>(const Rectangle &rhs) const {
        return rhs < *this;
    }

    inline
    bool operator<=(const Rectangle &rhs) const {
        return rhs == *this || (rhs < *this);
    }

    inline
    bool operator>=(const Rectangle &rhs) const {
        return rhs == *this || !(rhs < *this);
    }
    
    inline
    const Rectangle operator+(const int &rhs) const {
        return {
                topLeft.x + rhs,
                topLeft.y + rhs,
                bottomRight.x + rhs,
                bottomRight.y + rhs,
        };
    }

    inline
    const Rectangle operator-(const int &rhs) const {
        return {
                topLeft.x - rhs,
                topLeft.y - rhs,
                bottomRight.x - rhs,
                bottomRight.y - rhs,
        };
    }

    inline
    const Rectangle operator*(const int &rhs) const {
        return {
                topLeft.x * rhs,
                topLeft.y * rhs,
                bottomRight.x * rhs,
                bottomRight.y * rhs,
        };
    }

    inline
    const Rectangle operator/(const int &rhs) const {
        return {
                topLeft.x / rhs,
                topLeft.y / rhs,
                bottomRight.x / rhs,
                bottomRight.y / rhs,
        };
    }

    inline
    Rectangle& operator+=(const int &rhs) {
        *this = *this + rhs;
        return *this;
    }

    inline
    Rectangle& operator-=(const int &rhs) {
        *this = *this - rhs;
        return *this;
    }

    inline
    Rectangle& operator*=(const int &rhs) {
        *this = *this * rhs;
        return *this;
    }

    inline
    Rectangle& operator/=(const int &rhs) {
        *this = *this / rhs;
        return *this;
    }

    inline
    bool operator==(const int &rhs) const {
        return topLeft.x == rhs &&
               topLeft.y == rhs &&
               bottomRight.x == rhs &&
               bottomRight.y == rhs;
    }

    inline
    bool operator!=(const int &rhs) const {
        return !(*this == rhs);
    }

    inline
    bool operator<(const int &rhs) const {
        if (topLeft.x < rhs)
            return true;
        if (rhs < topLeft.x)
            return false;
        if (topLeft.y < rhs)
            return true;
        if (rhs < topLeft.y)
            return false;
        if (bottomRight.x < rhs)
            return true;
        if (rhs < bottomRight.x)
            return false;
        return bottomRight.y < rhs;
    }

    inline
    bool operator>(const int &rhs) const {
        if (topLeft.x > rhs)
            return true;
        if (rhs > topLeft.x)
            return false;
        if (topLeft.y > rhs)
            return true;
        if (rhs > topLeft.y)
            return false;
        if (bottomRight.x > rhs)
            return true;
        if (rhs > bottomRight.x)
            return false;
        return bottomRight.y > rhs;
    }

    inline
    bool operator<=(const int &rhs) const {
        return !(*this > rhs);
    }

    inline
    bool operator>=(const int &rhs) const {
        return !(*this < rhs);
    }

    inline
    const Rectangle operator+(const float &rhs) const {
        return {
                topLeft.x + rhs,
                topLeft.y + rhs,
                bottomRight.x + rhs,
                bottomRight.y + rhs,
        };
    }

    inline
    const Rectangle operator-(const float &rhs) const {
        return {
                topLeft.x - rhs,
                topLeft.y - rhs,
                bottomRight.x - rhs,
                bottomRight.y - rhs,
        };
    }

    inline
    const Rectangle operator*(const float &rhs) const {
        return {
                topLeft.x * rhs,
                topLeft.y * rhs,
                bottomRight.x * rhs,
                bottomRight.y * rhs,
        };
    }

    inline
    const Rectangle operator/(const float &rhs) const {
        return {
                topLeft.x / rhs,
                topLeft.y / rhs,
                bottomRight.x / rhs,
                bottomRight.y / rhs,
        };
    }

    inline
    Rectangle& operator+=(const float &rhs) {
        *this = *this + rhs;
        return *this;
    }

    inline
    Rectangle& operator-=(const float &rhs) {
        *this = *this - rhs;
        return *this;
    }

    inline
    Rectangle& operator*=(const float &rhs) {
        *this = *this * rhs;
        return *this;
    }

    inline
    Rectangle& operator/=(const float &rhs) {
        *this = *this / rhs;
        return *this;
    }

    inline
    bool operator==(const float &rhs) const {
        return topLeft.x == rhs &&
               topLeft.y == rhs &&
               bottomRight.x == rhs &&
               bottomRight.y == rhs;
    }

    inline
    bool operator!=(const float &rhs) const {
        return !(*this == rhs);
    }

    inline
    bool operator<(const float &rhs) const {
        if (topLeft.x < rhs)
            return true;
        if (rhs < topLeft.x)
            return false;
        if (topLeft.y < rhs)
            return true;
        if (rhs < topLeft.y)
            return false;
        if (bottomRight.x < rhs)
            return true;
        if (rhs < bottomRight.x)
            return false;
        return bottomRight.y < rhs;
    }

    inline
    bool operator>(const float &rhs) const {
        if (topLeft.x > rhs)
            return true;
        if (rhs > topLeft.x)
            return false;
        if (topLeft.y > rhs)
            return true;
        if (rhs > topLeft.y)
            return false;
        if (bottomRight.x > rhs)
            return true;
        if (rhs > bottomRight.x)
            return false;
        return bottomRight.y > rhs;
    }

    inline
    bool operator<=(const float &rhs) const {
        return !(*this > rhs);
    }

    inline
    bool operator>=(const float &rhs) const {
        return !(*this < rhs);
    }

    const Rectangle invert() const {
        return {
            -topLeft.x,
            -topLeft.y,
            -bottomRight.x,
            -bottomRight.y
        };
    }
};

#endif //GRAPHICAL_TOOL_KIT_RECTANGLE_H
