//
// Created by Matthew Good on 17/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_PIXEL_TO_NDC_H
#define GRAPHICAL_TOOL_KIT_PIXEL_TO_NDC_H

#include <MultiTouch/DiligentLog/Primitives/interface/Errors.hpp>
#include <array>

class PixelToNDC {
public:
    bool LOG_PRINT_CONVERSIONS = false;

    template<typename TYPE> float inverse(TYPE num) {
        return num < 0 ? -num : -(num);
    }

    template<typename TYPE> float convert(TYPE num, TYPE num_max) {
        // 3 year old magic
        return (num - (num_max / 2)) / (num_max / 2);
    }

    template<typename TYPE> class Coordinates {
    public:
        TYPE x;
        TYPE y;
        TYPE z;
        Coordinates() {}
        Coordinates(TYPE TYPE_INITIALIZER) {
            x = TYPE_INITIALIZER;
            y = TYPE_INITIALIZER;
            z = TYPE_INITIALIZER;
        }

        std::array<TYPE, 3> to_array() {
            return {x, y, z};
        };
    };

    static const int CONVERSION_ORIGIN_TOP_LEFT = 0;
    static const int CONVERSION_ORIGIN_TOP_RIGHT = 1;
    static const int CONVERSION_ORIGIN_BOTTOM_LEFT = 2;
    static const int CONVERSION_ORIGIN_BOTTOM_RIGHT = 3;

    int CONVERSION_ORIGIN = CONVERSION_ORIGIN_TOP_LEFT;

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPETO TYPETO_INITIALIZER, TYPEFROM x, TYPEFROM y, TYPEFROM z, TYPEFROM w, TYPEFROM h, bool clip) {
        Coordinates<TYPETO> xyz(TYPETO_INITIALIZER);
        xyz.z = z;
        if (x > w) {
            if (LOG_PRINT_CONVERSIONS)
                LOG_INFO_MESSAGE("x is out of bounds (expected ", w, ", got ", x, ")");
            if (clip) {
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("clipping to ", w);
                x = w;
            }
        } else if (x < 0) {
            if (LOG_PRINT_CONVERSIONS)
                LOG_INFO_MESSAGE("x is out of bounds (expected 0, got ", x, ")");
            if (clip) {
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("clipping to 0");
                x = 0;
            }
        }
        if (y > h) {
            if (LOG_PRINT_CONVERSIONS)
                LOG_INFO_MESSAGE("y is out of bounds (expected ", h, ", got ", y, ")");
            if (clip) {
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("clipping to ", h);
                y = h;
            }
        } else if (y < 0) {
            if (LOG_PRINT_CONVERSIONS)
                LOG_INFO_MESSAGE("y is out of bounds (expected 0, got ", y, ")");
            if (clip) {
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("clipping to 0");
                y = 0;
            }
        }
        xyz.x = convert<TYPETO>(static_cast<TYPETO>(x), static_cast<TYPETO>(w)); // x
        xyz.y = convert<TYPETO>(static_cast<TYPETO>(y), static_cast<TYPETO>(h)); // y

        switch (CONVERSION_ORIGIN) {
            case CONVERSION_ORIGIN_TOP_LEFT:
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("inverting 'y'");
                xyz.y = inverse<TYPETO>(xyz.y);
                break;
            case CONVERSION_ORIGIN_TOP_RIGHT:
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("inverting 'x'");
                xyz.x = inverse<TYPETO>(xyz.x);
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("inverting 'y'");
                xyz.y = inverse<TYPETO>(xyz.y);
                break;
            case CONVERSION_ORIGIN_BOTTOM_LEFT: {
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("no conversion");
                break;
            }
            case CONVERSION_ORIGIN_BOTTOM_RIGHT:
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("inverting 'x'");
                xyz.x = inverse<TYPETO>(xyz.x);
                break;
            default:
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("unknown conversion");
                break;
        }
        if (LOG_PRINT_CONVERSIONS) {
            LOG_INFO_MESSAGE("CONVERSION INFO:",
                             "\nx: ", x, ", y: ", y, ", z: ", z,
                             "\nwidth: ", w, ", height: ", h,
                             "\nNormalized Device Coordinates: ", xyz.x, ", ", xyz.y, ", ", xyz.z
            );
        }
        return xyz;
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPETO TYPETO_INITIALIZER, TYPEFROM x, TYPEFROM y, TYPEFROM w, TYPEFROM h, bool clip) {
        return toNDC<TYPEFROM, TYPETO>(TYPETO_INITIALIZER, x, y, 0, w, h, clip);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPEFROM x, TYPEFROM y, TYPEFROM w, TYPEFROM h) {
        return toNDC<TYPEFROM, TYPETO>(0, x, y, 0, w, h, true);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPEFROM x, TYPEFROM y, TYPEFROM z, TYPEFROM w, TYPEFROM h) {
        return toNDC<TYPEFROM, TYPETO>(0, x, y, z, w, h, true);
    }
};


#endif //GRAPHICAL_TOOL_KIT_PIXEL_TO_NDC_H
