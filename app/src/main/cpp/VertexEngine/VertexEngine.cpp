//
// Created by Matthew Good on 17/7/21.
//

#include "VertexEngine.h"
#include <cmath>
#include <DiligentTools/TextureLoader/interface/TextureUtilities.h>

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
storage_type *VertexEngine::Buffer<data_type, storage_type>::get(const size_t & index) {
    return kernel.table->objectAt(index)->resource.template get<storage_type*>();
}

template<typename data_type, typename storage_type>
size_t VertexEngine::Buffer<data_type, storage_type>::getIndex(HANDLE handle) {
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

VertexEngine::VertexEngine(int width, int height) :
        defaultPositionBuffer(addDataBuffer(positionCount)),
        defaultColorBuffer(addDataBuffer(colorCount+colorExtraCount)),
        defaultTextureCoordinateBuffer(addDataBuffer(textureCoordinatesCount)),
        width(width),
        height(height),
        indexPosition(0),
        textureManager(this)
{
    order({defaultPositionBuffer, defaultColorBuffer, defaultTextureCoordinateBuffer});
}

VertexEngine::VertexEngine() :
        VertexEngine::VertexEngine(0,0)
{
}

VertexEngine::~VertexEngine() {
    clear();
}

void VertexEngine::resize(int width, int height) {
    this->width = width;
    this->height = height;
}

void VertexEngine::clear() {
    auto i = vertexBuffer.getIterator();
    while (i.hasNext()) {
        auto v = vertexBuffer.get(i.next()->handle);
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

VertexEngine::GenerationInfo::GenerationInfo(
        const size_t lengthData, const float *data,
        const size_t lengthIndices, const uint32_t *indices,
        const bool isTexture
) :
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
        const size_t lengthData, const float *data,
        const size_t lengthIndices, const uint32_t *indices
) : GenerationInfo(lengthData, data, lengthIndices, indices, false)
{
}

VertexEngine::GenerationInfo::GenerationInfo(VertexEngine::GenerationInfo &&m) :
        data(m.data),
        lengthData(m.lengthData),
        sizeInBytesData(m.sizeInBytesData),
        indices(m.indices),
        lengthIndices(m.lengthIndices),
        sizeInBytesIndices(m.sizeInBytesIndices),
        chunkReader(0),
        chunksGenerated(0)
{
    m.data = nullptr;
    m.indices = nullptr;
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
    VertexEngine::GenerationInfo out(strideLength*chunkSize, nullptr, chunkSize, nullptr, true);

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
            out.textureIndex = firstTextureIndexIsNan ? -1 : data[textureIndexIndex];
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

        return GenerationInfo(lengthData, dataBuffer, lengthIndices, indicesBuffer);
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

        return GenerationInfo(lengthData, dataBuffer, lengthIndices, indicesBuffer);
    }
}

VertexEngine::TextureManager::TextureManager(VertexEngine *engine):
        vertexEngine(engine)
{
}

void VertexEngine::TextureManager::createTextureFromFile(
        const char * key,
        const char *FilePath,
        const Diligent::TextureLoadInfo &TexLoadInfo,
        Diligent::IRenderDevice *pDevice
) {
    Diligent::ITexture * tex = nullptr;
    Diligent::CreateTextureFromFile(FilePath, TexLoadInfo, pDevice, &tex);
    vertexEngine->textureBuffer.add({key, strlen(key), tex});
}

Diligent::ITexture * VertexEngine::TextureManager::findTexture(const char * key) {
    if (key == nullptr) return nullptr;
    auto i = vertexEngine->textureBuffer.getIterator();
    while (i.hasNext()) {
        HANDLE h = i.next()->handle;
        auto r = vertexEngine->textureBuffer.get(h);
        if (memcmp(key, r->first, r->second) == 0) return r->third;
    }
    return nullptr;
}

VertexEngine::Triple<const char *, size_t, Diligent::ITexture *> *
VertexEngine::TextureManager::getTexture(const size_t & index) {
    if (index == -1) return nullptr;
    VertexEngine::Triple<const char *, size_t, Diligent::ITexture *> * r = vertexEngine->textureBuffer.get(index);
    return r == nullptr ? nullptr : r;
}

size_t VertexEngine::TextureManager::findTextureIndex(const char * key) {
    auto i = vertexEngine->textureBuffer.getIterator();
    while (i.hasNext()) {
        auto idx = vertexEngine->textureBuffer.getIndex(i.next()->handle);
        auto r = vertexEngine->textureBuffer.get(idx);
        if (memcmp(key, r->first, r->second) == 0) return idx;
    }
    return -1;
}

void VertexEngine::TextureManager::deleteTexture(const char * key) {
    Diligent::ITexture * texture = findTexture(key);
    if (texture != nullptr) {
        texture->Release();
    }
}

HANDLE VertexEngine::Canvas::addData_(HANDLE dataBufferHandle, const std::vector<float>& data) {
    VertexInfo * buffer = vertexEngine->vertexBuffer.get(dataBufferHandle);
    size_t dataLength = data.size();
    size_t bufferLength = buffer->length;
    if (dataLength > bufferLength) {
        LOG_FATAL_ERROR_AND_THROW("data length (", dataLength, ") is greater than buffer length (", bufferLength, ")");
    }
    if (buffer->is_static) {
        if (buffer->static_data != nullptr) {
            buffer->dataBuffer.remove(buffer->static_data);
        }
        buffer->static_data = buffer->dataBuffer.add(data);
        return buffer->static_data;
    } else {
        return buffer->dataBuffer.add(data);
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
    return handle;
}

VertexEngine::Canvas::Canvas(VertexEngine *engine, int x, int y, int width, int height) :
    x(x), y(y), width(width), height(height),
    vertexEngine(engine)
{}

VertexEngine::Canvas::Canvas(VertexEngine *engine, int width, int height) :
        VertexEngine::Canvas(engine, 0, 0, width, height)
{
}

VertexEngine::Canvas VertexEngine::Canvas::subCanvas(int x, int y, int width, int height) {
    int actualX = this->x + x;
    int actualY = this->y + y;
    int actualWidth = actualX + width;
    int actualHeight = actualY + height;

    if(actualWidth > this->width) {
        LOG_FATAL_ERROR_AND_THROW("subcanvas actual width (", actualWidth, ") is greater than canvas width (", this->width, ")");
    }
    if(actualHeight > this->height) {
        LOG_FATAL_ERROR_AND_THROW("subcanvas actual height (", actualHeight, ") is greater than canvas height (", this->height, ")");
    }

    return VertexEngine::Canvas(vertexEngine, x, y, width, height);
}

void VertexEngine::Canvas::addData(const std::vector<Triple<const Position3&, const Color4&, const TextureCoordinate2&>>& data) {
    for (const Triple<const Position3&, const Color4&, const TextureCoordinate2&> & val : data) {
        processTextureInColor4(val.second);
        addData_({
            {vertexEngine->defaultPositionBuffer, val.first.toVector()},
            {vertexEngine->defaultColorBuffer, val.second.toVector()},
            {vertexEngine->defaultTextureCoordinateBuffer, val.third.toVector()}
        });
    }
}

HANDLE VertexEngine::Canvas::addIndexData(const std::vector<uint32_t>& data) {
    auto& mutableData = const_cast<std::vector<uint32_t>&>(data);
    for (uint32_t & i : mutableData) i += vertexEngine->indexPosition;
    return addIndexData_(data);
}

VertexEngine::Canvas::Color4 VertexEngine::Canvas::black = {0,0,0,1};

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
    processTextureInColor4(colorData);
    Position3 topLeft = vertexEngine->toNDC(x + from_X, y + from_Y, 0).toArray();
    Position3 topRight = vertexEngine->toNDC(x + to_X, y + from_Y, 0).toArray();
    Position3 bottomRight = vertexEngine->toNDC(x + to_X, y + to_Y, 0).toArray();
    Position3 bottomLeft = vertexEngine->toNDC(x + from_X, y + to_Y, 0).toArray();

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

void VertexEngine::Canvas::processTextureInColor4(const VertexEngine::Canvas::Color4 &colorData) {
    Color4 &c = const_cast<Color4 &>(colorData);
    if (colorData.isTexture == FLOAT_TRUE && std::isnan(c.textureResource)) {
        if (colorData.key == nullptr) {
            LOG_FATAL_ERROR_AND_THROW("key must not be nullptr");
        }
        size_t idx = vertexEngine->textureManager.findTextureIndex(colorData.key);
        if (idx == -1) {
            c.key = nullptr;
            c.isTexture = FLOAT_FALSE;
            c.textureResource = NAN;
        } else {
            c.textureResource = idx;
        }
    }
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

std::array<float, 2> VertexEngine::Canvas::Position2::toArray() const {
    return {x, y};
}

std::vector<float> VertexEngine::Canvas::Position2::toVector() const {
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

std::array<float, 3> VertexEngine::Canvas::Position3::toArray() const {
    return {x, y, z};
}

std::vector<float> VertexEngine::Canvas::Position3::toVector() const {
    return {x, y, z};
}

VertexEngine::Canvas::Color4::Color4(const std::array<float, 4> &data) {
    this->r = data[0];
    this->g = data[1];
    this->b = data[2];
    this->a = data[3];
    this->key = nullptr;
    this->isTexture = false;
    this->textureResource = NAN;
}

VertexEngine::Canvas::Color4::Color4(const std::array<float, 6> &data) {
    this->r = data[0];
    this->g = data[1];
    this->b = data[2];
    this->a = data[3];
    this->key = nullptr;
    this->isTexture = data[4] == FLOAT_FALSE ? FLOAT_FALSE : FLOAT_TRUE;
    this->textureResource = data[5];
}

VertexEngine::Canvas::Color4::Color4(const float &r, const float &g, const float &b,
                                     const float &a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    this->key = nullptr;
    this->isTexture = FLOAT_FALSE;
    this->textureResource = NAN;
}

VertexEngine::Canvas::Color4::Color4(const char * key) {
    this->r = 0;
    this->g = 0;
    this->b = 0;
    this->a = 1;
    this->key = key;
    this->isTexture = FLOAT_TRUE;
    this->textureResource = NAN;
}

std::array<float, 4> VertexEngine::Canvas::Color4::toRGBAArray() const {
    return {r, g, b, a};
}

std::array<float, 6> VertexEngine::Canvas::Color4::toArray() const {
    return {r, g, b, a, isTexture, textureResource};
}

std::vector<float> VertexEngine::Canvas::Color4::toRGBAVector() const {
    return {r, g, b, a};
}

std::vector<float> VertexEngine::Canvas::Color4::toVector() const {
    return {r, g, b, a, isTexture, textureResource};
}
