#include "bbbpch.h"
#include "ShaderLibrary.h"

namespace BlackBirdBox {

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
    PROFILE_FUNCTION();

    CORE_ASSERT(!Exists(name), "Shader already exists!");
    m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
    PROFILE_FUNCTION();

    auto& name = shader->GetName();
    Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
{
    PROFILE_FUNCTION();

    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
{
    PROFILE_FUNCTION();

    auto shader = Shader::Create(filepath);
    Add(name, shader);
    return shader;
}

const Ref<Shader>& ShaderLibrary::Get(const std::string& name)
{
    PROFILE_FUNCTION();

    CORE_ASSERT(Exists(name), "Shader not found!");
    return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const
{
    PROFILE_FUNCTION();

    return m_Shaders.find(name) != m_Shaders.end();
}
}
