//
// Created by Matthew Good on 17/7/21.
//

#include "VertexEngine.h"

template<typename storage_type, typename data_type>
HANDLE VertexEngine::Buffer<storage_type, data_type>::add(data_type data) {
    return kernel.newHandle(ObjectTypeNone, storage_type(data));
}

template<typename storage_type, typename data_type>
storage_type *VertexEngine::Buffer<storage_type, data_type>::get(HANDLE handle) {
    return kernel.getHandle(handle)->object->resource.template get<storage_type*>();
}

template<typename storage_type, typename data_type>
void VertexEngine::Buffer<storage_type, data_type>::remove(HANDLE handle) {
    kernel.removeHandle(handle);
}

template<typename storage_type, typename data_type>
Table::Iterator VertexEngine::Buffer<storage_type, data_type>::getIterator() {
    return kernel.table->getIterator();
}

VertexEngine::VertexInfo::VertexInfo(size_t length, bool is_static) {
    this->length = length;
    this->is_static = is_static;
}

VertexEngine::VertexEngine(int width, int height) {
    resize(width, height);
}

void VertexEngine::resize(int width, int height) {
    this->width = width;
    this->height = height;
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

HANDLE VertexEngine::addData(HANDLE dataBufferHandle, std::initializer_list<float> data) {
    VertexInfo * buffer = vertexBuffer.get(dataBufferHandle);
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

void VertexEngine::order(std::initializer_list<HANDLE> data) {
    vertexBufferOrder = data;
}

VertexEngine::GenerationInfo::GenerationInfo(const size_t length, const float *data) :
    data(new float[length]),
    length(length),
    sizeInBytes(length * sizeof(float))
{
    memcpy(this->data, data, sizeInBytes);
}

VertexEngine::GenerationInfo::~GenerationInfo() {
    delete[] data;
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

    size_t index = 0;

    if (isAllStatic) {
        // if we have all static, then every group contains only a single buffer
        //
        // ORDER A, B, C
        // OUT A.data, B.data, C.data

        // compute buffer size
        size_t bufferSize = 0;

        for (HANDLE group : vertexBufferOrder) {
            bufferSize += vertexBuffer.get(group)->length;
        }

        float buffer[bufferSize];

        // fill buffer
        for (HANDLE group : vertexBufferOrder) {
            VertexInfo *info = vertexBuffer.get(group);
            std::initializer_list<float> *data = info->dataBuffer.get(info->static_data);
            for (size_t i = 0; i < data->size(); i++) {
                buffer[index++] = *(data->begin() + i);
            }
        }

        return GenerationInfo(index, buffer);
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

        // compute buffer size
        size_t bufferSize = 0;

        for (size_t c = 0; c < bufferCount; c++) {
            for (HANDLE group : vertexBufferOrder) {
                VertexInfo *info = vertexBuffer.get(group);
                Table::Iterator iterator = info->dataBuffer.getIterator();
                if (!info->is_static) iterator.skip(c);
                if (iterator.hasNext()) bufferSize += info->length;
            }
        }

        float buffer[bufferSize];

        // fill buffer
        for (size_t c = 0; c < bufferCount; c++) {
            for (HANDLE group : vertexBufferOrder) {
                VertexInfo *info = vertexBuffer.get(group);
                Table::Iterator iterator = info->dataBuffer.getIterator();
                if (!info->is_static) iterator.skip(c);
                if (iterator.hasNext()) {
                    std::initializer_list<float> *data = info->dataBuffer.get(iterator.next()->handle);
                    for (size_t i = 0; i < data->size(); i++) {
                        buffer[index++] = *(data->begin() + i);
                    }
                }
            }
        }

        return GenerationInfo(index, buffer);
    }
}
