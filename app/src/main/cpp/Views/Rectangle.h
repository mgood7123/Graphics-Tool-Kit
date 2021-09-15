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

    Position topLeft;
    Position bottomRight;

    constexpr inline
    Rectangle() noexcept : Rectangle(0, 0, 0, 0) {}

    constexpr inline
    Rectangle(const int & value) noexcept : Rectangle(value, value, value, value) {}

    constexpr inline
    Rectangle(const float & value) noexcept : Rectangle(value, value, value, value) {}

    constexpr inline
    Rectangle(const Position & topLeft, const Position & bottomRight) noexcept :
            Rectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y) {}

    constexpr inline
    Rectangle(const int & topLeft_x, const int & topLeft_y, const Position & bottomRight) noexcept :
    Rectangle(topLeft_x, topLeft_y, bottomRight.x, bottomRight.y) {}

    constexpr inline
    Rectangle(const float & topLeft_x, const float & topLeft_y, const Position & bottomRight) noexcept :
            Rectangle(topLeft_x, topLeft_y, bottomRight.x, bottomRight.y) {}

    constexpr inline
    Rectangle(const float & topLeft_x, const int & topLeft_y, const Position & bottomRight) noexcept :
            Rectangle(topLeft_x, topLeft_y, bottomRight.x, bottomRight.y) {}

    constexpr inline
    Rectangle(const int & topLeft_x, const float & topLeft_y, const Position & bottomRight) noexcept :
            Rectangle(topLeft_x, topLeft_y, bottomRight.x, bottomRight.y) {}

    constexpr inline
    Rectangle(const Position & topLeft, const int & bottomRight_x, const int & bottomRight_y) noexcept :
            Rectangle(topLeft.x, topLeft.y, bottomRight_x, bottomRight_y) {}

    constexpr inline
    Rectangle(const Position & topLeft, const float & bottomRight_x, const float & bottomRight_y) noexcept :
            Rectangle(topLeft.x, topLeft.y, bottomRight_x, bottomRight_y) {}

    constexpr inline
    Rectangle(const Position & topLeft, const float & bottomRight_x, const int & bottomRight_y) noexcept :
            Rectangle(topLeft.x, topLeft.y, bottomRight_x, bottomRight_y) {}

    constexpr inline
    Rectangle(const Position & topLeft, const int & bottomRight_x, const float & bottomRight_y) noexcept :
            Rectangle(topLeft.x, topLeft.y, bottomRight_x, bottomRight_y) {}

#define POSITION_CONSTRUCTOR(T1, T2, T3, T4) \
constexpr inline \
Rectangle(const T1 & topLeft_x, const T2 & topLeft_y, const T3 & bottomRight_x, \
         const T4 & bottomRight_y) noexcept : \
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
    
    inline Position size() const {
        return {bottomRight - topLeft};
    }

    inline
    Rectangle resizeBy(const Rectangle & value) const {
        return {
            // if this = 10, 10, 20, 20
            // resize value = 0, 0, 5, 5, result = 10, 10, 15, 15
            // resize value = 0, 0, -5, -5, result = 10, 10, 25, 25
            // resize value = 5, 5, 0, 0, result = 15, 15, 20, 20
            // resize value = 5, 5, 5, 5, result = 15, 15, 15, 15
            // resize value = 5, 5, -5, -5, result = 15, 15, 25, 25
            topLeft.x + value.topLeft.x,
            topLeft.y + value.topLeft.y,
            bottomRight.x - value.topLeft.x - value.bottomRight.x,
            bottomRight.y - value.topLeft.y - value.bottomRight.y
        };
    }

    inline
    Rectangle addPadding(const Rectangle & padding) const {
        return resizeBy(
            {
                padding.topLeft.x,
                padding.topLeft.y,
                -padding.topLeft.x - -padding.bottomRight.x,
                -padding.topLeft.y - -padding.bottomRight.y
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
    const Rectangle operator+(const Position &rhs) const {
        return {
                topLeft.x + rhs.x,
                topLeft.y + rhs.y,
                bottomRight.x + rhs.x,
                bottomRight.y + rhs.y,
        };
    }

    inline
    const Rectangle operator-(const Position &rhs) const {
        return {
                topLeft.x - rhs.x,
                topLeft.y - rhs.y,
                bottomRight.x - rhs.x,
                bottomRight.y - rhs.y,
        };
    }

    inline
    const Rectangle operator*(const Position &rhs) const {
        return {
                topLeft.x * rhs.x,
                topLeft.y * rhs.y,
                bottomRight.x * rhs.x,
                bottomRight.y * rhs.y,
        };
    }

    inline
    const Rectangle operator/(const Position &rhs) const {
        return {
                topLeft.x / rhs.x,
                topLeft.y / rhs.y,
                bottomRight.x / rhs.x,
                bottomRight.y / rhs.y,
        };
    }

    inline
    Rectangle& operator+=(const Position &rhs) {
        *this = *this + rhs;
        return *this;
    }

    inline
    Rectangle& operator-=(const Position &rhs) {
        *this = *this - rhs;
        return *this;
    }

    inline
    Rectangle& operator*=(const Position &rhs) {
        *this = *this * rhs;
        return *this;
    }

    inline
    Rectangle& operator/=(const Position &rhs) {
        *this = *this / rhs;
        return *this;
    }

    inline
    bool operator==(const Position &rhs) const {
        return topLeft.x == rhs.x &&
               topLeft.y == rhs.y &&
               bottomRight.x == rhs.x &&
               bottomRight.y == rhs.y;
    }

    inline
    bool operator!=(const Position &rhs) const {
        return !(rhs == topLeft && rhs == bottomRight);
    }

    inline
    bool operator<(const Position &rhs) const {
        if (topLeft.x < rhs.x)
            return true;
        if (rhs.x < topLeft.x)
            return false;
        if (topLeft.y < rhs.y)
            return true;
        if (rhs.y < topLeft.y)
            return false;
        if (bottomRight.x < rhs.x)
            return true;
        if (rhs.x < bottomRight.x)
            return false;
        return bottomRight.y < rhs.y;
    }

    inline
    bool operator>(const Position &rhs) const {
        return rhs < topLeft && rhs < bottomRight;
    }

    inline
    bool operator<=(const Position &rhs) const {
        return (rhs == topLeft && rhs == bottomRight) || (rhs < topLeft && rhs < bottomRight);
    }

    inline
    bool operator>=(const Position &rhs) const {
        return (rhs == topLeft && rhs == bottomRight) || !(rhs < topLeft && rhs < bottomRight);
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
    inline
    Rectangle withTopLeft(const Position & topLeft) const {
        return {topLeft, bottomRight};
    }

    inline
    Rectangle withBottomRight(const Position & bottomRight) const {
        return {topLeft, bottomRight};
    }
#pragma clang diagnostic pop

    inline
    Rectangle withTopLeft(const int & x, const int & y) const {
        return {{x, y}, bottomRight};
    }

    inline
    Rectangle withTopLeft(const float & x, const int & y) const {
        return {{x, y}, bottomRight};
    }

    inline
    Rectangle withTopLeft(const int & x, const float & y) const {
        return {{x, y}, bottomRight};
    }

    inline
    Rectangle withTopLeft(const float & x, const float & y) const {
        return {{x, y}, bottomRight};
    }

    inline
    Rectangle withBottomRight(const int & x, const int & y) const {
        return {topLeft, {x, y}};
    }

    inline
    Rectangle withBottomRight(const float & x, const int & y) const {
        return {topLeft, {x, y}};
    }

    inline
    Rectangle withBottomRight(const int & x, const float & y) const {
        return {topLeft, {x, y}};
    }

    inline
    Rectangle withBottomRight(const float & x, const float & y) const {
        return {topLeft, {x, y}};
    }

    inline
    Rectangle withTopLeftX(const int & x) const {
        return withTopLeft(topLeft.withX(x));
    }

    inline
    Rectangle withTopLeftX(const float & x) const {
        return withTopLeft(topLeft.withX(x));
    }

    inline
    Rectangle withTopLeftY(const int & y) const {
        return withTopLeft(topLeft.withY(y));
    }

    inline
    Rectangle withTopLeftY(const float & y) const {
        return withTopLeft(topLeft.withY(y));
    }

    inline
    Rectangle withBottomRightX(const int & x) const {
        return withBottomRight(bottomRight.withX(x));
    }

    inline
    Rectangle withBottomRightX(const float & x) const {
        return withBottomRight(bottomRight.withX(x));
    }

    inline
    Rectangle withBottomRightY(const int & y) const {
        return withBottomRight(bottomRight.withY(y));
    }

    inline
    Rectangle withBottomRightY(const float & y) const {
        return withBottomRight(bottomRight.withY(y));
    }

};

#endif //GRAPHICAL_TOOL_KIT_RECTANGLE_H
