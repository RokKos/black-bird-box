#include "bbbpch.h"
#include "Layer.h"

namespace Core {

	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
		PROFILE_FUNCTION();
	}

	void Layer::OnAttach()
	{
		PROFILE_FUNCTION();

		LOG_INFO("OnAttach::Attaching layer: {0}", m_DebugName);
	}

	void Layer::OnDetach()
	{
		PROFILE_FUNCTION();

		LOG_INFO("OnDetach::Deattaching layer: {0}", m_DebugName);
	}

}