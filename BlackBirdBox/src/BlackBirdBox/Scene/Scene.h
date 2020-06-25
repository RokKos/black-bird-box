#pragma once

#include "BlackBirdBox/Primitives/Shape.h"
#include "BlackBirdBox/Primitives/LightSource.h"
#include "BlackBirdBox/Primitives/Point.h"

namespace Core {
	class Scene
	{
	public:
		Scene() = default;

		void AddShape(const Ref<Shape>& shape) { shapes_.push_back(shape); }
		const std::vector<Ref<Shape>>& GetShapes() const { return shapes_; }

		void DeletePoints() { points_.clear(); };
		void AddPoint(const Ref<Point>& point) { points_.push_back(point); }
		const std::vector<Ref<Point>>& GetPoints() const { return points_; }

		void AddLightSource(const Ref<LightSource>& light_source) { light_sources_.push_back(light_source); }
		const std::vector<Ref<LightSource>>& GetLightSources() const { return light_sources_; }

	private:
		std::vector<Ref<Shape>> shapes_;  // TODO(Rok Kos): Think if it can be without ref
		std::vector<Ref<LightSource>> light_sources_;
		std::vector<Ref<Point>> points_;
	};
}