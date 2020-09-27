#pragma once

#include "BlackBirdBox/Renderer/Texture/Texture2D.h"

#include <GL/glew.h>

namespace Platform {

class OpenGLTexture2D : public BlackBirdBox::Texture2D {
public:
    OpenGLTexture2D(const BlackBirdBox::Texture2DSpecification& specification);
    OpenGLTexture2D(const std::string& path, const BlackBirdBox::Texture2DSpecification& specification);
    virtual ~OpenGLTexture2D();

    virtual uint32_t GetRenderID() const override { return renderer_id_; }
    virtual uint32_t GetWidth() const override { return specification_.Width; }
    virtual uint32_t GetHeight() const override { return specification_.Height; }

    virtual void SetData(void* data, uint32_t size) override;

    virtual void Bind(uint32_t slot = 0) const override;

    virtual bool operator==(const Texture& other) const override { return renderer_id_ == ((OpenGLTexture2D&)other).renderer_id_; }

private:
    void CreateTexture(void* data = nullptr);

    GLenum OpenGLInternalFormat(BlackBirdBox::ImageFormat internal_format) const;
    GLenum OpenGLMagnificationFilter(BlackBirdBox::TextureMagnificationFilter magnification_filter) const;
    GLenum OpenGLMinifyingFilter(BlackBirdBox::TextureMinifyingFilter minifying_filter) const;
    GLenum OpenGLTextureWraping(BlackBirdBox::TextureWraping texuture_wraping) const;
    GLenum OpenGLTypeOfPixelData(BlackBirdBox::TypeOfPixelData type) const;

private:
    std::string path_;
    uint32_t renderer_id_;
    BlackBirdBox::Texture2DSpecification specification_;
};

}
