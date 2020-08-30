#include "EOLLayer.h"

#include <imgui.h>
#include <cmath>
#include <thread>
#include <functional>
#include <glm/gtc/type_ptr.hpp>

namespace EOL {
	EOLLayer::EOLLayer()
		: Core::Layer("EOLLayer")
	{
		PROFILE_FUNCTION();

		LoadAllShaders();
		LoadAllPrimitiveModels();


		perspective_camera_controller_ = Core::CreateRef<Core::PerspectiveCameraController>();
		perspective_camera_controller_->GetCamera().SetPosition(glm::vec3(0.0f, 0.5f, 1.5f));
		menus_.push_back(Core::CreateRef<Core::CameraMenu>("Camera Controls", perspective_camera_controller_));
		menus_.push_back(Core::CreateRef<Core::SceneViewMenu>("Scene View", scene_.GetShapes(), scene_.GetLightSources()));
		menus_.push_back(Core::CreateRef<Core::ComputeShaderMenu>("Compute Shader Simulation Controls", compute_shader_simulation_configuration_));
		menus_.push_back(Core::CreateRef<Core::MiscMenu>("Misc Menu", prev_time_step_, bg_color_, polygon_mode_));
		

		auto mat_generic_triangle = Core::CreateRef<Core::Material>(shader_library_.Get("TriangleTest"), Core::PhongLightingParameters(), "Generic_Triangle_MAT");
		auto mat_generic_color = Core::CreateRef<Core::Material>(shader_library_.Get("GenericColor"), Core::PhongLightingParameters(), "Generic_Color_MAT");

		auto mat_generic_normals = Core::CreateRef<Core::Material>(shader_library_.Get("GenericNormals"), Core::PhongLightingParameters(), "Generic_Normals_MAT");

		auto generic_uv_coordinates_shader = shader_library_.Get("GenericUVCoordinates");
		generic_uv_coordinates_shader->Bind();
		generic_uv_coordinates_shader->SetInt("u_Texture", 0);
		
		Core::Texture2DSpecification generic_tex_2d_spec = Core::Texture2DSpecification();
		generic_tex_2d_spec.TextureMinFilter = Core::TextureMinifyingFilter::LINEAR;
		generic_tex_2d_spec.TextureMagFilter = Core::TextureMagnificationFilter::NEAREST;

		auto uv_texture = Core::Texture2D::Create("assets/Textures/uv_texture.png", generic_tex_2d_spec);
		auto mat_generic_uv_coordinates = Core::CreateRef<Core::Material>(generic_uv_coordinates_shader, Core::PhongLightingParameters(), "Generic_UV_Coordinates_MAT");
		mat_generic_uv_coordinates->SetTexture("UV_TEST_Texture", uv_texture);

		auto generic_texture_shader = shader_library_.Get("GenericTexture");
		generic_texture_shader->Bind();
		generic_texture_shader->SetInt("u_Texture", 0);
		auto gun_texture = Core::Texture2D::Create("assets/Textures/Cerberus_A.tga", generic_tex_2d_spec);
		auto mat_generic_texture = Core::CreateRef<Core::Material>(generic_texture_shader, Core::PhongLightingParameters(), "Generic_Texture_MAT");
		mat_generic_texture->SetTexture("Gun_Texture", gun_texture);

		auto phong_lighting_parameters = Core::PhongLightingParameters();
		phong_lighting_parameters.diffuse_color_ = glm::vec3(188.0f / 255.0f, 185.0f / 255.0f, 167.0f / 255.0f);
		phong_lighting_parameters.specular_color_ = glm::vec3(1.0f, 1.0f, 1.0f);
		phong_lighting_parameters.specular_scattering_ = 15.0f;
		phong_lighting_parameters.ambient_color_ = glm::vec3(81.0f / 255.0f, 84.0f / 255.0f, 67.0f / 255.0f);
		phong_lighting_parameters.ambient_intensity_ = glm::vec3(0.21f, 0.21f, 0.21f);
		auto mat_generic_lighting = Core::CreateRef<Core::Material>(shader_library_.Get("GenericLighting"), phong_lighting_parameters, "Generic_Lighting_MAT");

		auto standard_shader = shader_library_.Get("FrameBufferShader");
		standard_shader->Bind();
		standard_shader->SetInt("u_Texture", 0);
		auto mat_stadard_shader = Core::CreateRef<Core::Material>(standard_shader, phong_lighting_parameters, "Standard_MAT");
		mat_stadard_shader->SetTexture("Gun_Texture", gun_texture);

		// Gun ------
		{
			auto vertex_array_gun = Core::VertexArray::Create();
			auto model_data = model_library_.Get("assets/Models/gun.obj");

			auto vertex_buffer_gun = Core::VertexBuffer::Create(model_data.vertices.data(), model_data.vertices.size() * sizeof(Core::Vertex));
			Core::BufferLayout layout_gun = {
			{ Core::ShaderDataType::Float3, "a_Position" },
			{ Core::ShaderDataType::Float3, "a_Normal" },
			{ Core::ShaderDataType::Float2, "a_TexCoord" },
			};

			vertex_buffer_gun->SetLayout(layout_gun);
			vertex_array_gun->AddVertexBuffer(vertex_buffer_gun);

			Core::Ref<Core::IndexBuffer> index_buffer_gun = Core::IndexBuffer::Create(model_data.indices.data(), model_data.indices.size());
			vertex_array_gun->SetIndexBuffer(index_buffer_gun);

			// TODO(Rok Kos): Read from JSON file
			auto shape = Core::CreateRef<Core::Shape>(mat_generic_color, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(0, 0.5, 0)), model_data, "Obj Model Test");
			auto shape2 = Core::CreateRef<Core::Shape>(mat_generic_normals, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(1, 0, 0)), model_data, "Obj Model Normals Test");
			auto shape3 = Core::CreateRef<Core::Shape>(mat_generic_uv_coordinates, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(4, 0, 0)), model_data, "Obj Texture UVs Test");
			auto shape4 = Core::CreateRef<Core::Shape>(mat_generic_texture, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(6, 0, 0)), model_data, "Obj Texture Test");
			auto shape5 = Core::CreateRef<Core::Shape>(mat_generic_lighting, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(8, 0, 0)), model_data, "Obj Lighting Test");
			auto shape6 = Core::CreateRef<Core::Shape>(mat_stadard_shader, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(0, 0.5, 0)), model_data, "Obj All Together");
			//scene_.AddShape(shape);
			//scene_.AddShape(shape2);
			//scene_.AddShape(shape3);
			//scene_.AddShape(shape4);
			//scene_.AddShape(shape5);
			//scene_.AddShape(shape6);
		}

		// City01 ------
		{
			auto vertex_array_ship = Core::VertexArray::Create();
			auto model_data_ship = model_library_.Get("assets/Models/LowPolyCity.obj");

			auto vertex_buffer_ship = Core::VertexBuffer::Create(model_data_ship.vertices.data(), model_data_ship.vertices.size() * sizeof(Core::Vertex));
			Core::BufferLayout layout_ship = {
			{ Core::ShaderDataType::Float3, "a_Position" },
			{ Core::ShaderDataType::Float3, "a_Normal" },
			{ Core::ShaderDataType::Float2, "a_TexCoord" },
			};

			vertex_buffer_ship->SetLayout(layout_ship);
			vertex_array_ship->AddVertexBuffer(vertex_buffer_ship);

			Core::Ref<Core::IndexBuffer> index_buffer_ship = Core::IndexBuffer::Create(model_data_ship.indices.data(), model_data_ship.indices.size());
			vertex_array_ship->SetIndexBuffer(index_buffer_ship);

			auto city_texture = Core::Texture2D::Create("assets/Textures/Palette.jpg", generic_tex_2d_spec);
			auto mat_stadard_shader_city = Core::CreateRef<Core::Material>(standard_shader, phong_lighting_parameters, "Standard_MAT");
			mat_stadard_shader_city->SetTexture("City_Texture", city_texture);

			auto ship = Core::CreateRef<Core::Shape>(mat_stadard_shader_city, vertex_array_ship, Core::CreateRef<Core::Transform>(glm::vec3(-1.5, 0.5, 0), glm::vec3(0.0), glm::vec3(0.1, 0.1, 0.1)), model_data_ship, "City 01");
			scene_.AddShape(ship);
		}


		auto blinn_phong_shader = shader_library_.Get("BlinnPhongShader");
        blinn_phong_shader->Bind();
        blinn_phong_shader->SetInt("u_Texture", 0);
		// City02 ------
        {
            auto vertex_array_ship = Core::VertexArray::Create();
            auto model_data_ship = model_library_.Get("assets/Models/LowPolyCity.obj");

            auto vertex_buffer_ship
                = Core::VertexBuffer::Create(model_data_ship.vertices.data(), model_data_ship.vertices.size() * sizeof(Core::Vertex));
            Core::BufferLayout layout_ship = {
                { Core::ShaderDataType::Float3, "a_Position" },
                { Core::ShaderDataType::Float3, "a_Normal" },
                { Core::ShaderDataType::Float2, "a_TexCoord" },
            };

            vertex_buffer_ship->SetLayout(layout_ship);
            vertex_array_ship->AddVertexBuffer(vertex_buffer_ship);

            Core::Ref<Core::IndexBuffer> index_buffer_ship
                = Core::IndexBuffer::Create(model_data_ship.indices.data(), model_data_ship.indices.size());
            vertex_array_ship->SetIndexBuffer(index_buffer_ship);

            auto city_texture = Core::Texture2D::Create("assets/Textures/Palette.jpg", generic_tex_2d_spec);
            auto mat_stadard_shader_city = Core::CreateRef<Core::Material>(blinn_phong_shader, phong_lighting_parameters, "Standard_MAT");
            mat_stadard_shader_city->SetTexture("City_Texture", city_texture);

            auto ship = Core::CreateRef<Core::Shape>(mat_stadard_shader_city, vertex_array_ship,
                Core::CreateRef<Core::Transform>(glm::vec3(1.5, 0.5, 0), glm::vec3(0.0), glm::vec3(0.1, 0.1, 0.1)), model_data_ship, "City 02");
            scene_.AddShape(ship);
        }

		scene_.AddLightSource(Core::CreateRef<Core::LightSource>(Core::LightType::kDirectional,
            Core::CreateRef<Core::Transform>(glm::vec3(0, 5, 0), glm::vec3(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)), glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.f, 1.f, 1.f)));

		scene_.AddPoint(Core::CreateRef<Core::Point>(10, glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)));
		scene_.AddPoint(Core::CreateRef<Core::Point>(100, glm::vec3(10, 2, 5), glm::vec3(0, 1, 0)));


		// Enviroment map ------
		auto enviroment_map_shader = shader_library_.Get("EnviromentMapShader");
		enviroment_map_shader->Bind();
		enviroment_map_shader->SetInt("u_EnviromentMap", 0);

		vertex_array_box_ = Core::VertexArray::Create();

		float enviroment_map_box_vertices[] = {
			// Front face
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,
			1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,

			// Back face
			-1.0, -1.0, -1.0,
			-1.0,  1.0, -1.0,
			1.0,  1.0, -1.0,
			1.0, -1.0, -1.0,

			// Top face
			-1.0,  1.0, -1.0,
			-1.0,  1.0,  1.0,
			1.0,  1.0,  1.0,
			1.0,  1.0, -1.0,

			// Bottom face
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0, -1.0,  1.0,
			-1.0, -1.0,  1.0,

			// Right face
			1.0, -1.0, -1.0,
			1.0,  1.0, -1.0,
			1.0,  1.0,  1.0,
			1.0, -1.0,  1.0,

			// Left face
			-1.0, -1.0, -1.0,
			-1.0, -1.0,  1.0,
			-1.0,  1.0,  1.0,
			-1.0,  1.0, -1.0,
		};

		uint32_t enviroment_map_box_indices[] = {
			0, 1, 2, 0, 2, 3,    // front
			4, 5, 6, 4, 6, 7,    // back
			8, 9, 10, 8, 10, 11,   // top
			12, 13, 14, 12, 14, 15,   // bottom
			16, 17, 18, 16, 18, 19,   // right
			20, 21, 22, 20, 22, 23,   // left
		};

		auto vertex_buffer_box = Core::VertexBuffer::Create(enviroment_map_box_vertices, sizeof(enviroment_map_box_vertices));
		Core::BufferLayout layout_box = {
		{ Core::ShaderDataType::Float3, "a_Position" },
		};

		vertex_buffer_box->SetLayout(layout_box);
		vertex_array_box_->AddVertexBuffer(vertex_buffer_box);

		Core::Ref<Core::IndexBuffer> index_buffer_box = Core::IndexBuffer::Create(enviroment_map_box_indices, sizeof(enviroment_map_box_indices));
		vertex_array_box_->SetIndexBuffer(index_buffer_box);

		enviroment_map_ = Core::CubeMap::Create("assets/Textures/CubeMap/");
		
		// Compute Cloth
		
		ParseSimulationSettings();

		cloth_ = Core::CreateRef<Core::Cloth>(num_cloth_dimension_size_, mat_generic_triangle);
		scene_.AddShape(cloth_);

		Core::FramebufferSpecification frame_buffer_spec = Core::FramebufferSpecification();
		frame_buffer_spec.width = 1920;
		frame_buffer_spec.height = 1080;
		frame_buffer_spec.frame_buffer_attachments.push_back(Core::FrameBufferAttachments::COLOR_ATTACHMENT1);
		frame_buffer_spec.frame_buffer_attachments.push_back(Core::FrameBufferAttachments::COLOR_ATTACHMENT2);
		frame_buffer_spec.frame_buffer_attachments.push_back(Core::FrameBufferAttachments::COLOR_ATTACHMENT3);
		frame_buffer_spec.frame_buffer_attachments.push_back(Core::FrameBufferAttachments::COLOR_ATTACHMENT4);
		frame_buffer_spec.frame_buffer_attachments.push_back(Core::FrameBufferAttachments::COLOR_ATTACHMENT5);
		frame_buffer_spec.frame_buffer_attachments.push_back(Core::FrameBufferAttachments::COLOR_ATTACHMENT6);
		frame_buffer_spec.frame_buffer_attachments.push_back(Core::FrameBufferAttachments::COLOR_ATTACHMENT7);
		test_frame_buffer_ = Core::FrameBuffer::Create(frame_buffer_spec);
	}

	void EOLLayer::OnAttach()
	{
		Core::Layer::OnAttach();
	}

	void EOLLayer::OnDetach()
	{
		Core::Layer::OnDetach();

	}

	void EOLLayer::OnUpdate(Core::TimeStep ts)
	{
		Core::Layer::OnUpdate(ts);
		perspective_camera_controller_->OnUpdate(ts);

		Core::Renderer::BeginScene(perspective_camera_controller_->GetCamera(), scene_.GetLightSources());

		prev_time_step_ = ts;

		Core::RenderCommand::SetClearColor(bg_color_);
		Core::RenderCommand::Clear();

		Core::Renderer::Submit(shader_library_.Get("EnviromentMapShader"), enviroment_map_, vertex_array_box_);

		auto compute_particles_shader = shader_library_.Get("ComputeCloth");
		
		for (size_t batch_id = 0; batch_id < cloth_->GetNumberOfBatchers(); ++batch_id) {
			Core::Renderer::DispatchComputeShader(compute_particles_shader, cloth_->GetClothStorageArray(batch_id), compute_shader_configuration_, compute_shader_simulation_configuration_);
		}
		

		// TODO(Rok Kos): Load Models on themand

		Core::RenderCommand::SetPolygonMode((Core::RendererAPI::PolygonMode)polygon_mode_);

		test_frame_buffer_->Bind();

		for (const auto& shape : scene_.GetShapes())
		{
			if (shape->GetObjectEnabled()) {
				Core::Renderer::Submit(shape->GetMaterial(), shape->GetVertexArray(), shape->GetTransform()->GetTransformMatrix());
			}
		}
		test_frame_buffer_->Unbind();
		
		for (const auto& shape : scene_.GetShapes())
		{
			if (shape->GetObjectEnabled()) {
				Core::Renderer::Submit(shape->GetMaterial(), shape->GetVertexArray(), shape->GetTransform()->GetTransformMatrix());
			}
		}

		for (const auto& light_source : scene_.GetLightSources()) {
			if (light_source->GetObjectEnabled()) {
                        Core::Renderer::Submit(Core::CreateRef<Core::Material>(
                            shader_library_.Get("GenericColor"), Core::PhongLightingParameters(), "Generic_Color_MAT"), vertex_array_box_, light_source->GetTransform()->GetTransformMatrix());
            }
        }

		Core::RenderCommand::SetPolygonMode(Core::RendererAPI::PolygonMode::FILL);

		Core::Renderer::EndScene();
	}

	void EOLLayer::OnImGuiRender()
	{
		Core::Layer::OnImGuiRender();

		for (const auto& menu : menus_) {
			menu->OnImGuiRender();
		}

		ImGui::Begin("ViewPort");
		int i = 0;
		for (Core::FrameBufferAttachments attachment : test_frame_buffer_->GetFrameBufferSpecification().frame_buffer_attachments) {
			// TODO(Rok Kos): Investigate why depth buffer is not rendering
			if (attachment == Core::FrameBufferAttachments::DEPTH_STENCIL_ATTACHMENT) {
				continue;
			}
			if (i != 0) {
				ImGui::SetCursorPos(ImVec2(360 * (i % 4) + 1, 360 * (i / 4) + 1));
			}
			
			ImGui::Text(FrameBufferAttachmentToName(attachment).c_str());
			ImGui::SetCursorPos(ImVec2(360 * (i % 4) + 1, 360 * (i / 4) + 2));
			uint32_t color_textureID = test_frame_buffer_->GetTextureAttachment(attachment)->GetRenderID();
			ImGui::Image((void*)color_textureID, ImVec2{ 360, 360 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			if (i != 0) {
				i++;
			}
			else {
				i += 4;
			}
			
		}
		ImGui::End();

	}

	void EOLLayer::OnEvent(Core::Event& e)
	{
		Core::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Core::KeyPressedEvent>(BIND_EVENT_FN(EOLLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<Core::KeyTypedEvent>(BIND_EVENT_FN(EOLLayer::OnKeyTypedEvent));

		perspective_camera_controller_->OnEvent(e);
		Core::Layer::OnEvent(e);
	}

	bool EOLLayer::OnKeyPressedEvent(Core::KeyPressedEvent& e)
	{
		LOG_INFO("EOL LAYER::OnKeyPressedEvent key pressed: {0}", e.GetKeyCode());
		return false;
	}

	bool EOLLayer::OnKeyTypedEvent(Core::KeyTypedEvent& e)
	{
		LOG_INFO("EOLLayer::OnKeyTypedEvent key pressed: {0}", e.GetKeyCode());

		return false;
	}

	void EOLLayer::ParseSimulationSettings()
	{
		auto simulation_config = Core::JsonUtil::ReadJson("assets/Configs/SimulationConfig.json");
		ASSERT(simulation_config["ClothSimulationSettings"].IsObject(), "ClothSimulationSettings is not an object!");
		auto cloth_simulation_settings = simulation_config["ClothSimulationSettings"].GetObject();

		ASSERT(cloth_simulation_settings["ClothDimensionSize"].IsInt(), "ClothDimensionSize is not an int!");
		num_cloth_dimension_size_ = cloth_simulation_settings["ClothDimensionSize"].GetInt();

		ASSERT(cloth_simulation_settings["Gravity"].IsArray(), "Gravity is not an array!");
		auto& gravity_json = cloth_simulation_settings["Gravity"];
		ASSERT(gravity_json.Size() == 3, "Gravity is not size 3!");

		glm::vec3 gravity;
		for (rapidjson::SizeType i = 0; i < gravity_json.Size(); i++) {
			gravity[i] = gravity_json[i].GetFloat();
		}

		ASSERT(cloth_simulation_settings["SimulationDeltaTime"].IsFloat(), "SimulationDeltaTime is not an int!");
		float simulation_delta_time = cloth_simulation_settings["SimulationDeltaTime"].GetFloat();

		ASSERT(cloth_simulation_settings["ConstraintItteratitions"].IsInt(), "ConstraintItteratitions is not an int!");
		int constraint_itterations = cloth_simulation_settings["ConstraintItteratitions"].GetInt();

		compute_shader_simulation_configuration_ = Core::ComputeShaderSimulationConfiguration(gravity, simulation_delta_time, constraint_itterations);

		ASSERT(cloth_simulation_settings["ComputeShaderConfiguration"].IsObject(), "ComputeShaderConfiguration is not an object!");
		auto compute_shader_configuration = cloth_simulation_settings["ComputeShaderConfiguration"].GetObject();
		ASSERT(compute_shader_configuration["WorkGroupSize"].IsArray(), "WorkGroupSize is not an array!");
		auto& work_group_size_json = compute_shader_configuration["WorkGroupSize"];
		ASSERT(work_group_size_json.Size() == 3, "WorkGroupSize is not size 3!");

		std::array<unsigned int, 3> work_group_config;
		for (rapidjson::SizeType i = 0; i < work_group_size_json.Size(); i++) {
			work_group_config[i] = work_group_size_json[i].GetInt();
		}
		

		ASSERT(compute_shader_configuration["LocalGroupSize"].IsArray(), "LocalGroupSize is not an array!");
		auto& local_group_size_json = compute_shader_configuration["LocalGroupSize"];
		ASSERT(local_group_size_json.Size() == 3, "LocalGroupSize is not size 3!");

		std::array<unsigned int, 3> local_group_config;
		for (rapidjson::SizeType i = 0; i < local_group_size_json.Size(); i++) {
			local_group_config[i] = local_group_size_json[i].GetInt();
		}
		
		compute_shader_configuration_ = Core::ComputeShaderConfiguration(work_group_config, local_group_config);
	}

	void EOLLayer::LoadAllShaders()
	{
		PROFILE_FUNCTION();

		auto load_shader = Core::JsonUtil::ReadJson("assets/Configs/LoadShader.json");
		ASSERT(load_shader["Shaders"].IsArray(), "Shaders is not an array!");

		for (auto& shader_path : load_shader["Shaders"].GetArray()) {
			ASSERT(shader_path.IsString(), "Shader Path is not string");
			shader_library_.Load(shader_path.GetString());
		}
	}

	void EOLLayer::LoadAllPrimitiveModels()
	{
		PROFILE_FUNCTION();
		auto load_model = Core::JsonUtil::ReadJson("assets/Configs/LoadModels.json");
		ASSERT(load_model["Models"].IsArray(), "Models is not an array!");

		std::vector<std::thread> threads;

		for (auto& model_path : load_model["Models"].GetArray()) {
			ASSERT(model_path.IsString(), "Models Path is not string");
			threads.push_back(std::thread(&EOLLayer::LoadModelInThread, this, model_path.GetString()));
		}

		for (auto& thread : threads) {
			thread.join();
		}
	}

	void EOLLayer::LoadModelInThread(const std::string& filepath)
	{
		PROFILE_FUNCTION();

		model_library_.Load(filepath);
	}

	std::string EOLLayer::FrameBufferAttachmentToName(Core::FrameBufferAttachments attachment)
	{
		switch (attachment)
		{
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT0: return "Main Color Output";
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT1: return "Normals Output";
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT2: return "UV Output";
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT3: return "vertex Positions Output";
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT4: return "Model Positions Output";
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT5: return "Light Diffuse Color";
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT6: return "Light Specular Color";
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT7: return "Light All Color";
		case Core::FrameBufferAttachments::DEPTH_STENCIL_ATTACHMENT: return "Depth/Stencil Buffer";
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT8:
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT9:
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT10:
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT11:
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT12:
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT13:
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT14:
		case Core::FrameBufferAttachments::COLOR_ATTACHMENT15:
		case Core::FrameBufferAttachments::DEPTH_ATTACHMENT:
		case Core::FrameBufferAttachments::STENCIL_ATTACHMENT:
		default:
			break;
		}

		return "";
	}

}