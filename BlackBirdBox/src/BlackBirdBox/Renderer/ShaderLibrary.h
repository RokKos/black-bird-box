#pragma once

#include "BlackBirdBox/Core/Core.h"
#include "BlackBirdBox/Renderer/Shader.h"

namespace Core {

class ShaderLibrary {
public:
    void Add(const std::string& name, const Ref<Shader>& shader);
    void Add(const Ref<Shader>& shader);
    Ref<Shader> Load(const std::string& filepath);
    Ref<Shader> Load(const std::string& name, const std::string& filepath);

    const Ref<Shader>& Get(const std::string& name);

    bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};

}
