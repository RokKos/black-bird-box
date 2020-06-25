#pragma once

// For use by Cloth Simulation

#include "BlackBirdBox/Core/Core.h"

#include "BlackBirdBox/Core/Application.h"
#include "BlackBirdBox/Layer/Layer.h"
#include "BlackBirdBox/Core/Log.h"

#include "BlackBirdBox/Core/Timestep.h"

#include "BlackBirdBox/Core/Input.h"
#include "BlackBirdBox/KeyCodes/KeyCodes.h"
#include "BlackBirdBox/KeyCodes/MouseCodes.h"

#include "BlackBirdBox/ImGui/ImGuiLayer.h"

// ---Renderer------------------------
#include "BlackBirdBox/Renderer/Renderer.h"
#include "BlackBirdBox/Renderer/RenderCommand.h"
#include "BlackBirdBox/Renderer/Texture/Texture2D.h"
#include "BlackBirdBox/Renderer/CubeMap/CubeMap.h"
#include "BlackBirdBox/Renderer/Shader.h"
#include "BlackBirdBox/Renderer/Buffers/FrameBuffer.h"
#include "BlackBirdBox/Renderer/Buffers/ShaderStorageArray.h"
// -----------------------------------


#include "BlackBirdBox/Components/Material.h"
#include "BlackBirdBox/Primitives/PhongLightingParamaters.h"
#include "BlackBirdBox/Tools/ModelLoader.h"


#include "BlackBirdBox/Events/KeyEvent.h"
#include "BlackBirdBox/Events/MouseEvent.h"

#include "BlackBirdBox/Controllers/Renderer/Camera/PerspectiveCameraController.h"
#include "BlackBirdBox/Primitives/ComputeShaderConfiguration.h"
#include "BlackBirdBox/Primitives/ComputeShaderSimulationConfiguration.h"
#include "BlackBirdBox/Scene/Scene.h"
#include "BlackBirdBox/ImGui/Menus/BaseMenu.h"
#include "BlackBirdBox/ImGui/Menus/CameraMenu.h"
#include "BlackBirdBox/ImGui/Menus/SceneViewMenu.h"
#include "BlackBirdBox/ImGui/Menus/ComputeShaderMenu.h"
#include "BlackBirdBox/ImGui/Menus/MiscMenu.h"

#include "BlackBirdBox/Core/Util.h"
#include "BlackBirdBox/JSON/JsonUtil.h"

#include "BlackBirdBox/Tools/Instrumentator.h"
#include "BlackBirdBox/Tools/InstrumentationTimer.h"

// Note(Rok Kos): Macros needs to be behind ImGui, because it includes Windows.h, look at this issue for further explanation: https://github.com/Tencent/rapidjson/issues/1448
#include "BlackBirdBox/Core/Macros.h"

#include "BlackBirdBox/Physics/Cloth/Cloth.h"