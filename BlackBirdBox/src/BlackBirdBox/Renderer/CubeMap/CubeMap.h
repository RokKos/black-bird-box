#pragma once

#include "BlackBirdBox/Core/Core.h"

namespace BlackBirdBox {

class CubeMap {
public:
    virtual ~CubeMap() = default;

    virtual void SetCubeMap(const std::string& path) = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;

    static Ref<CubeMap> Create(const std::string& path);
};

}
