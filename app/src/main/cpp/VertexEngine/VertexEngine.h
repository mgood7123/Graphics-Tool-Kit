//
// Created by Matthew Good on 17/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_VERTEXENGINE_H
#define GRAPHICAL_TOOL_KIT_VERTEXENGINE_H

#include "PixelToNDC.h"
#include <SDK/include/Windows/Kernel/WindowsAPIKernel.h>

class VertexEngine {
    template <typename storage_type, typename data_type>
    class Buffer {
        Kernel kernel;
    public:
        HANDLE add(data_type data);

        storage_type * get(HANDLE handle);

        void remove(HANDLE handle);

        Table::Iterator getIterator();
    };

    class VertexInfo {
    public:
        Buffer<std::initializer_list<float>, std::initializer_list<float>> dataBuffer;
        size_t length;
        bool is_static;
        HANDLE static_data = nullptr;
        VertexInfo(size_t length, bool is_static);
    };

    Buffer<VertexInfo, VertexInfo> vertexBuffer;
    std::initializer_list<HANDLE> vertexBufferOrder;

    int width;
    int height;
    PixelToNDC pixelConverter;
public:
    VertexEngine(int width, int height);
    void resize(int width, int height);

    /**
     * creates a new non-static data buffer
     * <br>
     * a buffer that is non-static means data
     * must be supplied for each ordered use
     * <br>
     * @param length the length of the data buffer
     * @return a handle to the buffer
     */
    HANDLE addDataBuffer(size_t length);

    /**
     * creates a new static data buffer
     * <br>
     * a buffer that is static means there is only one data set
     * and this data will be reused when needed
     * <br>
     * @param length the length of the data buffer
     * @return a handle to the buffer
     */
    HANDLE addDataBufferStatic(size_t length);

    /**
     * destroys an existing data buffer
     * @param handle a handle to the data buffer to remove
     */
    void removeDataBuffer(HANDLE handle);

    /**
     * adds data to the specified dataBufferHandle
     * <br>
     * <br>
     * if the dataBufferHandle is a static buffer, then:
     * <br>
     * <br>
     * if data does not exist it will be added
     * <br>
     * if data already exists it will be overwritten
     * <br>
     * <br>
     * otherwise, if the dataBufferHandle is a non static buffer, then
     * data will simply be added
     * <br>
     * <br>
     * data is given in as type float
     * @return a handle to the added data
     */
    HANDLE addData(HANDLE dataBufferHandle, std::initializer_list<float> data);

    /**
     * specifies the buffer order for output generation
     * @param data an ordered array of HANDLE's
     */
    void order(std::initializer_list<HANDLE> data);

    class GenerationInfo {
    public:
        size_t length;
        size_t sizeInBytes;
        float * data;

        GenerationInfo(const size_t length, const float *data);

        ~GenerationInfo();
    };

    /**
     * generates a vertex buffer for use in OpenGL
     * @return information about the generated data
     */
    GenerationInfo generateGL();
};


#endif //GRAPHICAL_TOOL_KIT_VERTEXENGINE_H
