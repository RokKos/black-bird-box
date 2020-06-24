#pragma once

#include "BlackBirdBox/Core/Core.h"

namespace Core {
	class GraphVertex {
	public:
		GraphVertex();

		void SetConnections(const std::vector<Ref<GraphVertex>>& connections);

		void SelectColor();
		int GetColor() const { return color_; };

	private:
		int color_ = -1;
		std::vector<Ref<GraphVertex>> connections_;

	};

}