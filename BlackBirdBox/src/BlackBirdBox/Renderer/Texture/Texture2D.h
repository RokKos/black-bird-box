#pragma once

#include "Texture.h"

#include "BlackBirdBox/Core/Core.h"

namespace Core {

	enum class TextureMinifyingFilter
	{
		NEAREST = 0,
		LINEAR = 1,
		NEAREST_MIPMAP_NEAREST = 2,
		LINEAR_MIPMAP_NEAREST = 3,
		NEAREST_MIPMAP_LINEAR = 4,
		LINEAR_MIPMAP_LINEAR = 5,
	};

	enum class TextureMagnificationFilter
	{
		NEAREST = 0,
		LINEAR = 1,
	};

	enum class TextureWraping
	{
		CLAMP_TO_EDGE = 0, 
		CLAMP_TO_BORDER = 1, 
		MIRRORED_REPEAT = 2, 
		REPEAT = 3, 
		MIRROR_CLAMP_TO_EDGE = 4
	};

	enum class ImageFormat 
	{
		DEPTH_COMPONENT,
		DEPTH_STENCIL,
		RED,
		RG,
		RGB,
		RGBA,
		R8,
		R16,
		RG8,
		RG16,
		R3_G3_B2,
		RGB4,
		RGB5,
		RGB8,
		RGB8_SNORM,
		RGB10,
		RGB12,
		RGB16,
		RGBA2,
		RGBA4,
		RGB5_A1,
		RGBA8,
		RGB10_A2,
		RGBA12,
		RGBA16,
		SRGB8_ALPHA8,
		R16F,
		RG16F,
		RGB16F,
		RGBA16F,
		R32F,
		RG32F,
		RGBA32F,
		R11F_G11F_B10F,
		R8I,
		R8UI,
		R16I,
		R16UI,
		R32I,
		R32UI,
		RG8I,
		RG8UI,
		RG16I,
		RG16UI,
		RGB16I,
		RGB16UI,
		RGB32I,
		RGB32UI,
		RGBA8I,
		RGBA8UI,
		RGBA16I,
		RGBA16UI,
		RGBA32I,
		RGBA32UI,
		DEPTH_COMPONENT16,
		DEPTH_COMPONENT24,
		DEPTH_COMPONENT32,
		DEPTH_COMPONENT32F,
		DEPTH24_STENCIL8,
		DEPTH32F_STENCIL8,
		COMPRESSED_RED,
		COMPRESSED_RG,
		COMPRESSED_RGB,
		COMPRESSED_RGBA,
		COMPRESSED_SRGB,
		COMPRESSED_SRGB_ALPHA,
		COMPRESSED_RED_RGTC1,
		COMPRESSED_SIGNED_RED_RGTC1,
		COMPRESSED_RG_RGTC2,
		COMPRESSED_SIGNED_RG_RGTC2,
	};
		
	struct Texture2DSpecification
	{
		uint32_t Width;
		uint32_t Height;
		TextureMinifyingFilter TextureMinFilter = TextureMinifyingFilter::NEAREST_MIPMAP_LINEAR;
		TextureMagnificationFilter TextureMagFilter = TextureMagnificationFilter::LINEAR;
		TextureWraping TextureWrapS = TextureWraping::REPEAT;
		TextureWraping TextureWrapT = TextureWraping::REPEAT;
		ImageFormat InternalFormat = ImageFormat::RGBA8;
		ImageFormat DataFormat = ImageFormat::RGBA;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const Texture2DSpecification& specification);
		static Ref<Texture2D> Create(const std::string& path, const Texture2DSpecification& specification);
	};

}