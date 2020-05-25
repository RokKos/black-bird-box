#include "bbbpch.h"
#include "Layer.h"

namespace Core {

	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}

	void Layer::OnAttach()
	{
		LOG_INFO("OnAttach::Attaching layer: {0}", m_DebugName);
	}

	void Layer::OnDetach()
	{
		LOG_INFO("OnDetach::Deattaching layer: {0}", m_DebugName);
	}

}