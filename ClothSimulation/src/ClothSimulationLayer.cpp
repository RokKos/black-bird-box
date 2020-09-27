#include "ClothSimulationLayer.h"

#include <cmath>
#include <functional>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <thread>

namespace ClothSimulation {
ClothSimulationLayer::ClothSimulationLayer()
    : BlackBirdBox::Layer("ClothSimulationLayer")
{
    PROFILE_FUNCTION();

    LoadAllShaders();
    LoadAllPrimitiveModels();

    perspective_camera_controller_ = BlackBirdBox::CreateRef<BlackBirdBox::PerspectiveCameraController>();
    perspective_camera_controller_->GetCamera().SetPosition(glm::vec3(0.0f, 0.5f, 1.5f));
    menus_.push_back(BlackBirdBox::CreateRef<BlackBirdBox::CameraMenu>("Camera Controls", perspective_camera_controller_));
    menus_.push_back(BlackBirdBox::CreateRef<BlackBirdBox::SceneViewMenu>("Scene View", scene_.GetShapes(), scene_.GetLightSources()));
    menus_.push_back(
        BlackBirdBox::CreateRef<BlackBirdBox::ComputeShaderMenu>("Compute Shader Simulation Controls", compute_shader_simulation_configuration_));
    menus_.push_back(BlackBirdBox::CreateRef<BlackBirdBox::MiscMenu>("Misc Menu", prev_time_step_, bg_color_, polygon_mode_));

    const auto& mat_generic_triangle = BlackBirdBox::CreateRef<BlackBirdBox::Material>(shader_library_.Get("TriangleTest"), "Generic_Triangle_MAT");
    const auto& mat_generic_color = BlackBirdBox::CreateRef<BlackBirdBox::Material>(shader_library_.Get("GenericColor"), "Generic_Color_MAT");
    const auto& mat_generic_normals = BlackBirdBox::CreateRef<BlackBirdBox::Material>(shader_library_.Get("GenericNormals"), "Generic_Normals_MAT");

    const auto& generic_uv_coordinates_shader = shader_library_.Get("GenericUVCoordinates");
    generic_uv_coordinates_shader->Bind();

    BlackBirdBox::Texture2DSpecification generic_tex_2d_spec = BlackBirdBox::Texture2DSpecification();
    generic_tex_2d_spec.TextureMinFilter = BlackBirdBox::TextureMinifyingFilter::LINEAR;
    generic_tex_2d_spec.TextureMagFilter = BlackBirdBox::TextureMagnificationFilter::NEAREST;

    auto uv_texture = BlackBirdBox::Texture2D::Create("assets/Textures/uv_texture.png", generic_tex_2d_spec);
    auto mat_generic_uv_coordinates = BlackBirdBox::CreateRef<BlackBirdBox::Material>(generic_uv_coordinates_shader, "Generic_UV_Coordinates_MAT");
    mat_generic_uv_coordinates->SetTexture("UV_TEST_Texture", uv_texture);
    mat_generic_uv_coordinates->SetUniform("u_Texture", 0);

    const auto& generic_texture_shader = shader_library_.Get("GenericTexture");
    generic_texture_shader->Bind();
    auto gun_texture = BlackBirdBox::Texture2D::Create("assets/Textures/Cerberus_A.tga", generic_tex_2d_spec);
    auto mat_generic_texture = BlackBirdBox::CreateRef<BlackBirdBox::Material>(generic_texture_shader, "Generic_Texture_MAT");
    mat_generic_texture->SetTexture("Gun_Texture", gun_texture);
    mat_generic_texture->SetUniform("u_Texture", 0);

    auto phong_lighting_parameters = BlackBirdBox::PhongLightingParameters();
    phong_lighting_parameters.diffuse_color_ = glm::vec3(188.0f / 255.0f, 185.0f / 255.0f, 167.0f / 255.0f);
    phong_lighting_parameters.specular_color_ = glm::vec3(1.0f, 1.0f, 1.0f);
    phong_lighting_parameters.specular_scattering_ = 15.0f;
    phong_lighting_parameters.ambient_color_ = glm::vec3(81.0f / 255.0f, 84.0f / 255.0f, 67.0f / 255.0f);
    phong_lighting_parameters.ambient_intensity_ = glm::vec3(0.21f, 0.21f, 0.21f);
    const auto& mat_generic_lighting
        = BlackBirdBox::CreateRef<BlackBirdBox::Material>(shader_library_.Get("GenericLighting"), "Generic_Lighting_MAT");
    mat_generic_lighting->SetUniform("u_Texture", 0);
    SetPhongParameters(mat_generic_lighting, phong_lighting_parameters);

    const auto& standard_shader = shader_library_.Get("FrameBufferShader");
    standard_shader->Bind();
    auto mat_standard_shader = BlackBirdBox::CreateRef<BlackBirdBox::Material>(standard_shader, "Standard_MAT");
    mat_standard_shader->SetTexture("Gun_Texture", gun_texture);
    mat_standard_shader->SetUniform("u_Texture", 0);
    SetPhongParameters(mat_standard_shader, phong_lighting_parameters);

    // Gun ------
    {
        auto vertex_array_gun = BlackBirdBox::VertexArray::Create();
        auto model_data = model_library_.Get("assets/Models/gun.obj");

        auto vertex_buffer_gun
            = BlackBirdBox::VertexBuffer::Create(model_data.vertices.data(), model_data.vertices.size() * sizeof(BlackBirdBox::Vertex));
        BlackBirdBox::BufferLayout layout_gun = {
            { BlackBirdBox::ShaderDataType::Float3, "a_Position" },
            { BlackBirdBox::ShaderDataType::Float3, "a_Normal" },
            { BlackBirdBox::ShaderDataType::Float2, "a_TexCoord" },
        };

        vertex_buffer_gun->SetLayout(layout_gun);
        vertex_array_gun->AddVertexBuffer(vertex_buffer_gun);

        BlackBirdBox::Ref<BlackBirdBox::IndexBuffer> index_buffer_gun
            = BlackBirdBox::IndexBuffer::Create(model_data.indices.data(), model_data.indices.size());
        vertex_array_gun->SetIndexBuffer(index_buffer_gun);

        // TODO(Rok Kos): Read from JSON file
        auto shape = BlackBirdBox::CreateRef<BlackBirdBox::Shape>(mat_generic_color, vertex_array_gun,
            BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(0, 0.5, 0)), model_data, "Obj Model Test");
        auto shape2 = BlackBirdBox::CreateRef<BlackBirdBox::Shape>(mat_generic_normals, vertex_array_gun,
            BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(1, 0, 0)), model_data, "Obj Model Normals Test");
        auto shape3 = BlackBirdBox::CreateRef<BlackBirdBox::Shape>(mat_generic_uv_coordinates, vertex_array_gun,
            BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(4, 0, 0)), model_data, "Obj Texture UVs Test");
        auto shape4 = BlackBirdBox::CreateRef<BlackBirdBox::Shape>(mat_generic_texture, vertex_array_gun,
            BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(6, 0, 0)), model_data, "Obj Texture Test");
        auto shape5 = BlackBirdBox::CreateRef<BlackBirdBox::Shape>(mat_generic_lighting, vertex_array_gun,
            BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(8, 0, 0)), model_data, "Obj Lighting Test");
        auto shape6 = BlackBirdBox::CreateRef<BlackBirdBox::Shape>(mat_standard_shader, vertex_array_gun,
            BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(0, 0.5, 0)), model_data, "Obj All Together");
        // scene_.AddShape(shape);
        // scene_.AddShape(shape2);
        // scene_.AddShape(shape3);
        // scene_.AddShape(shape4);
        // scene_.AddShape(shape5);
        // scene_.AddShape(shape6);
    }

    // City01 ------
    {
        auto vertex_array_ship = BlackBirdBox::VertexArray::Create();
        auto model_data_ship = model_library_.Get("assets/Models/LowPolyCity.obj");

        auto vertex_buffer_ship
            = BlackBirdBox::VertexBuffer::Create(model_data_ship.vertices.data(), model_data_ship.vertices.size() * sizeof(BlackBirdBox::Vertex));
        BlackBirdBox::BufferLayout layout_ship = {
            { BlackBirdBox::ShaderDataType::Float3, "a_Position" },
            { BlackBirdBox::ShaderDataType::Float3, "a_Normal" },
            { BlackBirdBox::ShaderDataType::Float2, "a_TexCoord" },
        };

        vertex_buffer_ship->SetLayout(layout_ship);
        vertex_array_ship->AddVertexBuffer(vertex_buffer_ship);

        BlackBirdBox::Ref<BlackBirdBox::IndexBuffer> index_buffer_ship
            = BlackBirdBox::IndexBuffer::Create(model_data_ship.indices.data(), model_data_ship.indices.size());
        vertex_array_ship->SetIndexBuffer(index_buffer_ship);

        auto city_texture = BlackBirdBox::Texture2D::Create("assets/Textures/Palette.jpg", generic_tex_2d_spec);
        auto mat_stadard_shader_city = BlackBirdBox::CreateRef<BlackBirdBox::Material>(standard_shader, "Standard_City_MAT");
        mat_stadard_shader_city->SetTexture("City_Texture", city_texture);
        mat_stadard_shader_city->SetUniform("u_Texture", 0);
        SetPhongParameters(mat_stadard_shader_city, phong_lighting_parameters);

        auto ship = BlackBirdBox::CreateRef<BlackBirdBox::Shape>(mat_stadard_shader_city, vertex_array_ship,
            BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(-1.5, 0.5, 0), glm::vec3(0.0), glm::vec3(0.1, 0.1, 0.1)), model_data_ship,
            "City 01");
        // scene_.AddShape(ship);
    }

    const auto& blinn_phong_shader = shader_library_.Get("BlinnPhongShader");
    blinn_phong_shader->Bind();
    // City02 ------
    {
        auto vertex_array_ship = BlackBirdBox::VertexArray::Create();
        auto model_data_ship = model_library_.Get("assets/Models/LowPolyCity.obj");

        auto vertex_buffer_ship
            = BlackBirdBox::VertexBuffer::Create(model_data_ship.vertices.data(), model_data_ship.vertices.size() * sizeof(BlackBirdBox::Vertex));
        BlackBirdBox::BufferLayout layout_ship = {
            { BlackBirdBox::ShaderDataType::Float3, "a_Position" },
            { BlackBirdBox::ShaderDataType::Float3, "a_Normal" },
            { BlackBirdBox::ShaderDataType::Float2, "a_TexCoord" },
        };

        vertex_buffer_ship->SetLayout(layout_ship);
        vertex_array_ship->AddVertexBuffer(vertex_buffer_ship);

        BlackBirdBox::Ref<BlackBirdBox::IndexBuffer> index_buffer_ship
            = BlackBirdBox::IndexBuffer::Create(model_data_ship.indices.data(), model_data_ship.indices.size());
        vertex_array_ship->SetIndexBuffer(index_buffer_ship);

        auto city_texture = BlackBirdBox::Texture2D::Create("assets/Textures/Palette.jpg", generic_tex_2d_spec);
        auto mat_stadard_shader_city = BlackBirdBox::CreateRef<BlackBirdBox::Material>(blinn_phong_shader, "Standard_MAT");
        mat_stadard_shader_city->SetUniform("u_Texture", 0);
        mat_stadard_shader_city->SetTexture("City_Texture", city_texture);

        auto ship = BlackBirdBox::CreateRef<BlackBirdBox::Shape>(mat_stadard_shader_city, vertex_array_ship,
            BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(1.5, 0.5, 0), glm::vec3(0.0), glm::vec3(0.1, 0.1, 0.1)), model_data_ship,
            "City 02");
        // scene_.AddShape(ship);
    }

    scene_.AddLightSource(BlackBirdBox::CreateRef<BlackBirdBox::LightSource>(BlackBirdBox::LightType::kDirectional,
        BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(0, 5, 0), glm::vec3(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)),
        glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.f, 1.f, 1.f)));

    scene_.AddPoint(BlackBirdBox::CreateRef<BlackBirdBox::Point>(10, glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)));
    scene_.AddPoint(BlackBirdBox::CreateRef<BlackBirdBox::Point>(100, glm::vec3(10, 2, 5), glm::vec3(0, 1, 0)));

    // Enviroment map ------
    const auto& enviroment_map_shader = shader_library_.Get("EnviromentMapShader");
    enviroment_map_shader->Bind();
    enviroment_map_material_ = BlackBirdBox::CreateRef<BlackBirdBox::Material>(enviroment_map_shader, "EnviromentMap_MAT");
    enviroment_map_material_->SetUniform("u_EnviromentMap", 0);

    vertex_array_box_ = BlackBirdBox::VertexArray::Create();

    float enviroment_map_box_vertices[] = {
        // Front face
        -1.0,
        -1.0,
        1.0,
        1.0,
        -1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        -1.0,
        1.0,
        1.0,

        // Back face
        -1.0,
        -1.0,
        -1.0,
        -1.0,
        1.0,
        -1.0,
        1.0,
        1.0,
        -1.0,
        1.0,
        -1.0,
        -1.0,

        // Top face
        -1.0,
        1.0,
        -1.0,
        -1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        -1.0,

        // Bottom face
        -1.0,
        -1.0,
        -1.0,
        1.0,
        -1.0,
        -1.0,
        1.0,
        -1.0,
        1.0,
        -1.0,
        -1.0,
        1.0,

        // Right face
        1.0,
        -1.0,
        -1.0,
        1.0,
        1.0,
        -1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        -1.0,
        1.0,

        // Left face
        -1.0,
        -1.0,
        -1.0,
        -1.0,
        -1.0,
        1.0,
        -1.0,
        1.0,
        1.0,
        -1.0,
        1.0,
        -1.0,
    };

    uint32_t enviroment_map_box_indices[] = {
        0, 1, 2, 0, 2, 3, // front
        4, 5, 6, 4, 6, 7, // back
        8, 9, 10, 8, 10, 11, // top
        12, 13, 14, 12, 14, 15, // bottom
        16, 17, 18, 16, 18, 19, // right
        20, 21, 22, 20, 22, 23, // left
    };

    auto vertex_buffer_box = BlackBirdBox::VertexBuffer::Create(enviroment_map_box_vertices, sizeof(enviroment_map_box_vertices));
    BlackBirdBox::BufferLayout layout_box = {
        { BlackBirdBox::ShaderDataType::Float3, "a_Position" },
    };

    vertex_buffer_box->SetLayout(layout_box);
    vertex_array_box_->AddVertexBuffer(vertex_buffer_box);

    BlackBirdBox::Ref<BlackBirdBox::IndexBuffer> index_buffer_box
        = BlackBirdBox::IndexBuffer::Create(enviroment_map_box_indices, sizeof(enviroment_map_box_indices));
    vertex_array_box_->SetIndexBuffer(index_buffer_box);

    enviroment_map_ = BlackBirdBox::CubeMap::Create("assets/Textures/CubeMap/");

    // Compute Cloth

    ParseSimulationSettings();

    cloth_ = BlackBirdBox::CreateRef<BlackBirdBox::Cloth>(num_cloth_dimension_size_, mat_generic_triangle);
    // TODO(Rok Kos): Move this
    compute_shader_simulation_configuration_.SetHorizontalVerticalDistanceBetweenVertexes(cloth_->GetHorizontalVerticalDistanceBetweenVertexes());
    compute_shader_simulation_configuration_.SetDiagonalDistanceBetweenVertexes(cloth_->GetDiagonalDistanceBetweenVertexes());
    compute_shader_simulation_configuration_.SetBendDistanceBetweenVertexes(cloth_->GetBendDistanceBetweenVertexes());
    scene_.AddShape(cloth_);

    const auto& verlet_integration_shader = shader_library_.Get("VerletIntegrationShader");
    vertlet_compute_material_ = BlackBirdBox::CreateRef<BlackBirdBox::Material>(verlet_integration_shader, "VerletIntegration_MAT");
    vertlet_compute_material_->SetUniform("u_Gravity", compute_shader_simulation_configuration_.GetGravity());
    vertlet_compute_material_->SetUniform("u_DeltaTime", compute_shader_simulation_configuration_.GetDeltaTime());
    vertlet_compute_material_->SetUniform("u_External_Force", compute_shader_simulation_configuration_.GetExternalForce());
    vertlet_compute_material_->SetUniform("u_Wind_Resistance", compute_shader_simulation_configuration_.GetWindResistance());

    const auto& constrains_shader = shader_library_.Get("ConstraintsShader");
    constraint_compute_material_ = BlackBirdBox::CreateRef<BlackBirdBox::Material>(constrains_shader, "Constraints_MAT");
    constraint_compute_material_->SetUniform("u_Iterations", compute_shader_simulation_configuration_.GetIterations());
    constraint_compute_material_->SetUniform(
        "u_Horizontal_Vertical_Rest_Lenght", compute_shader_simulation_configuration_.GetHorizontalVerticalDistanceBetweenVertexes());
    constraint_compute_material_->SetUniform("u_Diagonal_Rest_Lenght", compute_shader_simulation_configuration_.GetDiagonalDistanceBetweenVertexes());
    constraint_compute_material_->SetUniform("u_Bend_Lenght", compute_shader_simulation_configuration_.GetBendDistanceBetweenVertexes());
    constraint_compute_material_->SetUniform("u_Structural_Stiffness", compute_shader_simulation_configuration_.GetStructuralStiffness());
    constraint_compute_material_->SetUniform("u_Shear_Stiffness", compute_shader_simulation_configuration_.GetShearStiffness());
    constraint_compute_material_->SetUniform("u_Flexion_Stiffness", compute_shader_simulation_configuration_.GetFlexionStiffness());

    // Frame Buffer
    BlackBirdBox::FramebufferSpecification frame_buffer_spec = BlackBirdBox::FramebufferSpecification();
    frame_buffer_spec.width = 1920;
    frame_buffer_spec.height = 1080;
    frame_buffer_spec.frame_buffer_attachments.push_back(BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT1);
    frame_buffer_spec.frame_buffer_attachments.push_back(BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT2);
    frame_buffer_spec.frame_buffer_attachments.push_back(BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT3);
    frame_buffer_spec.frame_buffer_attachments.push_back(BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT4);
    frame_buffer_spec.frame_buffer_attachments.push_back(BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT5);
    frame_buffer_spec.frame_buffer_attachments.push_back(BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT6);
    frame_buffer_spec.frame_buffer_attachments.push_back(BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT7);
    test_frame_buffer_ = BlackBirdBox::FrameBuffer::Create(frame_buffer_spec);
}

void ClothSimulationLayer::OnAttach() { BlackBirdBox::Layer::OnAttach(); }

void ClothSimulationLayer::OnDetach() { BlackBirdBox::Layer::OnDetach(); }

void ClothSimulationLayer::OnUpdate(BlackBirdBox::TimeStep ts)
{
    BlackBirdBox::Layer::OnUpdate(ts);
    perspective_camera_controller_->OnUpdate(ts);

    BlackBirdBox::Renderer::BeginScene(perspective_camera_controller_->GetCamera(), scene_.GetLightSources());

    prev_time_step_ = ts;

    BlackBirdBox::RenderCommand::SetClearColor(bg_color_);
    BlackBirdBox::RenderCommand::Clear();

    BlackBirdBox::Renderer::Submit(enviroment_map_material_, enviroment_map_, vertex_array_box_);

    BlackBirdBox::Renderer::DispatchComputeShader(vertlet_compute_material_, cloth_->GetClothStorageArray(0), compute_shader_configuration_);

    for (size_t batch_id = 0; batch_id < cloth_->GetNumberOfBatches(); ++batch_id) {
        const size_t number_of_edges = cloth_->GetBatches()[batch_id].size();
        const BlackBirdBox::ComputeShaderConfiguration compute_shader_configuration(
            { static_cast<unsigned int>(number_of_edges), 1, 1 }, { 1, 1, 1 });
        BlackBirdBox::Renderer::DispatchComputeShader(
            constraint_compute_material_, cloth_->GetClothStorageArray(batch_id), compute_shader_configuration);
    }

    // TODO(Rok Kos): Load Models on themand

    BlackBirdBox::RenderCommand::SetPolygonMode((BlackBirdBox::RendererAPI::PolygonMode)polygon_mode_);

    test_frame_buffer_->Bind();

    for (const auto& shape : scene_.GetShapes()) {
        if (shape->GetObjectEnabled()) {
            BlackBirdBox::Renderer::Submit(shape->GetMaterial(), shape->GetVertexArray(), shape->GetTransform()->GetTransformMatrix());
        }
    }
    test_frame_buffer_->Unbind();

    for (const auto& shape : scene_.GetShapes()) {
        if (shape->GetObjectEnabled()) {
            BlackBirdBox::Renderer::Submit(shape->GetMaterial(), shape->GetVertexArray(), shape->GetTransform()->GetTransformMatrix());
        }
    }

    for (const auto& light_source : scene_.GetLightSources()) {
        if (light_source->GetObjectEnabled()) {
            BlackBirdBox::Renderer::Submit(BlackBirdBox::CreateRef<BlackBirdBox::Material>(shader_library_.Get("GenericColor"), "Generic_Color_MAT"),
                vertex_array_box_, light_source->GetTransform()->GetTransformMatrix());
        }
    }

    BlackBirdBox::RenderCommand::SetPolygonMode(BlackBirdBox::RendererAPI::PolygonMode::FILL);

    BlackBirdBox::Renderer::EndScene();
}

void ClothSimulationLayer::OnImGuiRender()
{
    BlackBirdBox::Layer::OnImGuiRender();

    for (const auto& menu : menus_) {
        menu->OnImGuiRender();
    }

    ImGui::Begin("Cloth");

    ImGui::InputFloat4("Gravity", glm::value_ptr(fixed_point_to_move_), 10);

    const std::vector<BlackBirdBox::Ref<BlackBirdBox::ShaderStorageBuffer>>& cloth_shader_storage_buffers
        = cloth_->GetClothStorageArray(0)->GetShaderStorageBuffers();
    const BlackBirdBox::Ref<BlackBirdBox::ShaderStorageBuffer> fixed_points_shader_storage_buffer = cloth_shader_storage_buffers[2];
    fixed_points_shader_storage_buffer->SetPersistentDataIndex(fixed_point_to_move_, num_cloth_dimension_size_ * num_cloth_dimension_size_ - 1);

    ImGui::End();
    /*ImGui::Begin("ViewPort");
    int i = 0;
    for (BlackBirdBox::FrameBufferAttachments attachment :
    test_frame_buffer_->GetFrameBufferSpecification().frame_buffer_attachments) {
        // TODO(Rok Kos): Investigate why depth buffer is not rendering
        if (attachment == BlackBirdBox::FrameBufferAttachments::DEPTH_STENCIL_ATTACHMENT) {
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
        } else {
            i += 4;
        }
    }
    ImGui::End();
    */
}

void ClothSimulationLayer::OnEvent(BlackBirdBox::Event& e)
{
    BlackBirdBox::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<BlackBirdBox::KeyPressedEvent>(BIND_EVENT_FN(ClothSimulationLayer::OnKeyPressedEvent));
    dispatcher.Dispatch<BlackBirdBox::KeyTypedEvent>(BIND_EVENT_FN(ClothSimulationLayer::OnKeyTypedEvent));

    perspective_camera_controller_->OnEvent(e);
    BlackBirdBox::Layer::OnEvent(e);
}

bool ClothSimulationLayer::OnKeyPressedEvent(BlackBirdBox::KeyPressedEvent& e)
{
    LOG_INFO("ClothSimulation LAYER::OnKeyPressedEvent key pressed: {0}", e.GetKeyCode());
    return false;
}

bool ClothSimulationLayer::OnKeyTypedEvent(BlackBirdBox::KeyTypedEvent& e)
{
    LOG_INFO("ClothSimulationLayer::OnKeyTypedEvent key pressed: {0}", e.GetKeyCode());

    return false;
}

void ClothSimulationLayer::ParseSimulationSettings()
{
    auto simulation_config = BlackBirdBox::JsonUtil::ReadJson("assets/Configs/SimulationConfig.json");
    ASSERT(simulation_config["ClothSimulationSettings"].IsObject(), "ClothSimulationSettings is not an object!");
    const auto cloth_simulation_settings = simulation_config["ClothSimulationSettings"].GetObject();

    ASSERT(cloth_simulation_settings["ClothDimensionSize"].IsInt(), "ClothDimensionSize is not an int!");
    num_cloth_dimension_size_ = cloth_simulation_settings["ClothDimensionSize"].GetInt();

    ASSERT(cloth_simulation_settings["Gravity"].IsArray(), "Gravity is not an array!");
    auto& gravity_json = cloth_simulation_settings["Gravity"];
    ASSERT(gravity_json.Size() == 3, "Gravity is not size 3!");

    glm::vec3 gravity = glm::vec3(0.0);
    for (rapidjson::SizeType i = 0; i < gravity_json.Size(); i++) {
        gravity[i] = gravity_json[i].GetFloat();
    }

    ASSERT(cloth_simulation_settings["ExternalForce"].IsArray(), "ExternalForce is not an array!");
    auto& external_force_json = cloth_simulation_settings["ExternalForce"];
    ASSERT(gravity_json.Size() == 3, "ExternalForce is not size 3!");
    glm::vec3 external_force = glm::vec3(0.0);
    for (rapidjson::SizeType i = 0; i < external_force_json.Size(); i++) {
        external_force[i] = external_force_json[i].GetFloat();
    }

    ASSERT(cloth_simulation_settings["WindResistance"].IsArray(), "WindResistance is not an array!");
    auto& wind_resistance_json = cloth_simulation_settings["WindResistance"];
    ASSERT(gravity_json.Size() == 3, "WindResistance is not size 3!");
    glm::vec3 wind_resistance = glm::vec3(0.0);
    for (rapidjson::SizeType i = 0; i < wind_resistance_json.Size(); i++) {
        wind_resistance[i] = wind_resistance_json[i].GetFloat();
    }

    ASSERT(cloth_simulation_settings["SimulationDeltaTime"].IsFloat(), "SimulationDeltaTime is not an float!");
    const float simulation_delta_time = cloth_simulation_settings["SimulationDeltaTime"].GetFloat();

    ASSERT(cloth_simulation_settings["ConstraintIterations"].IsInt(), "ConstraintIteratitions is not an int!");
    const int constraint_iterations = cloth_simulation_settings["ConstraintIterations"].GetInt();

    ASSERT(cloth_simulation_settings["StructuralStiffness"].IsFloat(), "StructuralStiffness is not an float!");
    const float structural_stiffness = cloth_simulation_settings["StructuralStiffness"].GetFloat();

    ASSERT(cloth_simulation_settings["ShearStiffness"].IsFloat(), "ShearStiffness is not an float!");
    const float shear_stiffness = cloth_simulation_settings["ShearStiffness"].GetFloat();

    ASSERT(cloth_simulation_settings["FlexionStiffness"].IsFloat(), "FlexionStiffness is not an float!");
    const float flexion_stiffness = cloth_simulation_settings["FlexionStiffness"].GetFloat();

    compute_shader_simulation_configuration_ = BlackBirdBox::ComputeShaderSimulationConfiguration(gravity, simulation_delta_time, external_force,
        wind_resistance, constraint_iterations, structural_stiffness, shear_stiffness, flexion_stiffness);

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

    compute_shader_configuration_ = BlackBirdBox::ComputeShaderConfiguration(work_group_config, local_group_config);
}

void ClothSimulationLayer::LoadAllShaders()
{
    PROFILE_FUNCTION();

    auto load_shader = BlackBirdBox::JsonUtil::ReadJson("assets/Configs/LoadShader.json");
    ASSERT(load_shader["Shaders"].IsArray(), "Shaders is not an array!");

    for (auto& shader_path : load_shader["Shaders"].GetArray()) {
        ASSERT(shader_path.IsString(), "Shader Path is not string");
        shader_library_.Load(shader_path.GetString());
    }
}

void ClothSimulationLayer::LoadAllPrimitiveModels()
{
    PROFILE_FUNCTION();
    auto load_model = BlackBirdBox::JsonUtil::ReadJson("assets/Configs/LoadModels.json");
    ASSERT(load_model["Models"].IsArray(), "Models is not an array!");

    std::vector<std::thread> threads;

    for (auto& model_path : load_model["Models"].GetArray()) {
        ASSERT(model_path.IsString(), "Models Path is not string");
        threads.push_back(std::thread(&ClothSimulationLayer::LoadModelInThread, this, model_path.GetString()));
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void ClothSimulationLayer::LoadModelInThread(const std::string& filepath)
{
    PROFILE_FUNCTION();

    model_library_.Load(filepath);
}

std::string ClothSimulationLayer::FrameBufferAttachmentToName(BlackBirdBox::FrameBufferAttachments attachment)
{
    switch (attachment) {
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT0:
        return "Main Color Output";
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT1:
        return "Normals Output";
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT2:
        return "UV Output";
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT3:
        return "vertex Positions Output";
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT4:
        return "Model Positions Output";
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT5:
        return "Light Diffuse Color";
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT6:
        return "Light Specular Color";
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT7:
        return "Light All Color";
    case BlackBirdBox::FrameBufferAttachments::DEPTH_STENCIL_ATTACHMENT:
        return "Depth/Stencil Buffer";
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT8:
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT9:
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT10:
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT11:
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT12:
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT13:
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT14:
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT15:
    case BlackBirdBox::FrameBufferAttachments::DEPTH_ATTACHMENT:
    case BlackBirdBox::FrameBufferAttachments::STENCIL_ATTACHMENT:
    default:
        break;
    }

    return "";
}

void ClothSimulationLayer::SetPhongParameters(
    const BlackBirdBox::Ref<BlackBirdBox::Material>& material, BlackBirdBox::PhongLightingParameters phong_lighting_parameters)
{
    material->SetUniform("u_DiffuseColor", phong_lighting_parameters.diffuse_color_);
    material->SetUniform("u_SpecularColor", phong_lighting_parameters.specular_color_);
    material->SetUniform("u_SpecularScatering", phong_lighting_parameters.specular_scattering_);
    material->SetUniform("u_AmbientColor", phong_lighting_parameters.ambient_color_);
    material->SetUniform("u_AmbientIntensity", phong_lighting_parameters.ambient_color_);
}
} // namespace ClothSimulation
