//
// Created by Matthew Good on 17/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_VERTEXENGINE_H
#define GRAPHICAL_TOOL_KIT_VERTEXENGINE_H

#include <PixelToNDC/PixelToNDC.h>
#include <vector>
#include <WINAPI_KERNEL/SDK/include/Windows/Kernel/WindowsAPIKernel.h>
#include <DiligentTools/TextureLoader/interface/TextureLoader.h>
#include <DiligentCore/Common/interface/RefCntAutoPtr.hpp>
#include <DiligentCore/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <MultiTouch/Utils/AutoResizingArray.h>

class VertexEngine {
public:
    constexpr static float FLOAT_FALSE = 0;
    constexpr static float FLOAT_TRUE = 1;

    class Color4 {
    public:
        float red;
        float green;
        float blue;
        float alpha;
        const char * key;
        float isTexture;
        float textureResource;

        // only used for [texture -> color] conversion
        Diligent::ITexture* texture = nullptr;

        Color4(const float * data);
        Color4(const std::array<float, 4> & data);
        Color4(const std::array<float, 6> & data);
        Color4(const float &red, const float &green, const float &blue, const float &alpha);
        Color4(const char * key);
        Color4(Diligent::ITexture* texture);
        Color4(const std::array<float, 4> & data, Diligent::ITexture* texture);
        std::array<float, 4> to_RGBA_array() const;
        std::array<float, 4> to_ARGB_array() const;
        std::array<float, 6> to_array() const;
        std::vector<float> to_RGBA_vector() const;
        std::vector<float> to_ARGB_vector() const;
        std::vector<float> to_vector() const;
        uint32_t to_RGBA_unsigned_32bit_int() const;
        uint32_t to_ARGB_unsigned_32bit_int() const;

        static std::array<float, 4> float_RGBA_to_ARGB_array(const float & red, const float & green, const float & blue, const float & alpha);
        static std::array<float, 4> float_RGBA_array_to_ARGB_array(const std::array<float, 4> & data);
        static std::array<float, 4> float_RGBA_vector_to_ARGB_array(const std::vector<float> & data);
        static std::vector<float> float_RGBA_to_ARGB_vector(const float & red, const float & green, const float & blue, const float & alpha);
        static std::vector<float> float_RGBA_array_to_ARGB_vector(const std::array<float, 4> & data);
        static std::vector<float> float_RGBA_vector_to_ARGB_vector(const std::vector<float> & data);

        static std::array<float, 4> float_ARGB_to_RGBA_array(const float & alpha, const float & red, const float & green, const float & blue);
        static std::array<float, 4> float_ARGB_array_to_RGBA_array(const std::array<float, 4> & data);
        static std::array<float, 4> float_ARGB_vector_to_RGBA_array(const std::vector<float> & data);
        static std::vector<float> float_ARGB_to_RGBA_vector(const float & alpha, const float & red, const float & green, const float & blue);
        static std::vector<float> float_ARGB_array_to_RGBA_vector(const std::array<float, 4> & data);
        static std::vector<float> float_ARGB_vector_to_RGBA_vector(const std::vector<float> & data);

        static uint32_t unsigned_8bit_to_unsigned_32_bit(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

        static uint32_t unsigned_8bit_int_to_RGBA_unsigned_32bit_int(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
        static uint32_t unsigned_8bit_int_to_RGBA_unsigned_32bit_int(const std::array<uint8_t, 4> & data);
        static uint32_t float_to_RGBA_unsigned_32bit_int(const float & red, const float & green, const float & blue, const float & alpha);
        static uint32_t float_to_RGBA_unsigned_32bit_int(const std::array<float, 4> & data);
        static uint32_t unsigned_32bit_int_to_RGBA_unsigned_32bit_int(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha);
        static uint32_t unsigned_32bit_int_to_RGBA_unsigned_32bit_int(const std::array<uint32_t, 4> & data);

        static uint32_t unsigned_8bit_int_to_ARGB_unsigned_32bit_int(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
        static uint32_t unsigned_8bit_int_to_ARGB_unsigned_32bit_int(const std::array<uint8_t, 4> & data);
        static uint32_t float_to_ARGB_unsigned_32bit_int(const float & red, const float & green, const float & blue, const float & alpha);
        static uint32_t float_to_ARGB_unsigned_32bit_int(const std::array<float, 4> & data);
        static uint32_t unsigned_32bit_int_to_ARGB_unsigned_32bit_int(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha);
        static uint32_t unsigned_32bit_int_to_ARGB_unsigned_32bit_int(const std::array<uint32_t, 4> & data);

        static std::array<float, 4> unsigned_8bit_Component4_to_float_array(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
        static std::vector<float> unsigned_8bit_Component4_to_float_vector(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
        static std::array<float, 4> unsigned_8bit_array_to_float_array(const std::array<uint8_t, 4> & data);
        static std::vector<float> unsigned_8bit_array_to_float_vector(const std::array<uint8_t, 4> & data);
        static std::array<float, 4> unsigned_8bit_vector_to_float_array(const std::vector<uint8_t> & data);
        static std::vector<float> unsigned_8bit_vector_to_float_vector(const std::vector<uint8_t> & data);
    };

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
        Kernel kernel {8};
    public:
        HANDLE add(data_type data);

        storage_type * get(HANDLE handle);
        storage_type * get(Object * object);
        storage_type * get(const size_t & index);
        tl::optional<size_t> getIndex(HANDLE handle);

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
            Triple<const char *, size_t, Color4>,
            Triple<const char *, size_t, Color4>
    > textureBuffer;
    std::vector<HANDLE> vertexBufferOrder;
    std::vector<HANDLE> indexHandles;
    Buffer<const std::vector<uint32_t>&, std::vector<uint32_t>> indexBuffer;
    uint32_t indexPosition;

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

    bool externalTextureManager = false;

public:
    static const int positionCount = 3;
    static const int colorCount = 4;
    static const int colorExtraCount = 2;
    static const int textureCoordinatesCount = 2;
    static const int strideLength = 3+5+2;

    VertexEngine();
    ~VertexEngine();

    /**
     * clears the vertex and index buffers
     */
    void clear();

    class GenerationInfo {
        size_t chunkReader;
        VertexEngine * vertexEngine;
    public:

        size_t chunksGenerated;

        size_t lengthData;
        size_t sizeInBytesData;
        float * data;

        size_t lengthIndices;
        size_t sizeInBytesIndices;
        uint32_t * indices;

        bool isTexture;
        tl::optional<size_t> textureIndex;

        GenerationInfo();

        GenerationInfo(
                VertexEngine * engine, const size_t lengthData, const float *data,
                const size_t lengthIndices, const uint32_t *indices
        );

        GenerationInfo(
                VertexEngine * engine, const size_t lengthData, const float *data,
                const size_t lengthIndices, const uint32_t *indices,
                const bool isTexture
        );

        GenerationInfo(const GenerationInfo& m);

        GenerationInfo& operator=(const GenerationInfo& m);

        GenerationInfo(GenerationInfo && m) noexcept;

        GenerationInfo& operator=(GenerationInfo&& m) noexcept;

        ~GenerationInfo();

        static bool isMultipleOf(size_t bytes, size_t dataSize, size_t multipleOf);

        void resetChunkReader();

        bool canGenerateChunk(size_t chunkSize) const;

        GenerationInfo generateChunk(size_t chunkSize);

        void fillColorData(const std::array<float, 4> &color);
        void fillColorDataAndDisableTextureFlag(const std::array<float, 4> &color);
        void fillColorDataAndDisableTextureFlag(const Color4 &color);
        void fillColorDataAndDisableTextureFlag(const char *key);
    };

    /**
     * generates a vertex buffer for use in OpenGL
     * @return information about the generated data
     */
    GenerationInfo generateGL();

    // shape and geometry generation

    class Canvas {
        VertexEngine * vertexEngine = nullptr;
        std::vector<std::pair<HANDLE, std::vector<HANDLE>>> vertexBufferHandles;
        std::vector<HANDLE> indexHandles;

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
            std::array<float, 2> to_array() const;
            std::vector<float> to_vector() const;
        };

        typedef Position2 TextureCoordinate2;

        class Position3 {
        public:
            float x;
            float y;
            float z;

            Position3(const std::array<float, 3> & data);
            Position3(const float & x, const float & y, const float & z);
            std::array<float, 3> to_array() const;
            std::vector<float> to_vector() const;
        };

        typedef VertexEngine::Color4 Color4;

    private:
        void checkIfTextureKeyExists(const Color4& colorData);

        Canvas * parent = nullptr;

        PixelToNDC pixelConverter;

    public:
        GenerationInfo generateGL();

        const int x;
        const int y;
        const int width;
        const int height;

        Canvas(Canvas * parent, int x, int y, int width, int height);
        Canvas(VertexEngine * engine, int x, int y, int width, int height);
        Canvas(VertexEngine * engine, int width, int height);
        ~Canvas();

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

        /**
         * converts the given coordinates into
         * NDC coordinates based on the Canvas's height
         * @return the resulting NDC coordinates
         */
        PixelToNDC::Coordinates<float> toNDC(int x, int y, int z);

        static Color4 black;
        static uint32_t black_RGBA_unsigned_32bit_int;
        static uint32_t black_ARGB_unsigned_32bit_int;
        
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

    class TextureManager {
        VertexEngine * vertexEngine;
        Diligent::IRenderDevice  * m_pDevice;
        Diligent::IDeviceContext * m_pImmediateContext;
    public:
        TextureManager();
        TextureManager(VertexEngine * engine);

        void addRef();

        /**
         * set the default device and context to use for texture operations
         * <br>
         * <br>
         * this function should be called as follows:
         * <br>
         * <br>
         * Diligent::RefCntAutoPtr<Diligent::IRenderDevice>  device;
         * <br>
         * Diligent::RefCntAutoPtr<Diligent::IDeviceContext> context;
         * <br>
         * <br>
         * // init device and context
         * <br>
         * // ...
         * <br>
         * <br>
         * setDefaultDevices(device.RawPtr(), context.RawPtr());
         */
        void setDefaultDevices(
                Diligent::IRenderDevice  * m_pDevice,
                Diligent::IDeviceContext * m_pImmediateContext
        );
        
        /**
         * set the default device and context to use for texture operations
         * <br>
         * <br>
         * the devices are obtained from the given texture manager
         */
        void setDefaultDevices(const TextureManager * textureManager);

        std::pair<bool, std::vector<uint8_t>> imageIsSolidColor(Diligent::RefCntAutoPtr<Diligent::Image> image);

        /**
         * creates a 1x1 texture with a specified color
         * <br>
         * <br>
         * this is generally useful if a dummy texture is required for a shader
         * <br>
         * <br>
         * however `createTextureFromColor(key, color)` should be used instead
         * if your intent is to use a texture as a solid color
         * <br>
         * since the texture will be optimized away into vertex color data
         * @param key the key that will be used to locate the texture
         * @param color the color to use
         */
        void createSolidColorTexture(const char *key, const Color4 & color);

        /**
         * creates a 1x1 texture with a default color of black
         * <br>
         * <br>
         * this is generally useful if a dummy texture is required for a shader
         * <br>
         * <br>
         * however `createTextureFromColor(key)` should be used instead
         * if your intent is to use a texture as a solid color
         * <br>
         * since the texture will be optimized away into vertex color data
         * @param key the key that will be used to locate the texture
         */
        void createSolidColorTexture(const char *key);

        /**
         * creates a texture from the specified color
         * <br>
         * <br>
         * this optimizes away the texture and instead uses the color
         * directly as vertex color data
         * <br>
         * if you REALLY want a texture then please use
         * `createSolidColorTexture(key, color)` instead
         * @param key the key that will be used to locate the texture
         * @param color the color to use
         */
        void createTextureFromColor(const char *key, const Color4 &color);

        /**
         * creates a texture with a default color of black
         * <br>
         * <br>
         * this optimizes away the texture and instead uses the color
         * directly as vertex color data
         * <br>
         * if you REALLY want a texture then please use
         * `createSolidColorTexture(key)` instead
         * @param key the key that will be used to locate the texture
         */
        void createTextureFromColor(const char *key);

        /**
         * creates a texture from the specified path
         * <br>
         * <br>
         * this calls `createTextureFromColor(key, DETECTED_COLOR)` if the loaded image
         * is detected to be a solid color, which is
         * optimized away into vertex color data
         * @param key the key that will be used to locate the texture
         * @param FilePath Source file path
         * @param TexLoadInfo Texture loading information
         * @note this function is expensive and SHOULD NOT be called in onDraw
         */
        void createTextureFromFile(const char *key, const Diligent::Char *FilePath,
                                   const Diligent::TextureLoadInfo &TexLoadInfo);

        /**
         * finds a texture associated with the key used to create the texture
         * @param key the key to locate the texture
         * @return nullptr if the key cannot be found
         *          or if the texture has been converted into color data
         */
        Diligent::ITexture *findTexture(const char *key);

        /**
         * obtains a texture associated with the index returned by findTextureIndex
         * @param index the index returned by findTextureIndex
         * @return nullptr if the key cannot be found
         *          or `third.texture == nullptr` if the texture has been
         *          converted into color data
         */
        Triple<const char *, size_t, Color4> * getTexture(const size_t & index);

        /**
         * obtains a texture associated with the key used to create the texture
         * @param key the key to locate the texture
         * @return nullptr if the key cannot be found
         *          or `third.texture == nullptr` if the texture has been
         *          converted into color data
         */
        Triple<const char *, size_t, Color4> * getTexture(const char *key);

        /**
         * finds a texture
         * @param texture the texture to locate
         * @return tl::nullopt if the texture cannot be found
         *          or `getTexture(returnedIndex.value())->third.texture == nullptr`
         *          if the texture has been converted into color data
         */
        tl::optional<size_t> findTexture(Diligent::ITexture * texture);

        /**
         * finds a texture associated with the key used to create the texture
         * @param key the key to locate the texture
         * @return tl::nullopt if the key cannot be found
         *          or `getTexture(returnedIndex.value())->third.texture == nullptr`
         *          if the texture has been converted into color data
         */
        tl::optional<size_t> findTextureIndex(const char *key);

        /**
         * deletes a texture associated with the key used to create the texture
         * @param key the key to locate the texture
         */
        void deleteTexture(const char *key);

        bool keyMatches(const char *key, Triple<const char *, size_t, Color4> *pTriple);
    };

    class TextureCache {
    public:
        struct StringInfo {
            const char * string;
            size_t stringLength;
            StringInfo(const char *string, size_t stringLength);
        };
        
        struct KeyInfo : public StringInfo {
            TextureManager * id;

            KeyInfo(const char *string, size_t stringLength, TextureManager * id);
        };
        
        struct TextureReference {
            size_t count;
            Diligent::ITexture* texture;

            TextureReference(size_t count, Diligent::ITexture *texture);
        };
        struct TextureInfo {
            StringInfo path;
            std::vector<KeyInfo> keys;
            TextureReference textureReference;

            TextureInfo(const StringInfo &path, std::vector<KeyInfo> keys,
                        const TextureReference &textureReference);
        };
    private:
        std::vector<TextureInfo> textureCacheBuffer;
    public:
        TextureInfo * findPath(const char *filePath, size_t filePathStrLen);

        TextureInfo * findKey(const char *key, size_t keyStrLen, TextureManager * id);

        void
        addRef(const char *filePath, size_t filePathStrLen, TextureManager * id, const char *key,
               Diligent::ITexture *texture);

        void
        addRef(const char *filePath, size_t filePathStrLen, TextureManager * id, const char *key,
               size_t keyLen, Diligent::ITexture *texture);

        void
        addRef(const char *filePath, size_t filePathStrLen, TextureManager * id, const char *key);

        void
        addRef(const char *filePath, size_t filePathStrLen, TextureManager * id, const char *key,
               size_t keyLen);

        void removeRef(Diligent::ITexture * tex);
    };

    TextureManager * textureManager = nullptr;
    VertexEngine(TextureManager * textureManager);

    void removeIndexHandle(HANDLE handle);
};

static VertexEngine::TextureCache vertexEngineTextureFileCache;

#endif //GRAPHICAL_TOOL_KIT_VERTEXENGINE_H
