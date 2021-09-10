//
// Created by Matthew Good on 9/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_POSITION_H
#define GRAPHICAL_TOOL_KIT_POSITION_H

#include <string>
#include <ostream>
#include <cmath>

class Position {

    template<typename T>
    std::string to_string(const T & t) const {
        std::string str{std::to_string(t)};
        int offset{1};
        if (str.find_last_not_of('0') == str.find('.')) { offset = 0; }
        str.erase(str.find_last_not_of('0') + offset, std::string::npos);
        return str;
    }

    template<typename T>
    std::string toString(const T & x_, const T & y_) const {
        std::string str = "{";
        str += to_string(x_);
        str += ", ";
        str += to_string(y_);
        str += "}";
        return str;
    }
public:
    float x;
    float y;

    constexpr inline
    Position() noexcept : Position(0, 0) {}

    constexpr inline
    Position(const int & value) noexcept : Position(value, value) {}

    constexpr inline
    Position(const float & value) noexcept : Position(value, value) {}

#define POSITION_CONSTRUCTOR(T1, T2) \
constexpr inline \
Position(const T1 & x, const T2 & y) noexcept : x(x), y(y) {}

    POSITION_CONSTRUCTOR(int, int)
    POSITION_CONSTRUCTOR(float, float)

    POSITION_CONSTRUCTOR(float, int)
    POSITION_CONSTRUCTOR(int, float)

#undef POSITION_CONSTRUCTOR

    inline
    std::string toString() const {
        return toString(x, y);
    }

    friend std::ostream &operator<<(std::ostream &os, const Position &position) {
        os << position.toString();
        return os;
    }

    inline
    Position roundUpToInteger() const {
        return {
            roundf(x),
            roundf(y)
        };
    }

    inline
    Position roundDownToInteger() const {
        return {
            roundf(x - 0.5f),
            roundf(y - 0.5f)
        };
    }

    inline
    const Position operator+(const Position &rhs) const {
        return {
            x + rhs.x,
            y + rhs.y
        };
    }

    inline
    const Position operator-(const Position &rhs) const {
        return {
                x - rhs.x,
                y - rhs.y
        };
    }

    inline
    const Position operator*(const Position &rhs) const {
        return {
                x * rhs.x,
                y * rhs.y
        };
    }

    inline
    const Position operator/(const Position &rhs) const {
        return {
                x / rhs.x,
                y / rhs.y
        };
    }

    inline
    Position& operator+=(const Position &rhs) {
        *this = *this + rhs;
        return *this;
    }

    inline
    Position& operator-=(const Position &rhs) {
        *this = *this - rhs;
        return *this;
    }

    inline
    Position& operator*=(const Position &rhs) {
        *this = *this * rhs;
        return *this;
    }

    inline
    Position& operator/=(const Position &rhs) {
        *this = *this / rhs;
        return *this;
    }

    inline
    bool operator==(const Position &rhs) const {
        return x == rhs.x &&
               y == rhs.y;
    }

    inline
    bool operator!=(const Position &rhs) const {
        return !(rhs == *this);
    }

    inline
    bool operator<(const Position &rhs) const {
        if (x < rhs.x)
            return true;
        if (rhs.x < x)
            return false;
        if (y < rhs.y)
            return true;
        return !(rhs.y < y);
    }

    inline
    bool operator>(const Position &rhs) const {
        return !(rhs < *this);
    }

    inline
    bool operator<=(const Position &rhs) const {
        return rhs == *this || (rhs < *this);
    }

    inline
    bool operator>=(const Position &rhs) const {
        return rhs == *this || !(rhs < *this);
    }
    
    inline
    const Position operator+(const int &rhs) const {
        return {
                x + rhs,
                y + rhs
        };
    }

    inline
    const Position operator-(const int &rhs) const {
        return {
                x - rhs,
                y - rhs
        };
    }

    inline
    const Position operator*(const int &rhs) const {
        return {
                x * rhs,
                y * rhs
        };
    }

    inline
    const Position operator/(const int &rhs) const {
        return {
                x / rhs,
                y / rhs
        };
    }

    inline
    Position& operator+=(const int &rhs) {
        *this = *this + rhs;
        return *this;
    }

    inline
    Position& operator-=(const int &rhs) {
        *this = *this - rhs;
        return *this;
    }

    inline
    Position& operator*=(const int &rhs) {
        *this = *this * rhs;
        return *this;
    }

    inline
    Position& operator/=(const int &rhs) {
        *this = *this / rhs;
        return *this;
    }

    inline
    bool operator==(const int &rhs) const {
        return x == rhs &&
               y == rhs;
    }

    inline
    bool operator!=(const int &rhs) const {
        return !(*this == rhs);
    }

    inline
    bool operator<(const int &rhs) const {
        if (x < rhs)
            return true;
        if (rhs < x)
            return false;
        if (y < rhs)
            return true;
        return !(rhs < y);
    }

    inline
    bool operator>(const int &rhs) const {
        return !(*this < rhs);
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
    const Position operator+(const float &rhs) const {
        return {
                x + rhs,
                y + rhs
        };
    }

    inline
    const Position operator-(const float &rhs) const {
        return {
                x - rhs,
                y - rhs
        };
    }

    inline
    const Position operator*(const float &rhs) const {
        return {
                x * rhs,
                y * rhs
        };
    }

    inline
    const Position operator/(const float &rhs) const {
        return {
                x / rhs,
                y / rhs
        };
    }

    inline
    Position& operator+=(const float &rhs) {
        *this = *this + rhs;
        return *this;
    }

    inline
    Position& operator-=(const float &rhs) {
        *this = *this - rhs;
        return *this;
    }

    inline
    Position& operator*=(const float &rhs) {
        *this = *this * rhs;
        return *this;
    }

    inline
    Position& operator/=(const float &rhs) {
        *this = *this / rhs;
        return *this;
    }

    inline
    bool operator==(const float &rhs) const {
        return x == rhs &&
               y == rhs;
    }

    inline
    bool operator!=(const float &rhs) const {
        return !(*this == rhs);
    }

    inline
    bool operator<(const float &rhs) const {
        if (x < rhs)
            return true;
        if (rhs < x)
            return false;
        if (y < rhs)
            return true;
        return !(rhs < y);
    }

    inline
    bool operator>(const float &rhs) const {
        return !(*this < rhs);
    }

    inline
    bool operator<=(const float &rhs) const {
        return !(*this > rhs);
    }

    inline
    bool operator>=(const float &rhs) const {
        return !(*this < rhs);
    }

    inline
    const Position invert() const {
        return {
            -x,
            -y
        };
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
    inline
    Position withX(const int & x) const {
        return {x, y};
    }

    inline
    Position withX(const float & x) const {
        return {x, y};
    }

    inline
    Position withY(const int & y) const {
        return {x, y};
    }

    inline
    Position withY(const float & y) const {
        return {x, y};
    }
#pragma clang diagnostic pop
};

#endif //GRAPHICAL_TOOL_KIT_POSITION_H
