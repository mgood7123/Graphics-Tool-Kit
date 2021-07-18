//
// Created by Matthew Good on 17/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_VERTEXENGINE_H
#define GRAPHICAL_TOOL_KIT_VERTEXENGINE_H

#include "PixelToNDC.h"
#include <SDK/include/Windows/Kernel/WindowsAPIKernel.h>

class VertexEngine {
    template <typename data_type, typename storage_type>
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
        Buffer<const std::vector<float>&, std::vector<float>> dataBuffer;
        size_t length;
        bool is_static;
        HANDLE static_data = nullptr;
        VertexInfo(size_t length, bool is_static);
    };


    Buffer<VertexInfo, VertexInfo> vertexBuffer;
    std::vector<HANDLE> vertexBufferOrder;
    std::vector<HANDLE> indexHandles;
    Buffer<const std::vector<uint32_t>&, std::vector<uint32_t>> indexBuffer;
    uint32_t indexPosition = 0;

    int width;
    int height;
    PixelToNDC pixelConverter;
public:
    PixelToNDC::Coordinates<float> toNDC(int x, int y, int z);
    VertexEngine(int width, int height);
    void resize(int width, int height);

    HANDLE defaultPositionBuffer = nullptr;
    HANDLE defaultColorBuffer = nullptr;

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
     * data is given as floating-point
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
     * @return a handle to the added data
     */
    HANDLE addData(HANDLE dataBufferHandle, const std::vector<float>& data);

    /**
     * adds data to the specified dataBufferHandle
     * <br>
     * data is given as sets of dataBufferHandle and floating-point
     * <br>
     * for each dataBufferHandle and floating-point pair:
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
     * @return a vector of handles to the added data
     */
    std::vector<HANDLE> addData(const std::vector<std::pair<HANDLE, const std::vector<float>&>>& handle_and_data);

    /**
     * adds index data to the specified dataBufferHandle
     * <br>
     * data is given as unsigned 32-bit integers
     * @return a handle to the added data
     */
    HANDLE addIndexData(const std::vector<uint32_t>& data);

    /**
     * specifies the buffer order for output generation
     * @param data an ordered array of HANDLE's
     */
    void order(const std::vector<HANDLE>& data);

    class GenerationInfo {
    public:
        size_t lengthData;
        size_t sizeInBytesData;
        float * data;

        size_t lengthIndices;
        size_t sizeInBytesIndices;
        uint32_t * indices;

        GenerationInfo(
                const size_t lengthData, const float *data,
                const size_t lengthIndices, const uint32_t *indices
        );

        ~GenerationInfo();
    };

    /**
     * generates a vertex buffer for use in OpenGL
     * @return information about the generated data
     */
    GenerationInfo generateGL();

    // shape and geometry generation

    void plane(int x, int y, int width, int height, const std::vector<float>& colorData);
    void planeAt(int from_X, int from_Y, int to_X, int to_Y, const std::vector<float>& colorData);
};


#endif //GRAPHICAL_TOOL_KIT_VERTEXENGINE_H
