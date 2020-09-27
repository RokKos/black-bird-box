#include "bbbpch.h"
#include "OpenGLTexture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Platform {
OpenGLTexture2D::OpenGLTexture2D(const BlackBirdBox::Texture2DSpecification& specification)
    : specification_(specification)
{
    PROFILE_FUNCTION();

    CreateTexture();
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const BlackBirdBox::Texture2DSpecification& specification)
    : path_(path)
    , specification_(specification)
{
    PROFILE_FUNCTION();

    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = nullptr;
    {
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }
    CORE_ASSERT(data, "Failed to load image!");
    specification_.Width = width;
    specification_.Height = height;

    BlackBirdBox::ImageFormat internalFormat = BlackBirdBox::ImageFormat::RGB8, dataFormat = BlackBirdBox::ImageFormat::RGB;
    if (channels == 4) {
        internalFormat = BlackBirdBox::ImageFormat::RGBA8;
        dataFormat = BlackBirdBox::ImageFormat::RGBA;
    } else if (channels == 3) {
        internalFormat = BlackBirdBox::ImageFormat::RGB8;
        dataFormat = BlackBirdBox::ImageFormat::RGB;
    }

    specification_.InternalFormat = internalFormat;
    specification_.DataFormat = dataFormat;

    CORE_ASSERT(internalFormat != dataFormat, "Format not supported!");

    CreateTexture(data);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    PROFILE_FUNCTION();

    glDeleteTextures(1, &renderer_id_);
}

void OpenGLTexture2D::SetData(void* data, uint32_t size)
{
    PROFILE_FUNCTION();

    uint32_t bpp = specification_.DataFormat == BlackBirdBox::ImageFormat::RGBA ? 4 : 3;
    CORE_ASSERT(size == specification_.Width * specification_.Height * bpp, "Data must be entire texture!");
    glTextureSubImage2D(renderer_id_, 0, 0, 0, specification_.Width, specification_.Height, OpenGLInternalFormat(specification_.DataFormat),
        OpenGLTypeOfPixelData(specification_.TypeOfData), data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    PROFILE_FUNCTION();

    glBindTextureUnit(slot, renderer_id_);
}

void OpenGLTexture2D::CreateTexture(void* data)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
    glBindTexture(GL_TEXTURE_2D, renderer_id_);

    if (data != nullptr) {
        glTextureStorage2D(renderer_id_, 1, OpenGLInternalFormat(specification_.InternalFormat), specification_.Width, specification_.Height);
        glTextureSubImage2D(renderer_id_, 0, 0, 0, specification_.Width, specification_.Height, OpenGLInternalFormat(specification_.DataFormat),
            OpenGLTypeOfPixelData(specification_.TypeOfData), data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, OpenGLInternalFormat(specification_.InternalFormat), specification_.Width, specification_.Height, 0,
            OpenGLInternalFormat(specification_.DataFormat), OpenGLTypeOfPixelData(specification_.TypeOfData), nullptr);
    }

    glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, OpenGLMinifyingFilter(specification_.TextureMinFilter));
    glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, OpenGLMagnificationFilter(specification_.TextureMagFilter));

    glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, OpenGLTextureWraping(specification_.TextureWrapS));
    glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, OpenGLTextureWraping(specification_.TextureWrapT));

    glBindTexture(GL_TEXTURE_2D, 0);
}

GLenum OpenGLTexture2D::OpenGLInternalFormat(BlackBirdBox::ImageFormat internal_format) const
{
    switch (internal_format) {
    case BlackBirdBox::ImageFormat::DEPTH_COMPONENT:
        return GL_DEPTH_COMPONENT;
    case BlackBirdBox::ImageFormat::DEPTH_STENCIL:
        return GL_DEPTH_STENCIL;
    case BlackBirdBox::ImageFormat::RED:
        return GL_RED;
    case BlackBirdBox::ImageFormat::RG:
        return GL_RG;
    case BlackBirdBox::ImageFormat::RGB:
        return GL_RGB;
    case BlackBirdBox::ImageFormat::RGBA:
        return GL_RGBA;
    case BlackBirdBox::ImageFormat::R8:
        return GL_R8;
    case BlackBirdBox::ImageFormat::R16:
        return GL_R16;
    case BlackBirdBox::ImageFormat::RG8:
        return GL_RG8;
    case BlackBirdBox::ImageFormat::RG16:
        return GL_RG16;
    case BlackBirdBox::ImageFormat::R3_G3_B2:
        return GL_R3_G3_B2;
    case BlackBirdBox::ImageFormat::RGB4:
        return GL_RGB4;
    case BlackBirdBox::ImageFormat::RGB5:
        return GL_RGB5;
    case BlackBirdBox::ImageFormat::RGB8:
        return GL_RGB8;
    case BlackBirdBox::ImageFormat::RGB8_SNORM:
        return GL_RGB8_SNORM;
    case BlackBirdBox::ImageFormat::RGB10:
        return GL_RGB10;
    case BlackBirdBox::ImageFormat::RGB12:
        return GL_RGB12;
    case BlackBirdBox::ImageFormat::RGB16:
        return GL_RGB16;
    case BlackBirdBox::ImageFormat::RGBA2:
        return GL_RGBA2;
    case BlackBirdBox::ImageFormat::RGBA4:
        return GL_RGBA4;
    case BlackBirdBox::ImageFormat::RGB5_A1:
        return GL_RGB5_A1;
    case BlackBirdBox::ImageFormat::RGBA8:
        return GL_RGBA8;
    case BlackBirdBox::ImageFormat::RGB10_A2:
        return GL_RGB10_A2;
    case BlackBirdBox::ImageFormat::RGBA12:
        return GL_RGBA12;
    case BlackBirdBox::ImageFormat::RGBA16:
        return GL_RGBA16;
    case BlackBirdBox::ImageFormat::SRGB8_ALPHA8:
        return GL_SRGB8_ALPHA8;
    case BlackBirdBox::ImageFormat::R16F:
        return GL_R16F;
    case BlackBirdBox::ImageFormat::RG16F:
        return GL_RG16F;
    case BlackBirdBox::ImageFormat::RGB16F:
        return GL_RGB16F;
    case BlackBirdBox::ImageFormat::RGBA16F:
        return GL_RGBA16F;
    case BlackBirdBox::ImageFormat::R32F:
        return GL_R32F;
    case BlackBirdBox::ImageFormat::RG32F:
        return GL_RG32F;
    case BlackBirdBox::ImageFormat::RGBA32F:
        return GL_RGBA32F;
    case BlackBirdBox::ImageFormat::R11F_G11F_B10F:
        return GL_R11F_G11F_B10F;
    case BlackBirdBox::ImageFormat::R8I:
        return GL_R8I;
    case BlackBirdBox::ImageFormat::R8UI:
        return GL_R8UI;
    case BlackBirdBox::ImageFormat::R16I:
        return GL_R16I;
    case BlackBirdBox::ImageFormat::R16UI:
        return GL_R16UI;
    case BlackBirdBox::ImageFormat::R32I:
        return GL_R32I;
    case BlackBirdBox::ImageFormat::R32UI:
        return GL_R32UI;
    case BlackBirdBox::ImageFormat::RG8I:
        return GL_RG8I;
    case BlackBirdBox::ImageFormat::RG8UI:
        return GL_RG8UI;
    case BlackBirdBox::ImageFormat::RG16I:
        return GL_RG16I;
    case BlackBirdBox::ImageFormat::RG16UI:
        return GL_RG16UI;
    case BlackBirdBox::ImageFormat::RGB16I:
        return GL_RGB16I;
    case BlackBirdBox::ImageFormat::RGB16UI:
        return GL_RGB16UI;
    case BlackBirdBox::ImageFormat::RGB32I:
        return GL_RGB32I;
    case BlackBirdBox::ImageFormat::RGB32UI:
        return GL_RGB32UI;
    case BlackBirdBox::ImageFormat::RGBA8I:
        return GL_RGBA8I;
    case BlackBirdBox::ImageFormat::RGBA8UI:
        return GL_RGBA8UI;
    case BlackBirdBox::ImageFormat::RGBA16I:
        return GL_RGBA16I;
    case BlackBirdBox::ImageFormat::RGBA16UI:
        return GL_RGBA16UI;
    case BlackBirdBox::ImageFormat::RGBA32I:
        return GL_RGBA32I;
    case BlackBirdBox::ImageFormat::RGBA32UI:
        return GL_RGBA32UI;
    case BlackBirdBox::ImageFormat::DEPTH_COMPONENT16:
        return GL_DEPTH_COMPONENT16;
    case BlackBirdBox::ImageFormat::DEPTH_COMPONENT24:
        return GL_DEPTH_COMPONENT24;
    case BlackBirdBox::ImageFormat::DEPTH_COMPONENT32:
        return GL_DEPTH_COMPONENT32;
    case BlackBirdBox::ImageFormat::DEPTH_COMPONENT32F:
        return GL_DEPTH_COMPONENT32F;
    case BlackBirdBox::ImageFormat::DEPTH24_STENCIL8:
        return GL_DEPTH24_STENCIL8;
    case BlackBirdBox::ImageFormat::DEPTH32F_STENCIL8:
        return GL_DEPTH32F_STENCIL8;
    case BlackBirdBox::ImageFormat::COMPRESSED_RED:
        return GL_COMPRESSED_RED;
    case BlackBirdBox::ImageFormat::COMPRESSED_RG:
        return GL_COMPRESSED_RG;
    case BlackBirdBox::ImageFormat::COMPRESSED_RGB:
        return GL_COMPRESSED_RGB;
    case BlackBirdBox::ImageFormat::COMPRESSED_RGBA:
        return GL_COMPRESSED_RGBA;
    case BlackBirdBox::ImageFormat::COMPRESSED_SRGB:
        return GL_COMPRESSED_SRGB;
    case BlackBirdBox::ImageFormat::COMPRESSED_SRGB_ALPHA:
        return GL_COMPRESSED_SRGB_ALPHA;
    case BlackBirdBox::ImageFormat::COMPRESSED_RED_RGTC1:
        return GL_COMPRESSED_RED_RGTC1;
    case BlackBirdBox::ImageFormat::COMPRESSED_SIGNED_RED_RGTC1:
        return GL_COMPRESSED_SIGNED_RED_RGTC1;
    case BlackBirdBox::ImageFormat::COMPRESSED_RG_RGTC2:
        return GL_COMPRESSED_RG_RGTC2;
    case BlackBirdBox::ImageFormat::COMPRESSED_SIGNED_RG_RGTC2:
        return GL_COMPRESSED_SIGNED_RG_RGTC2;
    }

    CORE_ASSERT(false, "Unknown ImageFormat!");
    return 0;
}

GLenum OpenGLTexture2D::OpenGLMagnificationFilter(BlackBirdBox::TextureMagnificationFilter magnification_filter) const
{
    switch (magnification_filter) {
    case BlackBirdBox::TextureMagnificationFilter::NEAREST:
        return GL_NEAREST;
    case BlackBirdBox::TextureMagnificationFilter::LINEAR:
        return GL_LINEAR;
    }
    CORE_ASSERT(false, "Unknown TextureMagnificationFilter!");
    return 0;
}

GLenum OpenGLTexture2D::OpenGLMinifyingFilter(BlackBirdBox::TextureMinifyingFilter minifying_filter) const
{
    switch (minifying_filter) {
    case BlackBirdBox::TextureMinifyingFilter::NEAREST:
        return GL_NEAREST;
    case BlackBirdBox::TextureMinifyingFilter::LINEAR:
        return GL_LINEAR;
    case BlackBirdBox::TextureMinifyingFilter::NEAREST_MIPMAP_NEAREST:
        return GL_NEAREST_MIPMAP_NEAREST;
    case BlackBirdBox::TextureMinifyingFilter::LINEAR_MIPMAP_NEAREST:
        return GL_LINEAR_MIPMAP_NEAREST;
    case BlackBirdBox::TextureMinifyingFilter::NEAREST_MIPMAP_LINEAR:
        return GL_NEAREST_MIPMAP_LINEAR;
    case BlackBirdBox::TextureMinifyingFilter::LINEAR_MIPMAP_LINEAR:
        return GL_LINEAR_MIPMAP_LINEAR;
    }

    CORE_ASSERT(false, "Unknown TextureMinifyingFilter!");
    return 0;
}

GLenum OpenGLTexture2D::OpenGLTextureWraping(BlackBirdBox::TextureWraping texuture_wraping) const
{
    switch (texuture_wraping) {
    case BlackBirdBox::TextureWraping::CLAMP_TO_EDGE:
        return GL_CLAMP_TO_EDGE;
    case BlackBirdBox::TextureWraping::CLAMP_TO_BORDER:
        return GL_CLAMP_TO_BORDER;
    case BlackBirdBox::TextureWraping::MIRRORED_REPEAT:
        return GL_MIRRORED_REPEAT;
    case BlackBirdBox::TextureWraping::REPEAT:
        return GL_REPEAT;
    case BlackBirdBox::TextureWraping::MIRROR_CLAMP_TO_EDGE:
        return GL_MIRROR_CLAMP_TO_EDGE;
    }

    CORE_ASSERT(false, "Unknown TextureWraping!");
    return 0;
}

GLenum OpenGLTexture2D::OpenGLTypeOfPixelData(BlackBirdBox::TypeOfPixelData type) const
{
    switch (type) {
    case BlackBirdBox::TypeOfPixelData::UNSIGNED_BYTE:
        return GL_UNSIGNED_BYTE;
    case BlackBirdBox::TypeOfPixelData::BYTE:
        return GL_BYTE;
    case BlackBirdBox::TypeOfPixelData::UNSIGNED_SHORT:
        return GL_UNSIGNED_SHORT;
    case BlackBirdBox::TypeOfPixelData::SHORT:
        return GL_SHORT;
    case BlackBirdBox::TypeOfPixelData::UNSIGNED_INT:
        return GL_UNSIGNED_INT;
    case BlackBirdBox::TypeOfPixelData::INT:
        return GL_INT;
    case BlackBirdBox::TypeOfPixelData::HALF_FLOAT:
        return GL_HALF_FLOAT;
    case BlackBirdBox::TypeOfPixelData::FLOAT:
        return GL_FLOAT;
    case BlackBirdBox::TypeOfPixelData::UNSIGNED_SHORT_5_6_5:
        return GL_UNSIGNED_SHORT_5_6_5;
    case BlackBirdBox::TypeOfPixelData::UNSIGNED_SHORT_4_4_4_4:
        return GL_UNSIGNED_SHORT_4_4_4_4;
    case BlackBirdBox::TypeOfPixelData::UNSIGNED_SHORT_5_5_5_1:
        return GL_UNSIGNED_SHORT_5_5_5_1;
    case BlackBirdBox::TypeOfPixelData::UNSIGNED_INT_2_10_10_10_REV:
        return GL_UNSIGNED_INT_2_10_10_10_REV;
    case BlackBirdBox::TypeOfPixelData::UNSIGNED_INT_10F_11F_11F_REV:
        return GL_UNSIGNED_INT_10F_11F_11F_REV;
    case BlackBirdBox::TypeOfPixelData::UNSIGNED_INT_5_9_9_9_REV:
        return GL_UNSIGNED_INT_5_9_9_9_REV;
    case BlackBirdBox::TypeOfPixelData::UNSIGNED_INT_24_8:
        return GL_UNSIGNED_INT_24_8;
    case BlackBirdBox::TypeOfPixelData::FLOAT_32_UNSIGNED_INT_24_8_REV:
        return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
    }

    CORE_ASSERT(false, "Unknown TypeOfPixelData!");
    return 0;
}

}
