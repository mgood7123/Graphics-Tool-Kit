//
// Created by Matthew Good on 17/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_VERTEXENGINE_H
#define GRAPHICAL_TOOL_KIT_VERTEXENGINE_H

#include "PixelToNDC.h"
#include <SDK/include/Windows/Kernel/WindowsAPIKernel.h>
#include <vector>
#include <DiligentTools/TextureLoader/interface/TextureLoader.h>

class VertexEngine {
public:
    constexpr static float FLOAT_FALSE = 0;
    constexpr static float FLOAT_TRUE = 1;

    template<typename A, typename B, typename C>
    class Triple {
    public:
        A first;
        B second;
        C third;

        Triple(A first, B second, C third);
    };

private:
    template <typename data_type, typename storage_type>
    class Buffer {
        Kernel kernel = Kernel(8);
    public:
        HANDLE add(data_type data);

        storage_type * get(HANDLE handle);
        storage_type * get(const size_t & index);
        size_t getIndex(HANDLE handle);

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
    Buffer<
        Triple<const char *, size_t, Diligent::ITexture*>,
        Triple<const char *, size_t, Diligent::ITexture*>
    > textureBuffer;
    std::vector<HANDLE> vertexBufferOrder;
    std::vector<HANDLE> indexHandles;
    Buffer<const std::vector<uint32_t>&, std::vector<uint32_t>> indexBuffer;
    uint32_t indexPosition;

    int width;
    int height;
    PixelToNDC pixelConverter;



    // until we can figure out how to make these customizable, we need
    // to have these as private due to output generation being dependant
    // on the color buffer configuration

    HANDLE defaultPositionBuffer;
    HANDLE defaultColorBuffer;
    HANDLE defaultTextureCoordinateBuffer;

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

public:
    constexpr static int positionCount = 3;
    constexpr static int colorCount = 4;
    constexpr static int colorExtraCount = 2;
    constexpr static int textureCoordinatesCount = 2;
    constexpr static int strideLength = 3+5+2;

    PixelToNDC::Coordinates<float> toNDC(int x, int y, int z);
    VertexEngine();
    VertexEngine(int width, int height);
    ~VertexEngine();
    void resize(int width, int height);

    /**
     * clears the vertex and index buffers
     */
    void clear();

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

        bool isTexture;
        size_t textureIndex;

        GenerationInfo(
                const size_t lengthData, const float *data,
                const size_t lengthIndices, const uint32_t *indices
        );

        GenerationInfo(
                const size_t lengthData, const float *data,
                const size_t lengthIndices, const uint32_t *indices,
                const bool isTexture
        );

        GenerationInfo(const GenerationInfo& m) = delete;

        GenerationInfo& operator=(const GenerationInfo& m) = delete;

        GenerationInfo(GenerationInfo && m);

        GenerationInfo& operator=(GenerationInfo&& m) = delete;

        ~GenerationInfo();

        static bool isMultipleOf(size_t bytes, size_t dataSize, size_t multipleOf);

        void resetChunkReader();

        bool canGenerateChunk(size_t chunkSize) const;

        GenerationInfo generateChunk(size_t chunkSize);
    };

    /**
     * generates a vertex buffer for use in OpenGL
     * @return information about the generated data
     */
    GenerationInfo generateGL();

    class TextureManager {
        VertexEngine * vertexEngine;
    public:
        TextureManager(VertexEngine * engine);

        /**
         * creates a texture from the specified path
         * @param key the key that will be used to locate the texture
         * @param FilePath Source file path
         * @param TexLoadInfo Texture loading information
         * @param pDevice Render device that will be used to create the texture
         */
        void createTextureFromFile(const char *key, const Diligent::Char *FilePath,
                                   const Diligent::TextureLoadInfo &TexLoadInfo,
                                   Diligent::IRenderDevice *pDevice);

        /**
         * finds a texture associated with the key used to create the texture
         * @param key the key to locate the texture
         * @return nullptr if the key cannot be found
         */
        Diligent::ITexture *findTexture(const char *key);

        /**
         * obtains a texture associated with the index returned by findTextureIndex
         * @param index the index returned by findTextureIndex
         * @return nullptr if the key cannot be found
         */
        Triple<const char *, size_t, Diligent::ITexture *> * getTexture(const size_t & index);

        /**
         * finds a texture associated with the key used to create the texture
         * @param key the key to locate the texture
         * @return -1 if the texture cannot be found
         */
        size_t findTextureIndex(const char *key);

        /**
         * deletes a texture associated with the key used to create the texture
         * @param key the key to locate the texture
         */
        void deleteTexture(const char *key);
    };

    TextureManager textureManager;

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
        class Position2 {
        public:
            float x;
            float y;

            Position2(const std::array<float, 2> & data);
            Position2(const std::array<float, 3> & data);
            Position2(const float & x, const float & y);
            std::array<float, 2> toArray() const;
            std::vector<float> toVector() const;
        };

        typedef Position2 TextureCoordinate2;

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
            const char * key;
            float isTexture;
            float textureResource;

            Color4(const std::array<float, 4> & data);
            Color4(const std::array<float, 6> & data);
            Color4(const float & r, const float & g, const float & b, const float & a);
            Color4(const char * key);
            std::array<float, 4> toRGBAArray() const;
            std::array<float, 6> toArray() const;
            std::vector<float> toRGBAVector() const;
            std::vector<float> toVector() const;
        };

    private:
        void processTextureInColor4(const Color4& colorData);

    public:

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
         * <br>
         * position coordinates { 0, 0 } is top left, { width, height } is bottom right
         * <br>
         * texture coordinates { 0, 0 } is bottom left, { 1, 1 } is top right
         */
        void addData(const std::vector<Triple<const Position3&, const Color4&, const TextureCoordinate2&>>& data);

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
