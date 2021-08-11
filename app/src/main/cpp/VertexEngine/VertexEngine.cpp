//
// Created by Matthew Good on 17/7/21.
//

#include "VertexEngine.h"
#include <cmath>
#include <DiligentTools/TextureLoader/interface/TextureUtilities.h>
#include <DiligentTools/TextureLoader/interface/TextureLoader.h>
#include <DiligentCore/Graphics/GraphicsAccessories/interface/GraphicsAccessories.hpp>
#include <DiligentTools/TextureLoader/interface/Image.h>

template<typename A, typename B, typename C>
VertexEngine::Triple<A, B, C>::Triple(A first, B second, C third):
        first(first), second(second), third(third)
{}

template<typename data_type, typename storage_type>
HANDLE VertexEngine::Buffer<data_type, storage_type>::add(data_type data) {
    return kernel.newHandle(ObjectTypeNone, storage_type(data));
}

template<typename data_type, typename storage_type>
storage_type *VertexEngine::Buffer<data_type, storage_type>::get(HANDLE handle) {
    return kernel.getHandle(handle)->object->resource.template get<storage_type*>();
}

template<typename data_type, typename storage_type>
storage_type *VertexEngine::Buffer<data_type, storage_type>::get(Object * object) {
    return object->resource.template get<storage_type*>();
}

template<typename data_type, typename storage_type>
storage_type *VertexEngine::Buffer<data_type, storage_type>::get(const size_t & index) {
    return kernel.table->objectAt(index)->resource.template get<storage_type*>();
}

template<typename data_type, typename storage_type>
tl::optional<size_t> VertexEngine::Buffer<data_type, storage_type>::getIndex(HANDLE handle) {
    return kernel.table->findObject(kernel.getHandle(handle)->object);
}

template<typename data_type, typename storage_type>
void VertexEngine::Buffer<data_type, storage_type>::remove(HANDLE handle) {
    kernel.removeHandle(handle);
}

template<typename data_type, typename storage_type>
void VertexEngine::Buffer<data_type, storage_type>::clear() {
    Table::Iterator i = getIterator();
    while (i.hasNext()) {
        remove(i.next()->handle);
    }
}

template<typename data_type, typename storage_type>
Table::Iterator VertexEngine::Buffer<data_type, storage_type>::getIterator() {
    return kernel.table->getIterator();
}


PixelToNDC::Coordinates<float> VertexEngine::toNDC(int x, int y, int z) {
    return pixelConverter.toNDC<int, float>(x, y, z, width, height);
}

VertexEngine::VertexInfo::VertexInfo(size_t length, bool is_static) {
    this->length = length;
    this->is_static = is_static;
}

VertexEngine::VertexEngine(int width, int height, TextureManager * textureManager) :
        defaultPositionBuffer(addDataBuffer(positionCount)),
        defaultColorBuffer(addDataBuffer(colorCount+colorExtraCount)),
        defaultTextureCoordinateBuffer(addDataBuffer(textureCoordinatesCount)),
        width(width),
        height(height),
        indexPosition(0),
        textureManager(textureManager == nullptr ? new TextureManager(this) : textureManager),
        externalTextureManager(textureManager != nullptr)
{
    order({defaultPositionBuffer, defaultColorBuffer, defaultTextureCoordinateBuffer});
}

VertexEngine::VertexEngine(int width, int height) :
        VertexEngine::VertexEngine(width, height, nullptr)
{
}

VertexEngine::VertexEngine() :
        VertexEngine::VertexEngine(0,0, nullptr)
{
}

VertexEngine::~VertexEngine() {
    clear();
    vertexBuffer.clear();
    if (!externalTextureManager) delete textureManager;
}

void VertexEngine::resize(int width, int height) {
    this->width = width;
    this->height = height;
}

int VertexEngine::getWidth() {
    return width;
}

int VertexEngine::getHeight() {
    return height;
}

void VertexEngine::clear() {
    auto i = vertexBuffer.getIterator();
    while (i.hasNext()) {
        Object * o = i.next();
        bool HANDLE_IS_DEFAULT_POSITION_BUFFER = defaultPositionBuffer == o->handle;
        bool HANDLE_IS_DEFAULT_COLOR_BUFFER = defaultColorBuffer == o->handle;
        bool HANDLE_IS_DEFAULT_TEXCORD_BUFFER = defaultTextureCoordinateBuffer == o->handle;
//        Log::Info("handle is default position buffer: ", HANDLE_IS_DEFAULT_POSITION_BUFFER);
//        Log::Info("handle is default color buffer: ", HANDLE_IS_DEFAULT_COLOR_BUFFER);
//        Log::Info("handle is default texcord buffer: ", HANDLE_IS_DEFAULT_TEXCORD_BUFFER);
        auto v = vertexBuffer.get(o);
        v->dataBuffer.clear();
    }
    indexBuffer.clear();
    indexHandles.clear();
    indexPosition = 0;
}

HANDLE VertexEngine::addDataBuffer(size_t length) {
    return vertexBuffer.add({length, false});
}

HANDLE VertexEngine::addDataBufferStatic(size_t length) {
    return vertexBuffer.add({length, true});
}

void VertexEngine::removeDataBuffer(HANDLE handle) {
    return vertexBuffer.remove(handle);
}

void VertexEngine::order(const std::vector<HANDLE>& data) {
    vertexBufferOrder = data;
}

VertexEngine::GenerationInfo::GenerationInfo() :
        vertexEngine(nullptr),
        data(nullptr),
        lengthData(0),
        sizeInBytesData(0),
        indices(nullptr),
        lengthIndices(0),
        sizeInBytesIndices(0),
        isTexture(false),
        textureIndex(0),
        chunkReader(0),
        chunksGenerated(0)
{
}

VertexEngine::GenerationInfo::GenerationInfo(
        VertexEngine * engine,
        const size_t lengthData, const float *data,
        const size_t lengthIndices, const uint32_t *indices,
        const bool isTexture
) :
    vertexEngine(engine),
    data(new float[lengthData]),
    lengthData(lengthData),
    sizeInBytesData(lengthData * sizeof(float)),
    indices(new uint32_t[lengthIndices]),
    lengthIndices(lengthIndices),
    sizeInBytesIndices(lengthIndices * sizeof(uint32_t)),
    isTexture(isTexture),
    textureIndex(0),
    chunkReader(0),
    chunksGenerated(0)
{
    if (data != nullptr) memcpy(this->data, data, sizeInBytesData);
    else memset(this->data, 0, sizeInBytesData);
    if (indices != nullptr) memcpy(this->indices, indices, sizeInBytesIndices);
    else memset(this->indices, 0, sizeInBytesIndices);
}

VertexEngine::GenerationInfo::GenerationInfo(
        VertexEngine * engine,
        const size_t lengthData, const float *data,
        const size_t lengthIndices, const uint32_t *indices
) : GenerationInfo(engine, lengthData, data, lengthIndices, indices, false)
{
}

VertexEngine::GenerationInfo::GenerationInfo(const VertexEngine::GenerationInfo &m) :
    GenerationInfo(m.vertexEngine, m.lengthData, m.data, m.lengthIndices, m.indices, m.isTexture)
{
}

VertexEngine::GenerationInfo& VertexEngine::GenerationInfo::operator=(const VertexEngine::GenerationInfo& m) {
    delete[] data;
    data = new float[m.lengthData];
    memcpy(data, m.data, m.sizeInBytesData);
    lengthData = m.lengthData;
    sizeInBytesData = m.sizeInBytesData;

    delete[] indices;
    indices = new uint32_t[m.lengthIndices];
    memcpy(indices, m.indices, m.sizeInBytesIndices);
    lengthIndices = m.lengthIndices;
    sizeInBytesIndices = m.sizeInBytesIndices;

    vertexEngine = m.vertexEngine;
    isTexture = m.isTexture;
    textureIndex = m.textureIndex;
    chunkReader = m.chunkReader;
    chunksGenerated = m.chunksGenerated;
    return *this;
}

VertexEngine::GenerationInfo::GenerationInfo(VertexEngine::GenerationInfo &&m) noexcept :
        vertexEngine(m.vertexEngine),
        data(m.data),
        lengthData(m.lengthData),
        sizeInBytesData(m.sizeInBytesData),
        indices(m.indices),
        lengthIndices(m.lengthIndices),
        sizeInBytesIndices(m.sizeInBytesIndices),
        chunkReader(m.chunkReader),
        chunksGenerated(m.chunksGenerated),
        isTexture(m.isTexture),
        textureIndex(m.textureIndex)
{
    m.vertexEngine = nullptr;
    m.data = nullptr;
    m.lengthData = 0;
    m.sizeInBytesData = 0;
    m.indices = nullptr;
    m.lengthIndices = 0;
    m.sizeInBytesIndices = 0;
    m.chunkReader = 0;
    m.chunksGenerated = 0;
    m.isTexture = false;
    m.textureIndex = 0;
}

VertexEngine::GenerationInfo& VertexEngine::GenerationInfo::operator=(VertexEngine::GenerationInfo&& m) noexcept {
    vertexEngine = m.vertexEngine;
    m.vertexEngine = nullptr;
    data = m.data;
    m.data = nullptr;
    lengthData = m.lengthData;
    m.lengthData = 0;
    sizeInBytesData = m.sizeInBytesData;
    m.sizeInBytesData = 0;
    indices = m.indices;
    m.indices = nullptr;
    lengthIndices = m.lengthIndices;
    m.lengthIndices = 0;
    sizeInBytesIndices = m.sizeInBytesIndices;
    m.sizeInBytesIndices = 0;
    chunkReader = m.chunkReader;
    m.chunkReader = 0;
    chunksGenerated = m.chunksGenerated;
    m.chunksGenerated = 0;
    isTexture = m.isTexture;
    m.isTexture = false;
    textureIndex = m.textureIndex;
    m.textureIndex = 0;
    return *this;
}

VertexEngine::GenerationInfo::~GenerationInfo() {
    delete[] data;
    delete[] indices;
}

bool VertexEngine::GenerationInfo::isMultipleOf(size_t bytes, size_t dataSize, size_t multipleOf) {
    return bytes % (dataSize*multipleOf) == 0;
}

void VertexEngine::GenerationInfo::resetChunkReader() {
    chunkReader = 0;
}

bool VertexEngine::GenerationInfo::canGenerateChunk(size_t chunkSize) const {
    if (chunkSize <= 0 || lengthIndices <= 0) return false;
    if (chunkReader >= lengthIndices) return false;
    if (chunkSize > lengthIndices) return true;
    return isMultipleOf(sizeInBytesIndices, sizeof(uint32_t), chunkSize);
}

VertexEngine::GenerationInfo VertexEngine::GenerationInfo::generateChunk(size_t chunkSize) {
    VertexEngine::GenerationInfo out(vertexEngine, strideLength*chunkSize, nullptr, chunkSize, nullptr, true);

    // indices increase from 0, 1, 2, ...
    size_t vertexIndex = 0;
    size_t indicesIndex = 0;

    // for now, we know the stride length exactly:
//        index + 0;  // pos x
//        index + 1;  // pos y
//        index + 2;  // pos z
//        index + 3;  // col r
//        index + 4;  // col g
//        index + 5;  // col b
//        index + 6;  // col a
//        index + 7;  // col isTexture
//        index + 8;  // col textureResource
//        index + 9;  // tex x
//        index + 10; // tex y

    bool first = true;
    float firstIsTexture;
    bool firstTextureIndexIsNan;
    float firstTextureIndex;

    while(indicesIndex < chunkSize && chunkReader < lengthIndices) {
        uint32_t index = indices[chunkReader] * (strideLength + 1);

        // to support textures, we need to separate different textures into chunks
        // same as generate: compare first input to rest of input

        uint32_t isTextureIndex = index + 7;
        uint32_t textureIndexIndex = isTextureIndex + 1;
        if (first) {
            first = false;
            firstIsTexture = data[isTextureIndex];
            firstTextureIndexIsNan = std::isnan(data[textureIndexIndex]);
            firstTextureIndex = data[textureIndexIndex];
            out.isTexture = firstIsTexture == FLOAT_TRUE ? true : false;
            if (firstTextureIndexIsNan || data[textureIndexIndex] == -1.0f) {
                out.textureIndex = tl::nullopt;
            } else {
                out.textureIndex = data[textureIndexIndex];
            }
        }
        if (
            firstIsTexture == data[isTextureIndex]
            && firstTextureIndexIsNan ? (
                firstTextureIndexIsNan == std::isnan(data[textureIndexIndex])
            ) : ( firstTextureIndex == data[textureIndexIndex] )
        ) {
            chunkReader++;

            out.indices[indicesIndex] = indicesIndex;
            indicesIndex++;

            // position 3
            out.data[vertexIndex++] = data[index++];
            out.data[vertexIndex++] = data[index++];
            out.data[vertexIndex++] = data[index++];
            // color 4
            out.data[vertexIndex++] = data[index++];
            out.data[vertexIndex++] = data[index++];
            out.data[vertexIndex++] = data[index++];
            out.data[vertexIndex++] = data[index++];
            // isTexture 1
            out.data[vertexIndex++] = data[index++];
            // skip textureIndex
            index++;
            // texture coordinate 2
            out.data[vertexIndex++] = data[index++];
            out.data[vertexIndex++] = data[index++];
        } else {
            LOG_WARNING_MESSAGE("chunk texture info differs from first chunk");
            break;
        }
    }
    chunksGenerated++;
    return out;
}

void VertexEngine::GenerationInfo::fillColorData(const std::array<float, 4> &color) {
// for now, we know the stride length exactly:
//        index + 0;  // pos x
//        index + 1;  // pos y
//        index + 2;  // pos z
//        index + 3;  // col r
//        index + 4;  // col g
//        index + 5;  // col b
//        index + 6;  // col a
//        index + 7;  // col isTexture
//        index + 8;  // tex x
//        index + 9; // tex y

    size_t vertexIndex = 0;
    float colorData[4] = {color[0], color[1], color[2], color[3]};
    size_t copy_amount = sizeof(float) * 4;
    while(vertexIndex < lengthData) {
        memcpy(data + 3 + vertexIndex, colorData, copy_amount);
        vertexIndex += 10;
    }
}

void VertexEngine::GenerationInfo::fillColorDataAndDisableTextureFlag(const std::array<float, 4> &color) {
// for now, we know the stride length exactly:
//        index + 0;  // pos x
//        index + 1;  // pos y
//        index + 2;  // pos z
//        index + 3;  // col r
//        index + 4;  // col g
//        index + 5;  // col b
//        index + 6;  // col a
//        index + 7;  // col isTexture
//        index + 8;  // tex x
//        index + 9; // tex y

    size_t vertexIndex = 0;
    float colorData[5] = {color[0], color[1], color[2], color[3], FLOAT_FALSE};
    size_t copy_amount = sizeof(float) * 5;
    while(vertexIndex < lengthData) {
        memcpy(data + 3 + vertexIndex, colorData, copy_amount);
        vertexIndex += 10;
    }
}

void VertexEngine::GenerationInfo::fillColorDataAndDisableTextureFlag(const Color4 &color) {
    fillColorDataAndDisableTextureFlag(color.to_RGBA_array());
}

void VertexEngine::GenerationInfo::fillColorDataAndDisableTextureFlag(const char *key) {
    if (key == nullptr) {
        LOG_ERROR_AND_THROW("cannot fill color data from a nullptr key");
    }
    auto tex = vertexEngine->textureManager->getTexture(key);
    if (tex == nullptr) {
        LOG_ERROR_AND_THROW("key '", key, "' does not exist");
    }
    fillColorDataAndDisableTextureFlag(tex->third.to_RGBA_array());
}

VertexEngine::GenerationInfo VertexEngine::generateGL() {
    // the vertex buffer order contains handles to data buffers
    // and specifies the order in which we should generate output data
    //
    // vertex buffer data will not be generated for handles
    // that are not specified by the vertex buffer order

    // first check if we have all buffers as static

    bool isAllStatic = true;

    for (HANDLE group : vertexBufferOrder) {
        VertexInfo *vertexInfo = vertexBuffer.get(group);
        if (!vertexInfo->is_static) {
            isAllStatic = false;
        }
    }

    size_t lengthData = 0;
    size_t lengthIndices = 0;

    // compute index buffer size
    size_t indexBufferSize = 0;

    for (HANDLE buffer : indexHandles) {
        indexBufferSize += indexBuffer.get(buffer)->size();
    }

    uint32_t indicesBuffer[indexBufferSize];

    // fill index buffer
    for (HANDLE buffer : indexHandles) {
        std::vector<uint32_t> *indices = indexBuffer.get(buffer);
        for (size_t i = 0; i < indices->size(); i++) {
            indicesBuffer[lengthIndices++] = *(indices->begin() + i);
        }
    }

    if (isAllStatic) {
        // if we have all static, then every group contains only a single buffer
        //
        // ORDER A, B, C
        // OUT A.data, B.data, C.data

        // compute data buffer size
        size_t dataBufferSize = 0;

        for (HANDLE group : vertexBufferOrder) {
            VertexInfo * info = vertexBuffer.get(group);
            dataBufferSize += info->length;
        }

        float dataBuffer[dataBufferSize];

        // fill data buffer
        for (HANDLE group : vertexBufferOrder) {
            VertexInfo *info = vertexBuffer.get(group);
            const std::vector<float>&data = *info->dataBuffer.get(info->static_data);
            for (float i : data) {
                dataBuffer[lengthData++] = i;
            }
        }

        return GenerationInfo(this, lengthData, dataBuffer, lengthIndices, indicesBuffer);
    } else {
        // otherwise, every group contains multiple buffers
        // ensure they are all the same size
        size_t bufferCount = 0;
        // grab the first size and match it against the rest
        bool first = false;
        for (HANDLE group : vertexBufferOrder) {
            VertexInfo *info = vertexBuffer.get(group);
            if (!info->is_static) {
                if (!first) {
                    Table::Iterator iterator = info->dataBuffer.getIterator();
                    while (iterator.hasNext()) {
                        iterator.next();
                        bufferCount++;
                    }
                    first = true;
                } else {
                    size_t dataCount = 0;
                    Table::Iterator iterator = info->dataBuffer.getIterator();
                    while (iterator.hasNext()) {
                        iterator.next();
                        dataCount++;
                    }
                    if (bufferCount != dataCount) {
                        LOG_FATAL_ERROR_AND_THROW("buffer count (", bufferCount,
                                                  ") is greater than current data buffer count (",
                                                  dataCount, ")");
                    }
                }
            }
        }

        // if we reached here, then all our groups contain the same number of buffers
        //
        // ORDER A, B, C
        // OUT A.data1, B.data1, C.data1, A.data2, B.data2, C.data2

        // compute data buffer size
        size_t dataBufferSize = 0;

        for (size_t c = 0; c < bufferCount; c++) {
            for (HANDLE group : vertexBufferOrder) {
                VertexInfo *info = vertexBuffer.get(group);
                Table::Iterator iterator = info->dataBuffer.getIterator();
                if (!info->is_static) iterator.skip(c);
                if (iterator.hasNext()) dataBufferSize += info->length;
            }
        }

        float dataBuffer[dataBufferSize];

        // fill buffer
        for (size_t c = 0; c < bufferCount; c++) {
            for (HANDLE group : vertexBufferOrder) {
                VertexInfo *info = vertexBuffer.get(group);
                Table::Iterator iterator = info->dataBuffer.getIterator();
                if (!info->is_static) iterator.skip(c);
                if (iterator.hasNext()) {
                    const std::vector<float>&data = *info->dataBuffer.get(iterator.next()->handle);
                    for (float i : data) {
                        dataBuffer[lengthData++] = i;
                    }
                }
            }
        }

        return GenerationInfo(this, lengthData, dataBuffer, lengthIndices, indicesBuffer);
    }
}

void VertexEngine::removeIndexHandle(HANDLE handle) {
    indexBuffer.remove(handle);
    std::vector<HANDLE> rebuild;
    for (HANDLE h : indexHandles) {
        if (h != handle) rebuild.push_back(h);
    }
    indexHandles = std::move(rebuild);
}

VertexEngine::TextureManager::TextureManager(VertexEngine *engine):
        vertexEngine(engine)
{
}

void VertexEngine::TextureManager::setDefaultDevices(
        const TextureManager * textureManager
) {
    m_pDevice = textureManager->m_pDevice;
    m_pImmediateContext = textureManager->m_pImmediateContext;
}

void VertexEngine::TextureManager::setDefaultDevices(
        Diligent::IRenderDevice  * m_pDevice,
        Diligent::IDeviceContext * m_pImmediateContext
) {
    this->m_pDevice = m_pDevice;
    this->m_pImmediateContext = m_pImmediateContext;
}

std::pair<bool, std::vector<uint8_t>>
VertexEngine::TextureManager::imageIsSolidColor(
        Diligent::RefCntAutoPtr<Diligent::Image> image
) {
    auto Desc = image->GetDesc();

    // image can be ARGB or RGB
    // we only support ARGB for now
    if (Desc.NumComponents == 4) {
        uint8_t color[Desc.NumComponents];
        uint8_t *pixels = static_cast<uint8_t *>(image->GetData()->GetDataPtr());
        for (uint32_t y = 0; y < Desc.Height; y++) {
            for (uint32_t x = 0; x < Desc.Width; x++) {
                if (y == 0 && x == 0) {
                    // dst, src, length
                    memcpy(color, pixels, Desc.NumComponents);
                } else {
                    if (memcmp(&pixels[x * y * Desc.NumComponents], color, Desc.NumComponents) != 0) {
                        return {false, {}};
                    }
                }
            }
        }
        return {true, {color[0], color[1], color[2], color[3]}};
    } else {
        LOG_ERROR_MESSAGE("Error while testing if image is a solid color: number of color components in image must be 4, was ", Desc.NumComponents);
        return {false, {}};
    }
}

void VertexEngine::TextureManager::createSolidColorTexture(const char *key, const Color4 & color) {
    if (key == nullptr) {
        LOG_ERROR_AND_THROW("a texture cannot be created using a nullptr key");
    }
    if (getTexture(key) != nullptr) {
        LOG_ERROR_AND_THROW("a texture with the key '", key, "' already exists");
    }
    Diligent::TextureDesc TexDesc;
    TexDesc.Name = "Dummy Texture";
    TexDesc.Type = Diligent::RESOURCE_DIM_TEX_2D;
    TexDesc.Format = Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB;
    TexDesc.Width = 1;
    TexDesc.Height = 1;
    TexDesc.MipLevels = 1;
    TexDesc.BindFlags = Diligent::BIND_SHADER_RESOURCE;
    TexDesc.Usage = Diligent::USAGE_IMMUTABLE;

    // init texture with color data
    Diligent::Uint32 pixelData[1] = {color.to_RGBA_unsigned_32bit_int()};
    Diligent::Uint32 stride = TexDesc.Width*4;
    Diligent::TextureSubResData subResData[] {{&pixelData, stride}};
    Diligent::TextureData InitData;
    InitData.pSubResources = subResData;
    InitData.NumSubresources = _countof(subResData);

    Diligent::ITexture * tex = nullptr;
    m_pDevice->CreateTexture(TexDesc, &InitData, &tex);
    vertexEngine->textureBuffer.add({key, strlen(key), {color.to_RGBA_array(), tex}});
}

void VertexEngine::TextureManager::createSolidColorTexture(const char *key) {
    createSolidColorTexture(key, VertexEngine::Canvas::black);
}

void VertexEngine::TextureManager::createTextureFromColor(const char *key,
                                                         const VertexEngine::Canvas::Color4 &color) {
    if (key == nullptr) {
        LOG_ERROR_AND_THROW("a texture cannot be created using a nullptr key");
    }
    if (getTexture(key) != nullptr) {
        LOG_ERROR_AND_THROW("a texture with the key '", key, "' already exists");
    }
    // do not create a texture
    vertexEngine->textureBuffer.add({key, strlen(key), color});
}

void VertexEngine::TextureManager::createTextureFromColor(const char *key) {
    createTextureFromColor(key, VertexEngine::Canvas::black);
}

void VertexEngine::TextureManager::createTextureFromFile(
        const char * key,
        const char *FilePath,
        const Diligent::TextureLoadInfo &TexLoadInfo
) {
    if (key == nullptr) {
        LOG_ERROR_AND_THROW("a texture cannot be created using a nullptr key");
    }
    if (getTexture(key) != nullptr) {
        LOG_ERROR_AND_THROW("a texture with the key '", key, "' already exists");
    }
    Diligent::RefCntAutoPtr<Diligent::Image>     pImage;
    Diligent::RefCntAutoPtr<Diligent::IDataBlob> pRawData;

    auto ImgFmt = Diligent::CreateImageFromFile(FilePath, &pImage, &pRawData);

    if (pImage) {
        auto r = imageIsSolidColor(pImage);
        if (r.first) {
            // associate key with solid color instead of texture
            vertexEngine->textureBuffer.add({
                key,
                strlen(key),
                Color4::unsigned_8bit_vector_to_float_array(r.second)
            });
            return;
        } else {
            Diligent::ITexture * tex = nullptr;
            Diligent::CreateTextureFromImage(pImage, TexLoadInfo, m_pDevice, &tex);
            vertexEngine->textureBuffer.add({key, strlen(key), tex});
        }
    } else if (pRawData) {
        // TODO: can DDS and KTX be solid color?
        if (ImgFmt == Diligent::IMAGE_FILE_FORMAT_DDS) {
            Diligent::ITexture * tex = nullptr;
            Diligent::CreateTextureFromDDS(pRawData->GetConstDataPtr(), pRawData->GetSize(),
                                           TexLoadInfo, m_pDevice, &tex);
            vertexEngine->textureBuffer.add({key, strlen(key), tex});
        } else if (ImgFmt == Diligent::IMAGE_FILE_FORMAT_KTX) {
            Diligent::ITexture * tex = nullptr;
            Diligent::CreateTextureFromKTX(pRawData->GetConstDataPtr(), pRawData->GetSize(),
                                           TexLoadInfo, m_pDevice, &tex);
            vertexEngine->textureBuffer.add({key, strlen(key), tex});
        } else {
            LOG_ERROR_AND_THROW("Unexpected format");
        }
    }
}

Diligent::ITexture * VertexEngine::TextureManager::findTexture(const char * key) {
    auto r = getTexture(key);
    if (r == nullptr) return nullptr;
    return keyMatches(key, r) ? r->third.texture : nullptr;
}

VertexEngine::Triple<const char *, size_t, VertexEngine::Color4> *
VertexEngine::TextureManager::getTexture(const size_t & index) {
    if (index == -1) return nullptr;
    auto * r = vertexEngine->textureBuffer.get(index);
    return r == nullptr ? nullptr : r;
}

VertexEngine::Triple<const char *, size_t, VertexEngine::Color4> *
VertexEngine::TextureManager::getTexture(const char * key) {
    if (key == nullptr) return nullptr;
    auto i = vertexEngine->textureBuffer.getIterator();
    while (i.hasNext()) {
        auto r = vertexEngine->textureBuffer.get(i.next()->handle);
        if (keyMatches(key, r)) return r;
    }
    return nullptr;
}

tl::optional<size_t> VertexEngine::TextureManager::findTextureIndex(const char * key) {
    auto i = vertexEngine->textureBuffer.getIterator();
    while (i.hasNext()) {
        auto idx = vertexEngine->textureBuffer.getIndex(i.next()->handle);
        if (!idx) continue;
        auto r = vertexEngine->textureBuffer.get(idx.value());
        if (keyMatches(key, r)) return idx;
    }
    return tl::nullopt;
}

void VertexEngine::TextureManager::deleteTexture(const char * key) {
    if (key != nullptr) {
        auto i = vertexEngine->textureBuffer.getIterator();
        while (i.hasNext()) {
            HANDLE h = i.next()->handle;
            auto r = vertexEngine->textureBuffer.get(h);
            if (keyMatches(key, r)) {
                if (r->third.texture != nullptr) r->third.texture->Release();
                vertexEngine->textureBuffer.remove(h);
                break;
            }
        }
    }
}

bool VertexEngine::TextureManager::keyMatches(
        const char *key, VertexEngine::Triple<const char *, size_t, VertexEngine::Color4> *p
) {
    return strlen(key) == p->second ? memcmp(key, p->first, p->second) == 0 : false;
}

HANDLE VertexEngine::Canvas::addData_(HANDLE dataBufferHandle, const std::vector<float>& data) {
    bool containsHandle = false;
    std::pair<HANDLE, std::vector<HANDLE>> * pair;
    for (auto & pair_ : vertexBufferHandles) {
        if (pair_.first == dataBufferHandle) {
            pair = &pair_;
            containsHandle = true;
            break;
        }
    }

    if (!containsHandle) {
        vertexBufferHandles.push_back({dataBufferHandle, {}});
        // find the added handle
        for (auto & pair_ : vertexBufferHandles) {
            if (pair_.first == dataBufferHandle) {
                pair = &pair_;
                containsHandle = true;
                break;
            }
        }
    }

    VertexInfo * buffer = vertexEngine->vertexBuffer.get(dataBufferHandle);
    size_t dataLength = data.size();
    size_t bufferLength = buffer->length;
    if (dataLength > bufferLength) {
        LOG_FATAL_ERROR_AND_THROW("data length (", dataLength, ") is greater than buffer length (", bufferLength, ")");
    }
    if (buffer->is_static) {
        if (buffer->static_data != nullptr) {
            size_t c = pair->second.size();
            for (size_t i = 0; i < c; ++i) {
                if (pair->second[i] == buffer->static_data) {
                    buffer->dataBuffer.remove(pair->second[i]);
                    pair->second[i] = buffer->dataBuffer.add(data);
                    buffer->static_data = pair->second[i];
                    return pair->second[i];
                }
            }
            Log::Error_And_Throw("buffer is non null but does not exist in known handles");
        } else {
            buffer->static_data = buffer->dataBuffer.add(data);
            pair->second.push_back(buffer->static_data);
            return buffer->static_data;
        }
    } else {
        HANDLE handle = buffer->dataBuffer.add(data);
        pair->second.push_back(handle);
        return handle;
    }
}

std::vector<HANDLE> VertexEngine::Canvas::addData_(
        const std::vector<std::pair<HANDLE, const std::vector<float>&>>& handle_and_data) {
    std::vector<HANDLE> handles;
    handles.reserve(handle_and_data.size());
    for (const std::pair<void *, const std::vector<float>&> & pair : handle_and_data) {
        handles.push_back(addData_(pair.first, pair.second));
    }
    return handles;
}

HANDLE VertexEngine::Canvas::addIndexData_(const std::vector<uint32_t>& data) {
    HANDLE handle = vertexEngine->indexBuffer.add(data);
    vertexEngine->indexHandles.push_back(handle);
    indexHandles.push_back(handle);
    return handle;
}

VertexEngine::Canvas::Canvas(VertexEngine::Canvas *parent, int x, int y, int width, int height) :
        x(x), y(y), width(width), height(height)
{
    if(parent == nullptr) {
        LOG_FATAL_ERROR_AND_THROW("parent cannot be nullptr");
    }
    int actualX = parent->x + x;
    int actualY = parent->y + y;
    int actualWidth = actualX + width;
    int actualHeight = actualY + height;

    if(actualWidth > parent->width) {
        LOG_FATAL_ERROR_AND_THROW("subcanvas actual width (", actualWidth, ") is greater than canvas width (", parent->width, ")");
    }
    if(actualHeight > parent->height) {
        LOG_FATAL_ERROR_AND_THROW("subcanvas actual height (", actualHeight, ") is greater than canvas height (", parent->height, ")");
    }
    // attach vertex engine
    vertexEngine = parent->vertexEngine;
    this->parent = parent;
}

VertexEngine::Canvas::Canvas(VertexEngine *engine, int x, int y, int width, int height) :
    x(x), y(y), width(width), height(height)
{
    vertexEngine = new VertexEngine(engine->width, engine->height, engine->textureManager);
}

VertexEngine::Canvas::Canvas(VertexEngine *engine, int width, int height) :
        VertexEngine::Canvas(engine, 0, 0, width, height)
{
}

VertexEngine::Canvas::~Canvas() {
    if (parent == nullptr) {
        for (std::pair<HANDLE, std::vector<HANDLE>> & pair : vertexBufferHandles) {
            VertexInfo * vertexInfo = vertexEngine->vertexBuffer.get(pair.first);
            for (HANDLE handle : pair.second) {
                vertexInfo->dataBuffer.remove(handle);
            }
        }

        for (HANDLE handle : indexHandles) {
            vertexEngine->removeIndexHandle(handle);
        }
        delete vertexEngine;
    } else {
        // add our handles to parent handles
        // handles will probogate up the call chain to root object
        for (std::pair<HANDLE, std::vector<HANDLE>> & pair : vertexBufferHandles) {
            parent->vertexBufferHandles.push_back(pair);
        }

        for (HANDLE handle : indexHandles) {
            parent->indexHandles.push_back(handle);
        }
        // dont delete vertex engine as it points to parent vertex engine
    }
}

VertexEngine::Canvas VertexEngine::Canvas::subCanvas(int x, int y, int width, int height) {
    return {this, x, y, width, height};
}

void VertexEngine::Canvas::addData(const std::vector<Triple<const Position3&, const Color4&, const TextureCoordinate2&>>& data) {
    for (const Triple<const Position3&, const Color4&, const TextureCoordinate2&> & val : data) {
        checkIfTextureKeyExists(val.second);
        addData_({
            {vertexEngine->defaultPositionBuffer, val.first.to_vector()},
            {vertexEngine->defaultColorBuffer, val.second.to_vector()},
            {vertexEngine->defaultTextureCoordinateBuffer, val.third.to_vector()}
        });
    }
}

HANDLE VertexEngine::Canvas::addIndexData(const std::vector<uint32_t>& data) {
    auto& mutableData = const_cast<std::vector<uint32_t>&>(data);
    for (uint32_t & i : mutableData) i += vertexEngine->indexPosition;
    return addIndexData_(data);
}

VertexEngine::Canvas::Color4 VertexEngine::Canvas::black = {0,0,0,1};
uint32_t VertexEngine::Canvas::black_RGBA_unsigned_32bit_int = VertexEngine::Color4::float_to_RGBA_unsigned_32bit_int(0, 0, 0, 1);
uint32_t VertexEngine::Canvas::black_ARGB_unsigned_32bit_int = VertexEngine::Color4::float_to_ARGB_unsigned_32bit_int(1, 0, 0, 0);

void VertexEngine::Canvas::clear() {
    fill(black);
}

void VertexEngine::Canvas::clear(VertexEngine::Canvas::Color4 color) {
    fill(color);
}

void VertexEngine::Canvas::fill(VertexEngine::Canvas::Color4 color) {
    planeAt(0, 0, width, height, color);
}

void VertexEngine::Canvas::plane(int x, int y, int width, int height, const Color4& colorData) {
    return planeAt(x, y, x + width, y + height, colorData);
}

void VertexEngine::Canvas::planeAt(int from_X, int from_Y, int to_X, int to_Y,
                           const Color4 &colorData) {
    checkIfTextureKeyExists(colorData);
    Position3 topLeft = vertexEngine->toNDC(x + from_X, y + from_Y, 0).to_array();
    Position3 topRight = vertexEngine->toNDC(x + to_X, y + from_Y, 0).to_array();
    Position3 bottomRight = vertexEngine->toNDC(x + to_X, y + to_Y, 0).to_array();
    Position3 bottomLeft = vertexEngine->toNDC(x + from_X, y + to_Y, 0).to_array();

    addData({
        {topLeft, colorData, {0,0}},
        {topRight, colorData, {1,0}},
        {bottomRight, colorData, {1,1}},
        {bottomLeft, colorData, {0,1}}
    });

    addIndexData({
        0, 1, 2,
        2, 3, 0
    });
    vertexEngine->indexPosition += 4;
}

void VertexEngine::Canvas::checkIfTextureKeyExists(const VertexEngine::Canvas::Color4 &colorData) {
    Color4 &c = const_cast<Color4 &>(colorData);
    if (colorData.isTexture == FLOAT_TRUE && std::isnan(c.textureResource)) {
        if (colorData.key == nullptr) {
            LOG_FATAL_ERROR_AND_THROW("key must not be nullptr");
        }
        TextureManager * tm = vertexEngine->textureManager;
        auto textureIndex = tm->findTextureIndex(colorData.key);
        if (!textureIndex.has_value()) {
            LOG_WARNING_MESSAGE(
                    "the key '", colorData.key, "' is not associated to any texture"
            );
            c.textureResource = -1;
        } else {
            auto & texColor = tm->getTexture(textureIndex.value())->third;
            if (texColor.texture != nullptr) {
                // texture is not solid color
                c.textureResource = textureIndex.value();
            } else {
                // texture is solid color
                c.isTexture = FLOAT_FALSE;
                c.textureResource = -1;
                c.red = texColor.red;
                c.green = texColor.green;
                c.blue = texColor.blue;
                c.alpha = texColor.alpha;
            }
        }
    }
}

VertexEngine::GenerationInfo VertexEngine::Canvas::generateGL() {
    return vertexEngine->generateGL();
}

VertexEngine::Canvas::Position2::Position2(const std::array<float, 2> &data) {
    this->x = data[0];
    this->y = data[1];
}

VertexEngine::Canvas::Position2::Position2(const std::array<float, 3> &data) {
    this->x = data[0];
    this->y = data[1];
}

VertexEngine::Canvas::Position2::Position2(const float &x, const float &y) {
    this->x = x;
    this->y = y;
}

std::array<float, 2> VertexEngine::Canvas::Position2::to_array() const {
    return {x, y};
}

std::vector<float> VertexEngine::Canvas::Position2::to_vector() const {
    return {x, y};
}

VertexEngine::Canvas::Position3::Position3(const std::array<float, 3> &data) {
    this->x = data[0];
    this->y = data[1];
    this->z = data[2];
}

VertexEngine::Canvas::Position3::Position3(const float &x, const float &y, const float &z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

std::array<float, 3> VertexEngine::Canvas::Position3::to_array() const {
    return {x, y, z};
}

std::vector<float> VertexEngine::Canvas::Position3::to_vector() const {
    return {x, y, z};
}

VertexEngine::Color4::Color4(const float * data) {
    this->red = data[0];
    this->green = data[1];
    this->blue = data[2];
    this->alpha = data[3];
    this->key = nullptr;
    this->isTexture = false;
    this->textureResource = NAN;
    this->texture = nullptr;
}

VertexEngine::Color4::Color4(const std::array<float, 4> &data) {
    this->red = data[0];
    this->green = data[1];
    this->blue = data[2];
    this->alpha = data[3];
    this->key = nullptr;
    this->isTexture = false;
    this->textureResource = NAN;
    this->texture = nullptr;
}

VertexEngine::Color4::Color4(const std::array<float, 6> &data) {
    this->red = data[0];
    this->green = data[1];
    this->blue = data[2];
    this->alpha = data[3];
    this->key = nullptr;
    this->isTexture = data[4] == FLOAT_FALSE ? FLOAT_FALSE : FLOAT_TRUE;
    this->textureResource = data[5];
    this->texture = nullptr;
}

VertexEngine::Color4::Color4(
        const float &red, const float &green, const float &blue, const float &alpha
) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
    this->key = nullptr;
    this->isTexture = FLOAT_FALSE;
    this->textureResource = NAN;
    this->texture = nullptr;
}

VertexEngine::Color4::Color4(const char * key) {
    this->red = 0;
    this->green = 0;
    this->blue = 0;
    this->alpha = 1;
    this->key = key;
    this->isTexture = FLOAT_TRUE;
    this->textureResource = NAN;
    this->texture = nullptr;
}

VertexEngine::Color4::Color4(Diligent::ITexture* texture) {
    this->red = 0;
    this->green = 0;
    this->blue = 0;
    this->alpha = 1;
    this->key = nullptr;
    this->isTexture = FLOAT_TRUE;
    this->textureResource = NAN;
    this->texture = texture;
}

VertexEngine::Color4::Color4(const std::array<float, 4> & data, Diligent::ITexture* texture) {
    this->red = data[0];
    this->green = data[1];
    this->blue = data[2];
    this->alpha = data[3];
    this->key = nullptr;
    this->isTexture = FLOAT_TRUE;
    this->textureResource = NAN;
    this->texture = texture;
}

std::array<float, 4> VertexEngine::Color4::to_RGBA_array() const {
    return {red, green, blue, alpha};
}

std::array<float, 4> VertexEngine::Color4::to_ARGB_array() const {
    return {alpha, red, green, blue};
}

std::array<float, 6> VertexEngine::Color4::to_array() const {
    return {red, green, blue, alpha, isTexture, textureResource};
}

std::vector<float> VertexEngine::Color4::to_RGBA_vector() const {
    return {red, green, blue, alpha};
}

std::vector<float> VertexEngine::Color4::to_ARGB_vector() const {
    return {alpha, red, green, blue};
}

std::vector<float> VertexEngine::Color4::to_vector() const {
    return {red, green, blue, alpha, isTexture, textureResource};
}

uint32_t VertexEngine::Color4::unsigned_8bit_to_unsigned_32_bit(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    // u32   ff ff ff ff
    // u8[]  a  b  c  d
    uint32_t u32 = a + (b << 8) | (c << 16) + (d << 24);
    return u32;
}

uint32_t VertexEngine::Color4::to_RGBA_unsigned_32bit_int() const {
    return float_to_RGBA_unsigned_32bit_int(red, green, blue, alpha);
}

uint32_t VertexEngine::Color4::to_ARGB_unsigned_32bit_int() const {
    return float_to_ARGB_unsigned_32bit_int(alpha, red, green, blue);
}

std::array<float, 4> VertexEngine::Color4::float_RGBA_to_ARGB_array(const float & red, const float & green, const float & blue, const float & alpha) {
    return {alpha, red, green, blue};
}

std::array<float, 4> VertexEngine::Color4::float_RGBA_array_to_ARGB_array(const std::array<float, 4> & data) {
    return {data[0], data[1], data[2], data[3]};
}

std::array<float, 4> VertexEngine::Color4::float_RGBA_vector_to_ARGB_array(const std::vector<float> & data) {
    return {data[0], data[1], data[2], data[3]};
}

std::vector<float> VertexEngine::Color4::float_RGBA_to_ARGB_vector(const float & red, const float & green, const float & blue, const float & alpha) {
    return {alpha, red, green, blue};
}

std::vector<float> VertexEngine::Color4::float_RGBA_array_to_ARGB_vector(const std::array<float, 4> & data) {
    return {data[0], data[1], data[2], data[3]};
}

std::vector<float> VertexEngine::Color4::float_RGBA_vector_to_ARGB_vector(const std::vector<float> & data) {
    return {data[0], data[1], data[2], data[3]};
}

std::array<float, 4> VertexEngine::Color4::float_ARGB_to_RGBA_array(const float & alpha, const float & red, const float & green, const float & blue) {
    return {red, green, blue, alpha};
}

std::array<float, 4> VertexEngine::Color4::float_ARGB_array_to_RGBA_array(const std::array<float, 4> & data) {
    return {data[1], data[2], data[3], data[0]};
}

std::array<float, 4> VertexEngine::Color4::float_ARGB_vector_to_RGBA_array(const std::vector<float> & data) {
    return {data[1], data[2], data[3], data[0]};
}

std::vector<float> VertexEngine::Color4::float_ARGB_to_RGBA_vector(const float & alpha, const float & red, const float & green, const float & blue) {
    return {red, green, blue, alpha};
}

std::vector<float> VertexEngine::Color4::float_ARGB_array_to_RGBA_vector(const std::array<float, 4> & data) {
    return {data[1], data[2], data[3], data[0]};
}

std::vector<float> VertexEngine::Color4::float_ARGB_vector_to_RGBA_vector(const std::vector<float> & data) {
    return {data[1], data[2], data[3], data[0]};
}

uint32_t VertexEngine::Color4::unsigned_8bit_int_to_RGBA_unsigned_32bit_int(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    return unsigned_8bit_to_unsigned_32_bit(red, green, blue, alpha);
}

uint32_t VertexEngine::Color4::unsigned_8bit_int_to_RGBA_unsigned_32bit_int(const std::array<uint8_t, 4> & data) {
    return unsigned_8bit_int_to_RGBA_unsigned_32bit_int(data[0], data[1], data[2], data[3]);
}

uint32_t VertexEngine::Color4::float_to_RGBA_unsigned_32bit_int(const float & red, const float & green, const float & blue, const float & alpha) {
    return unsigned_8bit_int_to_RGBA_unsigned_32bit_int(static_cast<uint8_t>(255 * red), static_cast<uint8_t>(255 * green), static_cast<uint8_t>(255 * blue), static_cast<uint8_t>(255 * alpha));
}

uint32_t VertexEngine::Color4::float_to_RGBA_unsigned_32bit_int(const std::array<float, 4> & data) {
    return float_to_RGBA_unsigned_32bit_int(data[0], data[1], data[2], data[3]);
}

uint32_t VertexEngine::Color4::unsigned_32bit_int_to_RGBA_unsigned_32bit_int(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha) {
    // To extract each byte, we can mask them using bitwise AND (&)
    // then shift them right to the first byte.
    return unsigned_8bit_int_to_RGBA_unsigned_32bit_int(
            (red & 0xff000000) >> 24,
            (green & 0x00ff0000) >> 16,
            (blue & 0x0000ff00) >> 8,
            alpha & 0x000000ff
    );
}

uint32_t VertexEngine::Color4::unsigned_32bit_int_to_RGBA_unsigned_32bit_int(const std::array<uint32_t, 4> & data) {
    return unsigned_32bit_int_to_RGBA_unsigned_32bit_int(data[0], data[1], data[2], data[3]);
}

uint32_t VertexEngine::Color4::unsigned_8bit_int_to_ARGB_unsigned_32bit_int(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    return unsigned_8bit_to_unsigned_32_bit(alpha, red, green, blue);
}

uint32_t VertexEngine::Color4::unsigned_8bit_int_to_ARGB_unsigned_32bit_int(const std::array<uint8_t, 4> & data) {
    return unsigned_8bit_int_to_ARGB_unsigned_32bit_int(data[0], data[1], data[2], data[3]);
}

uint32_t VertexEngine::Color4::float_to_ARGB_unsigned_32bit_int(const float & red, const float & green, const float & blue, const float & alpha) {
    return unsigned_8bit_int_to_ARGB_unsigned_32bit_int(static_cast<uint8_t>(255 * red), static_cast<uint8_t>(255 * green), static_cast<uint8_t>(255 * blue), static_cast<uint8_t>(255 * alpha));
}

uint32_t VertexEngine::Color4::float_to_ARGB_unsigned_32bit_int(const std::array<float, 4> & data) {
    return float_to_ARGB_unsigned_32bit_int(data[0], data[1], data[2], data[3]);
}

uint32_t VertexEngine::Color4::unsigned_32bit_int_to_ARGB_unsigned_32bit_int(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha) {
    // To extract each byte, we can mask them using bitwise AND (&)
    // then shift them right to the first byte.
    return unsigned_8bit_int_to_ARGB_unsigned_32bit_int(
            (red & 0xff000000) >> 24,
            (green & 0x00ff0000) >> 16,
            (blue & 0x0000ff00) >> 8,
            alpha & 0x000000ff
    );
}

uint32_t VertexEngine::Color4::unsigned_32bit_int_to_ARGB_unsigned_32bit_int(const std::array<uint32_t, 4> & data) {
    return unsigned_32bit_int_to_ARGB_unsigned_32bit_int(data[0], data[1], data[2], data[3]);
}

std::array<float, 4>
VertexEngine::Color4::unsigned_8bit_Component4_to_float_array(uint8_t a, uint8_t b,
                                                                      uint8_t c, uint8_t d) {
    return {static_cast<float>(a) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(c) / 255.0f, static_cast<float>(d) / 255.0f};
}

std::vector<float>
VertexEngine::Color4::unsigned_8bit_Component4_to_float_vector(uint8_t a, uint8_t b,
                                                                       uint8_t c, uint8_t d) {
    return {static_cast<float>(a) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(c) / 255.0f, static_cast<float>(d) / 255.0f};
}

std::array<float, 4> VertexEngine::Color4::unsigned_8bit_array_to_float_array(
        const std::array<uint8_t, 4> &data) {
    return {static_cast<float>(data[0]) / 255.0f, static_cast<float>(data[1]) / 255.0f, static_cast<float>(data[2]) / 255.0f, static_cast<float>(data[3]) / 255.0f};
}

std::vector<float> VertexEngine::Color4::unsigned_8bit_array_to_float_vector(
        const std::array<uint8_t, 4> &data) {
    return {static_cast<float>(data[0]) / 255.0f, static_cast<float>(data[1]) / 255.0f, static_cast<float>(data[2]) / 255.0f, static_cast<float>(data[3]) / 255.0f};
}

std::array<float, 4> VertexEngine::Color4::unsigned_8bit_vector_to_float_array(
        const std::vector<uint8_t> &data) {
    return {static_cast<float>(data[0]) / 255.0f, static_cast<float>(data[1]) / 255.0f, static_cast<float>(data[2]) / 255.0f, static_cast<float>(data[3]) / 255.0f};
}

std::vector<float> VertexEngine::Color4::unsigned_8bit_vector_to_float_vector(
        const std::vector<uint8_t> &data) {
    return {static_cast<float>(data[0]) / 255.0f, static_cast<float>(data[1]) / 255.0f, static_cast<float>(data[2]) / 255.0f, static_cast<float>(data[3]) / 255.0f};
}
