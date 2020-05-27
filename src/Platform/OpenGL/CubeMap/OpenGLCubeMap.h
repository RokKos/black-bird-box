#pragma once

#include "BlackBirdBox/Renderer/CubeMap/CubeMap.h"

#include <GL/glew.h>

namespace Platform {
	class OpenGLCubeMap : public Core::CubeMap
	{
	public:
		OpenGLCubeMap(const std::string& path);
		virtual ~OpenGLCubeMap();

		virtual void SetCubeMap(const std::string& path) override;

		virtual void Bind(uint32_t slot = 0) const override;


	private:
		void LoadOneTextureOfCubeMap(const std::string& path, GLenum side_of_cube_map);
	private:
		std::string path_;
		uint32_t renderer_id_;
	};

}