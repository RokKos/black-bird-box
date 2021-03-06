﻿#pragma once

#include "BlackBirdBox/Core/Core.h"
#include "BufferLayout.h"

#include "BlackBirdBox/Primitives/ModelData.h"

#include "glm/glm.hpp"

namespace BlackBirdBox {
class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(const void* data, uint32_t size) = 0;

    virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout& layout) = 0;

    static Ref<VertexBuffer> Create(Vertex* vertices, uint32_t size);
    static Ref<VertexBuffer> Create(glm::vec3* vertices, uint32_t size);
    static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

    static Ref<VertexBuffer> CreateExistingBuffer(uint32_t renderer_id);
};
}
