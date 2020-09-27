#pragma once

#include "BlackBirdBox/Core/Core.h"

#include "glm/glm.hpp"

namespace BlackBirdBox {
class ShaderStorageBuffer {
public:
    virtual ~ShaderStorageBuffer() = default;

    virtual uint32_t GetRendererID() const = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(const std::vector<glm::vec3>& storage_data, uint32_t size) = 0;
    virtual void SetData(const std::vector<glm::vec4>& storage_data, uint32_t size) = 0;
    virtual void SetData(const std::vector<glm::mat3>& storage_data, uint32_t size) = 0;
    virtual void SetData(const std::vector<glm::mat4>& storage_data, uint32_t size) = 0;
    virtual void SetData(const std::vector<glm::int32>& storage_data, uint32_t size) = 0;

    virtual void SetPersistentData(const std::vector<glm::vec4>& storage_data, uint32_t size) = 0;
    virtual void SetPersistentDataIndex(const glm::vec4& value, size_t index) = 0;

    virtual std::vector<glm::vec3> GetData(uint32_t size) = 0;
    // virtual std::vector<glm::vec4> GetData(uint32_t size) = 0;

    static Ref<ShaderStorageBuffer> Create(uint32_t size);
    static Ref<ShaderStorageBuffer> Create(const std::vector<glm::vec3>& storage_data, uint32_t size);
    static Ref<ShaderStorageBuffer> Create(const std::vector<glm::vec4>& storage_data, uint32_t size, bool persistent);
    static Ref<ShaderStorageBuffer> Create(const std::vector<glm::mat3>& storage_data, uint32_t size);
    static Ref<ShaderStorageBuffer> Create(const std::vector<glm::mat4>& storage_data, uint32_t size);
    static Ref<ShaderStorageBuffer> Create(const std::vector<glm::int32>& storage_data, uint32_t size);
};
}
