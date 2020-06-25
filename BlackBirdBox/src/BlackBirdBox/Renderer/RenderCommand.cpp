#include "bbbpch.h"
#include "RenderCommand.h"

namespace Core {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}