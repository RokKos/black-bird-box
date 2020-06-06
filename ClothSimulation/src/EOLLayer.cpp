#include "EOLLayer.h"

#include <imgui.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>


namespace EOL {
	EOLLayer::EOLLayer()
		: Core::Layer("EOLLayer"), perspective_camera_controller_()
	{

		auto triangle_test_shader = shader_library_.Load("assets/Shaders/TriangleTest.glsl");
		auto mat_generic_triangle = Core::CreateRef<Core::Material>(triangle_test_shader, Core::PhongLightingParameters(), "Generic_Triangle_MAT");
		auto texture_shader = shader_library_.Load("assets/Shaders/Texture.glsl");


		auto generic_color_shader = shader_library_.Load("assets/Shaders/GenericColor.glsl");
		auto mat_generic_color = Core::CreateRef<Core::Material>(generic_color_shader, Core::PhongLightingParameters(), "Generic_Color_MAT");

		auto generic_normals_shader = shader_library_.Load("assets/Shaders/GenericNormals.glsl");
		auto mat_generic_normals = Core::CreateRef<Core::Material>(generic_normals_shader, Core::PhongLightingParameters(), "Generic_Normals_MAT");

		auto generic_uv_coordinates_shader = shader_library_.Load("assets/Shaders/GenericUVCoordinates.glsl");
		generic_uv_coordinates_shader->Bind();
		generic_uv_coordinates_shader->SetInt("u_Texture", 0);
		auto uv_texture = Core::Texture2D::Create("assets/Textures/uv_texture.png");
		auto mat_generic_uv_coordinates = Core::CreateRef<Core::Material>(generic_uv_coordinates_shader, Core::PhongLightingParameters(), "Generic_UV_Coordinates_MAT");
		mat_generic_uv_coordinates->SetTexture("UV_TEST_Texture", uv_texture);

		auto generic_texture_shader = shader_library_.Load("assets/Shaders/GenericTexture.glsl");
		generic_texture_shader->Bind();
		generic_texture_shader->SetInt("u_Texture", 0);
		auto gun_texture = Core::Texture2D::Create("assets/Textures/Cerberus_A.tga");
		auto mat_generic_texture = Core::CreateRef<Core::Material>(generic_texture_shader, Core::PhongLightingParameters(), "Generic_Texture_MAT");
		mat_generic_texture->SetTexture("Gun_Texture", gun_texture);

		auto generic_lighting_shader = shader_library_.Load("assets/Shaders/GenericLighting.glsl");
		auto phong_lighting_parameters = Core::PhongLightingParameters();
		phong_lighting_parameters.diffuse_color_ = glm::vec3(0.8f, 0.0f, 0.0f);
		phong_lighting_parameters.specular_color_ = glm::vec3(0.0f, 0.3f, 0.0f);
		phong_lighting_parameters.specular_scattering_ = 32.0f;
		phong_lighting_parameters.ambient_color_ = glm::vec3(0.0f, 0.0f, 1.0f);
		phong_lighting_parameters.ambient_intensity_ = glm::vec3(0.5f, 0.5f, 0.5f);
		auto mat_generic_lighting = Core::CreateRef<Core::Material>(generic_lighting_shader, phong_lighting_parameters, "Generic_Lighting_MAT");

		auto standard_shader = shader_library_.Load("assets/Shaders/StandardShader.glsl");
		standard_shader->Bind();
		standard_shader->SetInt("u_Texture", 0);
		auto mat_stadard_shader = Core::CreateRef<Core::Material>(standard_shader, phong_lighting_parameters, "Standard_MAT");
		mat_stadard_shader->SetTexture("Gun_Texture", gun_texture);

		// Gun ------
		auto vertex_array_gun = Core::VertexArray::Create();
		auto model_data = Core::ModelLoader::LoadModel("assets/Models/gun.obj");

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
		auto shape = Core::CreateRef<Core::Shape>(mat_generic_color, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(0, 0, 0)), model_data, "Obj Model Test");
		auto shape2 = Core::CreateRef<Core::Shape>(mat_generic_normals, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(2, 0, 0)), model_data, "Obj Model Normals Test");
		auto shape3 = Core::CreateRef<Core::Shape>(mat_generic_uv_coordinates, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(4, 0, 0)), model_data, "Obj Texture UVs Test");
		auto shape4 = Core::CreateRef<Core::Shape>(mat_generic_texture, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(6, 0, 0)), model_data, "Obj Texture Test");
		auto shape5 = Core::CreateRef<Core::Shape>(mat_generic_lighting, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(8, 0, 0)), model_data, "Obj Lighting Test");
		auto shape6 = Core::CreateRef<Core::Shape>(mat_stadard_shader, vertex_array_gun, Core::CreateRef<Core::Transform>(glm::vec3(10, 0, 0)), model_data, "Obj All Together");
		scene_.AddShape(shape);
		scene_.AddShape(shape2);
		scene_.AddShape(shape3);
		scene_.AddShape(shape4);
		scene_.AddShape(shape5);
		scene_.AddShape(shape6);

		scene_.AddLightSource(Core::CreateRef<Core::LightSource>(Core::LightType::kDirectional, Core::CreateRef<Core::Transform>(glm::vec3(0, 0, 10)), glm::vec3(0.5f, 0.0f, 0.7f)));

		scene_.AddPoint(Core::CreateRef<Core::Point>(10, glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)));
		scene_.AddPoint(Core::CreateRef<Core::Point>(100, glm::vec3(10, 2, 5), glm::vec3(0, 1, 0)));


		// Enviroment map ------
		enviroment_map_shader_ = shader_library_.Load("assets/Shaders/EnviromentMapShader.glsl");
		enviroment_map_shader_->Bind();
		enviroment_map_shader_->SetInt("u_EnviromentMap", 0);

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
		
		auto compute_cloth_shader = shader_library_.Load("assets/Shaders/ComputeCloth.glsl");
		num_cloth_dimension_size_ = 5;
		num_cloth_particles_ = num_cloth_dimension_size_ * num_cloth_dimension_size_;
		compute_shader_configuration_ = Core::ComputeShaderConfiguration({ num_cloth_particles_, 1, 1 }, { 1, 1, 1 });

		std::vector<glm::vec3> prev_cloth_particle_positons;
		prev_cloth_particle_positons.reserve(num_cloth_particles_);
		std::vector<glm::vec3> cloth_particle_positons;
		cloth_particle_positons.reserve(num_cloth_particles_);
		
		std::vector<glm::mat3> cloth_particle_constraints;
		cloth_particle_constraints.reserve(num_cloth_particles_);
		std::vector<glm::int32> cloth_particle_fixed_pos;
		cloth_particle_fixed_pos.reserve(num_cloth_particles_);
		for (unsigned int i = 0; i < num_cloth_particles_; ++i) {
			prev_cloth_particle_positons.push_back(glm::vec3(i % num_cloth_dimension_size_, i / num_cloth_dimension_size_, 0));
			cloth_particle_positons.push_back(glm::vec3( (float)(i % num_cloth_dimension_size_) / (float) num_cloth_dimension_size_, (float)(i / num_cloth_dimension_size_) / (float)num_cloth_dimension_size_, 0));
			cloth_particle_constraints.push_back(glm::mat3(1.0));
			cloth_particle_fixed_pos.push_back(0);
		}

		auto prev_positions_buffer = Core::ShaderStorageBuffer::Create(prev_cloth_particle_positons, prev_cloth_particle_positons.size() * sizeof(glm::vec3));
		auto positions_buffer = Core::ShaderStorageBuffer::Create(cloth_particle_positons, cloth_particle_positons.size() * sizeof(glm::vec3));
		auto constrains_buffer = Core::ShaderStorageBuffer::Create(cloth_particle_constraints, cloth_particle_constraints.size() * sizeof(glm::mat3));
		auto fixed_pos_buffer = Core::ShaderStorageBuffer::Create(cloth_particle_fixed_pos, cloth_particle_fixed_pos.size() * sizeof(glm::int32));
		cloth_storage_array_ = Core::ShaderStorageArray::Create();
		cloth_storage_array_->AddShaderStorageBuffer(prev_positions_buffer);
		cloth_storage_array_->AddShaderStorageBuffer(positions_buffer);
		cloth_storage_array_->AddShaderStorageBuffer(constrains_buffer);
		cloth_storage_array_->AddShaderStorageBuffer(fixed_pos_buffer);
		

		auto vertex_array_cloth = Core::VertexArray::Create();
		auto vertex_buffer_cloth = Core::VertexBuffer::CreateExistingBuffer(positions_buffer->GetRendererID());
		Core::BufferLayout layout_cloth = {
		{ Core::ShaderDataType::Float3, "a_Position" },
		};

		vertex_buffer_cloth->SetLayout(layout_cloth);
		vertex_array_cloth->AddVertexBuffer(vertex_buffer_cloth);

		std::vector<uint32_t> cloth_indices;
		cloth_indices.reserve(num_cloth_particles_);
		for (unsigned int x = 0; x < num_cloth_dimension_size_; ++x) {
			for (unsigned int y = 0; y < num_cloth_dimension_size_; ++y) {
				// Left Triangle
				cloth_indices.push_back(x + y * num_cloth_dimension_size_);
				cloth_indices.push_back((x + 1) + y * num_cloth_dimension_size_);
				cloth_indices.push_back(x + (y + 1) * num_cloth_dimension_size_);
				
				// Right Triangle
				cloth_indices.push_back((x + 1) + y * num_cloth_dimension_size_);
				cloth_indices.push_back((x + 1) + (y + 1) * num_cloth_dimension_size_);
				cloth_indices.push_back(x + (y + 1) * num_cloth_dimension_size_);
			}
		}

		Core::Ref<Core::IndexBuffer> index_buffer_cloth = Core::IndexBuffer::Create(cloth_indices.data(), cloth_indices.size());
		vertex_array_cloth->SetIndexBuffer(index_buffer_cloth);

		auto cloth = Core::CreateRef<Core::Shape>(mat_generic_triangle, vertex_array_cloth, Core::CreateRef<Core::Transform>(glm::vec3(0, 0, 0)), Core::ModelData(), "Cloth");
		scene_.AddShape(cloth);

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
		perspective_camera_controller_.OnUpdate(ts);

		Core::Renderer::BeginScene(perspective_camera_controller_.GetCamera(), scene_.GetLightSources());

		prev_time_step_ = ts;

		Core::RenderCommand::SetClearColor(bg_color_);
		Core::RenderCommand::Clear();

		Core::Renderer::Submit(enviroment_map_shader_, enviroment_map_, vertex_array_box_);

		auto compute_particles_shader = shader_library_.Get("ComputeCloth");
		Core::Renderer::DispatchComputeShader(compute_particles_shader, cloth_storage_array_, compute_shader_configuration_);

		// TODO(Rok Kos): Load Models on themand

		for (auto shape : scene_.GetShapes())
		{
			Core::Renderer::Submit(shape->GetMaterial(), shape->GetVertexArray(), shape->GetTransform()->GetTransformMatrix());
		}
		
		Core::Renderer::EndScene();
	}

	void EOLLayer::OnImGuiRender()
	{
		Core::Layer::OnImGuiRender();

		ImGui::Begin("Debug Controlls");
		if (ImGui::TreeNode("Misc")) {
			ImGui::ColorEdit4("BG Color", glm::value_ptr(bg_color_));
			ImGui::Text("Delta time: %f", prev_time_step_.GetSeconds());

			ImGui::TreePop();
		}

	
		if (ImGui::TreeNode("Camera Controls")) {

			float camera_movement_speed = perspective_camera_controller_.GetCameraMovementSpeed();
			ImGui::InputFloat("Camera movement speed", &camera_movement_speed);
			perspective_camera_controller_.SetCameraMovementSpeed(camera_movement_speed);

			float camera_rotation_speed = perspective_camera_controller_.GetCameraRotationSpeed();
			ImGui::InputFloat("Camera rotation speed", &camera_rotation_speed);
			perspective_camera_controller_.SetCameraRotationSpeed(camera_rotation_speed);

			ImGui::Text("Yaw: %f", perspective_camera_controller_.GetYaw());
			ImGui::Text("Pitch: %f", perspective_camera_controller_.GetPitch());
			auto camera_front = perspective_camera_controller_.GetCamerFront();
			ImGui::Text("Camera front x: %f y: %f z: %f", camera_front.x, camera_front.y, camera_front.z);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Scene View")) {

			for (auto shape : scene_.GetShapes())
			{
				if (ImGui::TreeNode(shape->GetName().c_str())) {
					if (ImGui::TreeNode("Transform")) {
						// TODO(Rok Kos): Refac this
						auto shape_transform = shape->GetTransform();

						glm::vec3 t_pos = shape_transform->GetPosition();
						float pos[3] = { t_pos.x, t_pos.y, t_pos.z };
						ImGui::Text("Pos:"); ImGui::InputFloat3("a", &pos[0]);
						t_pos.x = pos[0];
						t_pos.y = pos[1];
						t_pos.z = pos[2];
						shape_transform->SetPosition(t_pos);


						glm::vec3 t_rot = shape_transform->GetRotation();
						float rot[3] = { t_rot.x, t_rot.y, t_rot.z };
						ImGui::Text("Rot:"); ImGui::InputFloat3("b", &rot[0]);
						t_rot.x = rot[0];
						t_rot.y = rot[1];
						t_rot.z = rot[2];
						shape_transform->SetRotation(t_rot);



						glm::vec3 t_scale = shape_transform->GetScale();
						float scl[3] = { t_scale.x, t_scale.y, t_scale.z };
						ImGui::Text("Scale:"); ImGui::InputFloat3("c", &scl[0]);
						t_scale.x = scl[0];
						t_scale.y = scl[1];
						t_scale.z = scl[2];
						shape_transform->SetScale(t_scale);

						ImGui::TreePop();
					}
					
					if (ImGui::TreeNode("Material")) {
						auto shape_material = shape->GetMaterial();
						ImGui::Text(shape_material->GetName().c_str());
						auto lighting_data = shape_material->GetPhongLightingParameters();
						
						ImGui::ColorEdit3("Diffuse Color:", glm::value_ptr(lighting_data.diffuse_color_));
						ImGui::ColorEdit3("Specular Color:", glm::value_ptr(lighting_data.specular_color_));
						ImGui::ColorEdit3("Ambient Color:", glm::value_ptr(lighting_data.ambient_color_));
						ImGui::InputFloat3("Ambient Intensity:", glm::value_ptr(lighting_data.ambient_intensity_));

						ImGui::SliderFloat("Specular scattering:", &lighting_data.specular_scattering_, 0.0f, 256.0f);

						shape_material->SetPhongLightingParameters(lighting_data);

						ImGui::TreePop();
					}


					ImGui::TreePop();
				}
			}

			for (auto light_source : scene_.GetLightSources())
			{
				if (ImGui::TreeNode(light_source->GetName().c_str())) {
					if (ImGui::TreeNode("Color Properties")) {
						glm::vec3 t_color = light_source->GetColor();
						ImGui::ColorEdit3("Color:", glm::value_ptr(t_color));
						light_source->SetColor(t_color);

						ImGui::TreePop();
					}


					if (ImGui::TreeNode("Transform")) {
						// TODO(Rok Kos): Refac this
						

						glm::vec3 t_pos = light_source->GetPosition();
						float pos[3] = { t_pos.x, t_pos.y, t_pos.z };
						ImGui::Text("Pos:"); ImGui::InputFloat3("a", &pos[0]);
						t_pos.x = pos[0];
						t_pos.y = pos[1];
						t_pos.z = pos[2];
						light_source->SetPosition(t_pos);


						glm::vec3 t_rot = light_source->GetDirection();
						float rot[3] = { t_rot.x, t_rot.y, t_rot.z };
						ImGui::Text("Rot:"); ImGui::InputFloat3("b", &rot[0]);
						t_rot.x = rot[0];
						t_rot.y = rot[1];
						t_rot.z = rot[2];
						light_source->SetDirection(t_rot);



						glm::vec3 t_scale = light_source->GetIntensity();
						float scl[3] = { t_scale.x, t_scale.y, t_scale.z };
						ImGui::Text("Scale:"); ImGui::InputFloat3("c", &scl[0]);
						t_scale.x = scl[0];
						t_scale.y = scl[1];
						t_scale.z = scl[2];
						light_source->SetIntensity(t_scale);

						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
			}
			
			ImGui::TreePop();
		}
		
		ImGui::End();

	}

	void EOLLayer::OnEvent(Core::Event& e)
	{
		Core::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Core::KeyPressedEvent>(BIND_EVENT_FN(EOLLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<Core::KeyTypedEvent>(BIND_EVENT_FN(EOLLayer::OnKeyTypedEvent));

		perspective_camera_controller_.OnEvent(e);
		Core::Layer::OnEvent(e);
	}

	bool EOLLayer::OnKeyPressedEvent(Core::KeyPressedEvent& e)
	{
		LOG_INFO("EOL LAYER::OnKeyPressedEvent key pressed: {0}", e.GetKeyCode());
		/*switch (e.GetKeyCode()) {
		case Core::KeyCode::Up: {
			glm::vec3 pos = transform_box_.GetPosition();
			pos.y += 1.0 * prev_time_step_;
			transform_box_.SetPosition(pos);
			return true;
		}
		case Core::KeyCode::Down: {
			glm::vec3 pos = transform_box_.GetPosition();
			pos.y -= 1.0 * prev_time_step_;
			transform_box_.SetPosition(pos);
			return true;
		}
		case Core::KeyCode::Left: {
			glm::vec3 pos = transform_box_.GetPosition();
			pos.x -= 1.0 * prev_time_step_;
			transform_box_.SetPosition(pos);
			return true;
		}
		case Core::KeyCode::Right: {
			glm::vec3 pos = transform_box_.GetPosition();
			pos.x += 1.0 * prev_time_step_;
			transform_box_.SetPosition(pos);
			return true;
		}
		}*/
		return false;
	}

	bool EOLLayer::OnKeyTypedEvent(Core::KeyTypedEvent& e)
	{
		LOG_INFO("EOLLayer::OnKeyTypedEvent key pressed: {0}", e.GetKeyCode());
		
		switch (e.GetKeyCode()) {
		case Core::KeyCode::H:
			//scene->step(gs->online, gs->exportObjs);
			return true;
		case Core::KeyCode::R:
			//scene->reset();  It was before commented out
			return true;
		case Core::KeyCode::P:
			//scene->partialStep();
			return true;
		case Core::KeyCode::V:
			//camera->toggleFlatView();
			return true;
		}

		return false;
	}

}