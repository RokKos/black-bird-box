#include "bbbpch.h"
#include "RenderCommand.h"

namespace BlackBirdBox {

Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}
