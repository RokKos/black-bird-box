﻿#include "bbbpch.h"
#include "OpenGLShader.h"

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

namespace Platform {

static GLenum ShaderTypeFromString(const std::string& type)
{
    if (type == "vertex") {
        return GL_VERTEX_SHADER;
    }
    if (type == "fragment" || type == "pixel") {
        return GL_FRAGMENT_SHADER;
    }
    if (type == "compute") {
        return GL_COMPUTE_SHADER;
    }

    CORE_ASSERT(false, "Unknown shader type!");
    return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath)
{
    PROFILE_FUNCTION();

    std::string source = ReadFile(filepath);
    shader_sources_ = PreProcess(source);
    LOG_INFO("Compiling shader from path: {0}", filepath);
    Compile(shader_sources_);

    // Extract name from filepath
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    name_ = filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    : name_(name)
{
    PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    LOG_INFO("Compiling shader: {0}", name);
    Compile(sources);
}

OpenGLShader::~OpenGLShader()
{
    PROFILE_FUNCTION();

    glDeleteProgram(renderer_id_);
}

std::string OpenGLShader::ReadFile(const std::string& filepath)
{
    PROFILE_FUNCTION();

    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        if (size != -1) {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
            in.close();
        } else {
            LOG_ERROR("Could not read from file '{0}'", filepath);
        }
    } else {
        LOG_ERROR("Could not open file '{0}'", filepath);
    }

    return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
{
    PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0); // Start of shader type declaration line
    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
        CORE_ASSERT(eol != std::string::npos, "Syntax error");
        size_t begin = pos + typeTokenLength + 1; // Start of shader type name (after "#type " keyword)
        std::string type = source.substr(begin, eol - begin);
        CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

        size_t nextLinePos = source.find_first_not_of("\r\n", eol); // Start of shader code after shader type declaration line
        CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
        pos = source.find(typeToken, nextLinePos); // Start of next shader type declaration line

        shaderSources[ShaderTypeFromString(type)]
            = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}

void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
{
    PROFILE_FUNCTION();

    GLuint program = glCreateProgram();
    CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
    std::array<GLenum, 2> glShaderIDs;
    int glShaderIDIndex = 0;
    for (auto& kv : shaderSources) {
        GLenum type = kv.first;
        const std::string& source = kv.second;

        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            LOG_ERROR("{0}", infoLog.data());
            CORE_ASSERT(false, "Shader compilation failure!");
            break;
        }

        glAttachShader(program, shader);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    renderer_id_ = program;

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);

        for (auto id : glShaderIDs)
            glDeleteShader(id);

        LOG_ERROR("{0}", infoLog.data());
        CORE_ASSERT(false, "Shader link failure!");
        return;
    }

    for (int i = 0; i < glShaderIDIndex; ++i) {
        auto id = glShaderIDs[i];
        glDetachShader(program, id);
        glDeleteShader(id);
    }
}

GLint OpenGLShader::GetUniformLocation(const std::string& name)
{
    if (uniform_location_cache_.find(name) != uniform_location_cache_.end()) {
        return uniform_location_cache_[name];
    }

    GLint location = glGetUniformLocation(renderer_id_, name.c_str());
    uniform_location_cache_[name] = location;
    return location;
}

void OpenGLShader::Bind() const
{
    PROFILE_FUNCTION();

    glUseProgram(renderer_id_);
}

void OpenGLShader::Unbind() const
{
    PROFILE_FUNCTION();

    glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string& name, int value)
{
    PROFILE_FUNCTION();

    UploadUniformInt(name, value);
}

void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
{
    PROFILE_FUNCTION();

    UploadUniformIntArray(name, values, count);
}

void OpenGLShader::SetFloat(const std::string& name, float value)
{
    PROFILE_FUNCTION();

    UploadUniformFloat(name, value);
}

void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& value)
{
    PROFILE_FUNCTION();

    UploadUniformVec2(name, value);
}

void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value)
{
    PROFILE_FUNCTION();

    UploadUniformVec3(name, value);
}

void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value)
{
    PROFILE_FUNCTION();

    UploadUniformVec4(name, value);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
{
    PROFILE_FUNCTION();

    UploadUniformMat4(name, value);
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value)
{
    PROFILE_FUNCTION();

    GLint location = GetUniformLocation(name);
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
{
    PROFILE_FUNCTION();

    GLint location = GetUniformLocation(name);
    glUniform1iv(location, count, values);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
{
    PROFILE_FUNCTION();

    GLint location = GetUniformLocation(name);
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformVec2(const std::string& name, const glm::vec2& value)
{
    PROFILE_FUNCTION();

    GLint location = GetUniformLocation(name);
    glUniform2f(location, value.x, value.y);
}

void OpenGLShader::UploadUniformVec3(const std::string& name, const glm::vec3& value)
{
    PROFILE_FUNCTION();

    GLint location = GetUniformLocation(name);
    glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniformVec4(const std::string& name, const glm::vec4& value)
{
    PROFILE_FUNCTION();

    GLint location = GetUniformLocation(name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
    PROFILE_FUNCTION();

    GLint location = GetUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    PROFILE_FUNCTION();

    GLint location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

}
