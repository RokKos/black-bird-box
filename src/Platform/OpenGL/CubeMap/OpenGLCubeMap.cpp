#include "bbbpch.h"
#include "OpenGLCubeMap.h"

#include <stb_image.h>

namespace Platform {


	OpenGLCubeMap::OpenGLCubeMap(const std::string& path)
	{
		glGenTextures(1, &renderer_id_);
		glBindTexture(GL_TEXTURE_CUBE_MAP, renderer_id_);

		std::vector<std::pair<std::string, GLenum>> cube_map_paths_enums{
			std::make_pair("Right.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X),
			std::make_pair("Left.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X),
			std::make_pair("Top.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y),
			std::make_pair("Bottom.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y),
			std::make_pair("Front.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z),
			std::make_pair("Back.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) };

		for (const auto& path_enum : cube_map_paths_enums) {
			std::string side_of_cube_path = path + path_enum.first;
			LoadOneTextureOfCubeMap(side_of_cube_path, path_enum.second);
		}
		

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	OpenGLCubeMap::~OpenGLCubeMap()
	{
		glDeleteTextures(1, &renderer_id_);
	}

	void OpenGLCubeMap::SetCubeMap(const std::string& path)
	{

	}

	void OpenGLCubeMap::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, renderer_id_);
	}



	void OpenGLCubeMap::LoadOneTextureOfCubeMap(const std::string& path, GLenum side_of_cube_map)
	{
		
		int width, height, channels;
		stbi_set_flip_vertically_on_load(0);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		CORE_ASSERT(data, "Failed to load image!");

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glTexImage2D(side_of_cube_map, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

}