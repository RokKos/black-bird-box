#pragma once

#include "Camera.h"

namespace Core {
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera();
		PerspectiveCamera(float aspect_ratio, float field_of_view, float near_clipping_plane, float far_clipping_plane);


		float GetAspectRatio() const { return aspect_ratio_; }
		void SetAspectRatio(float aspect_ratio) { aspect_ratio_ = aspect_ratio; RecalculateProjectionMatrix(); }

		float GetFieldOfView() const { return field_of_view_; }
		void SetFieldOfView(float field_of_view) { field_of_view_ = field_of_view; RecalculateProjectionMatrix(); }

		float GetNearClippingPlane() const { return near_clipping_plane_; }
		void SetNearClippingPlane(float near_clipping_plane) { near_clipping_plane_ = near_clipping_plane; RecalculateProjectionMatrix(); }

		float GetFarClippingPlane() const { return far_clipping_plane_; }
		void SetFarClippingPlane(float far_clipping_plane) { far_clipping_plane_ = far_clipping_plane; RecalculateProjectionMatrix(); }

	private:
		void RecalculateProjectionMatrix();

	private:
		float aspect_ratio_ = 1.0f;
		float field_of_view_ = 45.0f;
		float near_clipping_plane_ = 0.01f;
		float far_clipping_plane_ = 1000.f;
	};

}