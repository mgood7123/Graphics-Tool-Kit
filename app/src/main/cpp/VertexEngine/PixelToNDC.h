//
// Created by Matthew Good on 17/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_PIXEL_TO_NDC_H
#define GRAPHICAL_TOOL_KIT_PIXEL_TO_NDC_H

#include <DiligentCore/Primitives/interface/Errors.hpp>

class PixelToNDC {
public:
    bool LOG_PRINT_CONVERSIONS = true;

    template<typename TYPE> float inverse(TYPE num) {
        return num < 0 ? -num : -(num);
    }

    template<typename TYPE> float convert(TYPE num, TYPE num_max) {
        // 3 year old magic
        return (num - (num_max / 2)) / (num_max / 2);
    }

    template<typename TYPE> class coordinates {
    public:
        coordinates() {}
        coordinates(TYPE TYPE_INITIALIZER) {
            TYPE x = TYPE_INITIALIZER;
            TYPE y = TYPE_INITIALIZER;
        }

        TYPE x;
        TYPE y;
    };

    constexpr static int CONVERSION_ORIGIN_TOP_LEFT = 0;
    constexpr static int CONVERSION_ORIGIN_TOP_RIGHT = 1;
    constexpr static int CONVERSION_ORIGIN_BOTTOM_LEFT = 2;
    constexpr static int CONVERSION_ORIGIN_BOTTOM_RIGHT = 3;

    int CONVERSION_ORIGIN = CONVERSION_ORIGIN_TOP_LEFT;

    template<typename TYPEFROM, typename TYPETO>
    class coordinates<TYPETO> pixel_location_to_normalized_device_coordinate(TYPETO TYPETO_INITIALIZER, TYPEFROM x, TYPEFROM y, TYPEFROM w, TYPEFROM h, bool clip) {
        class coordinates<TYPETO> xy(TYPETO_INITIALIZER);
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
        xy.x = convert<TYPETO>(static_cast<TYPETO>(x), static_cast<TYPETO>(w)); // x
        xy.y = convert<TYPETO>(static_cast<TYPETO>(y), static_cast<TYPETO>(h)); // y

        switch (CONVERSION_ORIGIN) {
            case CONVERSION_ORIGIN_TOP_LEFT:
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("inverting 'y'");
                xy.y = inverse<TYPETO>(xy.y);
                break;
            case CONVERSION_ORIGIN_TOP_RIGHT:
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("inverting 'x'");
                xy.x = inverse<TYPETO>(xy.x);
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("inverting 'y'");
                xy.y = inverse<TYPETO>(xy.y);
                break;
            case CONVERSION_ORIGIN_BOTTOM_LEFT: {
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("no conversion");
                break;
            }
            case CONVERSION_ORIGIN_BOTTOM_RIGHT:
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("inverting 'x'");
                xy.x = inverse<TYPETO>(xy.x);
                break;
            default:
                if (LOG_PRINT_CONVERSIONS) LOG_INFO_MESSAGE("unknown conversion");
                break;
        }
        if (LOG_PRINT_CONVERSIONS) {
            LOG_INFO_MESSAGE("CONVERSION INFO:",
                             "\nx: ", x, ", y: ", y,
                             "\nwidth: ", w, ", height: ", h,
                             "\nNormalized Device Coordinates: ", xy.x, ", ", xy.y
            );
        }
        return xy;
    }

    template<typename TYPEFROM, typename TYPETO>
    class coordinates<TYPETO> pixel_location_to_normalized_device_coordinate(TYPEFROM x, TYPEFROM y, TYPEFROM w, TYPEFROM h) {
        return pixel_location_to_normalized_device_coordinate<TYPEFROM, TYPETO>(0, x, y, w, h, true);
    }
};


#endif //GRAPHICAL_TOOL_KIT_PIXEL_TO_NDC_H
