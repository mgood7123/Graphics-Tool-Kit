//
// Created by Matthew Good on 17/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_VERTEXENGINE_H
#define GRAPHICAL_TOOL_KIT_VERTEXENGINE_H

#include "PixelToNDC.h"
#include <SDK/include/Windows/Kernel/WindowsAPIKernel.h>
#include <vector>

class VertexEngine {
    template <typename data_type, typename storage_type>
    class Buffer {
        Kernel kernel = Kernel(8);
    public:
        HANDLE add(data_type data);

        storage_type * get(HANDLE handle);

        void remove(HANDLE handle);

        void clear();

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
    uint32_t indexPosition;

    int width;
    int height;
    PixelToNDC pixelConverter;
public:
    PixelToNDC::Coordinates<float> toNDC(int x, int y, int z);
    VertexEngine();
    VertexEngine(int width, int height);
    ~VertexEngine();
    void resize(int width, int height);

    /**
     * clears the vertex and index buffers
     */
    void clear();

    HANDLE defaultPositionBuffer;
    HANDLE defaultColorBuffer;

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
     * specifies the buffer order for output generation
     * @param data an ordered array of HANDLE's
     */
    void order(const std::vector<HANDLE>& data);

    class GenerationInfo {
        size_t chunkReader;
    public:

        size_t chunksGenerated;

        const size_t lengthData;
        const size_t sizeInBytesData;
        float * data;

        const size_t lengthIndices;
        const size_t sizeInBytesIndices;
        uint32_t * indices;

        GenerationInfo(
                const size_t lengthData, const float *data,
                const size_t lengthIndices, const uint32_t *indices
        );


        GenerationInfo(const GenerationInfo& m) = delete;

        GenerationInfo& operator=(const GenerationInfo& m) = delete;

        GenerationInfo(GenerationInfo && m);

        GenerationInfo& operator=(GenerationInfo&& m) = delete;

        ~GenerationInfo();

        static bool isMultipleOf(size_t bytes, size_t dataSize, size_t multipleOf);

        void resetChunkReader();

        bool canGenerateChunk(size_t chunkSize) const;

        GenerationInfo generateChunk(int posCount, int colorCount, size_t chunkSize);
    };

    /**
     * generates a vertex buffer for use in OpenGL
     * @return information about the generated data
     */
    GenerationInfo generateGL();

    // shape and geometry generation

    class Canvas {
        VertexEngine * vertexEngine = nullptr;

        typedef std::pair<HANDLE, const std::vector<float>&> Data;

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
        HANDLE addData_(HANDLE dataBufferHandle, const std::vector<float>& data);

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
        std::vector<HANDLE> addData_(const std::vector<Data>& handle_and_data);

        /**
         * adds index data
         * <br>
         * data is given as unsigned 32-bit integers
         * @return a handle to the added data
         */
        HANDLE addIndexData_(const std::vector<uint32_t>& data);
    public:
        class Position3 {
        public:
            float x;
            float y;
            float z;

            Position3(const std::array<float, 3> & data);
            Position3(const float & x, const float & y, const float & z);
            std::array<float, 3> toArray() const;
            std::vector<float> toVector() const;
        };

        class Color4 {
        public:
            float r;
            float g;
            float b;
            float a;

            Color4(const std::array<float, 4> & data);
            Color4(const float & r, const float & g, const float & b, const float & a);
            std::array<float, 4> toArray() const;
            std::vector<float> toVector() const;
        };

        const int x;
        const int y;
        const int width;
        const int height;

        Canvas(VertexEngine * engine, int x, int y, int width, int height);
        Canvas(VertexEngine * engine, int width, int height);

        Canvas subCanvas(int x, int y, int width, int height);

        /**
         * adds position and color data
         * <br>
         * data is given as floating-point
         */
        void addData(const std::vector<std::pair<const Position3&, const Color4&>>& data);

        /**
         * adds index data
         * <br>
         * data is given as unsigned 32-bit integers
         * @return a handle to the added data
         */
        HANDLE addIndexData(const std::vector<uint32_t>& data);

        static Color4 black;

        /**
         * alias for fill(black);
         */
        void clear();

        /**
         * alias for fill(color)
         */
        void clear(Color4 color);

        void fill(Color4 color);

        void planeAt(int from_X, int from_Y, int to_X, int to_Y, const Color4& colorData);

        void plane(int x, int y, int width, int height, const Color4& colorData);
    };
};


#endif //GRAPHICAL_TOOL_KIT_VERTEXENGINE_H
