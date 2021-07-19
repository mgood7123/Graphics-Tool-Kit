//
// Created by Matthew Good on 17/7/21.
//

#include "VertexEngine.h"

template<typename data_type, typename storage_type>
HANDLE VertexEngine::Buffer<data_type, storage_type>::add(data_type data) {
    return kernel.newHandle(ObjectTypeNone, storage_type(data));
}

template<typename data_type, typename storage_type>
storage_type *VertexEngine::Buffer<data_type, storage_type>::get(HANDLE handle) {
    return kernel.getHandle(handle)->object->resource.template get<storage_type*>();
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

VertexEngine::VertexEngine() :
    canvas(this),
    defaultPositionBuffer(nullptr),
    defaultColorBuffer(nullptr),
    width(0),
    height(0),
    indexPosition(0)
{
}

VertexEngine::VertexEngine(int width, int height) :
        canvas(this),
        defaultPositionBuffer(nullptr),
        defaultColorBuffer(nullptr),
        width(width),
        height(height),
        indexPosition(0)
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
        const size_t lengthIndices, const uint32_t *indices
    ) :
    data(new float[lengthData]),
    lengthData(lengthData),
    sizeInBytesData(lengthData * sizeof(float)),
    indices(new uint32_t[lengthIndices]),
    lengthIndices(lengthIndices),
    sizeInBytesIndices(lengthIndices * sizeof(uint32_t)),
    chunkReader(0),
    chunksGenerated(0)
{
    if (data != nullptr) memcpy(this->data, data, sizeInBytesData);
    else memset(this->data, 0, sizeInBytesData);
    if (indices != nullptr) memcpy(this->indices, indices, sizeInBytesIndices);
    else memset(this->indices, 0, sizeInBytesIndices);
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

VertexEngine::GenerationInfo VertexEngine::GenerationInfo::generateChunk(
        int posCount, int colorCount, size_t chunkSize
) {
    int size = posCount + colorCount;

    VertexEngine::GenerationInfo out(size*chunkSize, nullptr, chunkSize, nullptr);

    // indices increase from 0, 1, 2, ...
    size_t vertexIndex = 0;
    size_t indicesIndex = 0;
    while(indicesIndex < chunkSize && chunkReader < lengthIndices) {
        out.indices[indicesIndex] = indicesIndex;
        indicesIndex++;

        uint32_t target = indices[chunkReader];
        chunkReader++;

        uint32_t index = target * size;

        for (int i = 0; i < size; i++) {
            out.data[vertexIndex++] = data[index+i];
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

VertexEngine::Canvas::Canvas(VertexEngine *engine) {
    vertexEngine = engine;
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

void VertexEngine::Canvas::addData(const std::vector<std::pair<const Position3&, const Color4&>>& data) {
    for (std::pair<const Position3&, const Color4&> pair : data) {
        const Position3& position = pair.first;
        const Color4& color = pair.second;
        addData_({
            {vertexEngine->defaultPositionBuffer, position.toVector()},
            {vertexEngine->defaultColorBuffer, color.toVector()}
        });
    }
}

HANDLE VertexEngine::Canvas::addIndexData(const std::vector<uint32_t>& data) {
    auto& mutableData = const_cast<std::vector<uint32_t>&>(data);
    for (uint32_t & i : mutableData) i += vertexEngine->indexPosition;
    return addIndexData_(data);
}

void VertexEngine::Canvas::clear() {
    vertexEngine->clear();

}

void VertexEngine::Canvas::plane(int x, int y, int width, int height, const Color4& colorData) {
    return planeAt(x, y, x + width, y + height, colorData);
}

void VertexEngine::Canvas::planeAt(int from_X, int from_Y, int to_X, int to_Y,
                           const Color4 &colorData) {
    Position3 topLeft = vertexEngine->toNDC(from_X, from_Y, 0).toArray();
    Position3 topRight = vertexEngine->toNDC(to_X, from_Y, 0).toArray();
    Position3 bottomRight = vertexEngine->toNDC(to_X, to_Y, 0).toArray();
    Position3 bottomLeft = vertexEngine->toNDC(from_X, to_Y, 0).toArray();

    addData({
        {topLeft, colorData},
        {topRight, colorData},
        {bottomRight, colorData},
        {bottomLeft, colorData}
    });

    addIndexData({
        0, 1, 2,
        2, 3, 0
    });
    vertexEngine->indexPosition += 4;
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
}

VertexEngine::Canvas::Color4::Color4(const float &r, const float &g, const float &b,
                                     const float &a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

std::array<float, 4> VertexEngine::Canvas::Color4::toArray() const {
    return {r, g, b, a};
}

std::vector<float> VertexEngine::Canvas::Color4::toVector() const {
    return {r, g, b, a};
}
