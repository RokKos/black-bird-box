#include "ClothSimulationLayer.h"

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

    perspective_camera_controller_ = BlackBirdBox::CreateRef<BlackBirdBox::PerspectiveCameraController>();
    perspective_camera_controller_->GetCamera().SetPosition(glm::vec3(0.0f, 0.5f, 1.5f));
    menus_.push_back(BlackBirdBox::CreateRef<BlackBirdBox::CameraMenu>("Camera Controls", perspective_camera_controller_));
    menus_.push_back(BlackBirdBox::CreateRef<BlackBirdBox::SceneViewMenu>("Scene View", scene_.GetShapes(), scene_.GetLightSources()));
    menus_.push_back(
        BlackBirdBox::CreateRef<BlackBirdBox::ComputeShaderMenu>("Compute Shader Simulation Controls", compute_shader_simulation_configuration_));
    menus_.push_back(BlackBirdBox::CreateRef<BlackBirdBox::MiscMenu>("Misc Menu", prev_time_step_, bg_color_, polygon_mode_));

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

    scene_.AddLightSource(BlackBirdBox::CreateRef<BlackBirdBox::LightSource>(BlackBirdBox::LightType::kDirectional,
        BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(0, 1.5f, 1.5f), glm::vec3(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)),
        glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.f, 1.f, 1.f)));

    // Compute Cloth

    ParseSimulationSettings();
    const auto& mat_generic_triangle = BlackBirdBox::CreateRef<BlackBirdBox::Material>(shader_library_.Get("TriangleTest"), "Generic_Triangle_MAT");
    auto phong_lighting_parameters = BlackBirdBox::PhongLightingParameters();
    phong_lighting_parameters.diffuse_color_ = glm::vec3(188.0f / 255.0f, 185.0f / 255.0f, 167.0f / 255.0f);
    phong_lighting_parameters.specular_color_ = glm::vec3(1.0f, 1.0f, 1.0f);
    phong_lighting_parameters.specular_scattering_ = 15.0f;
    phong_lighting_parameters.ambient_color_ = glm::vec3(81.0f / 255.0f, 84.0f / 255.0f, 67.0f / 255.0f);
    phong_lighting_parameters.ambient_intensity_ = glm::vec3(0.21f, 0.21f, 0.21f);
    SetPhongParameters(mat_generic_triangle, phong_lighting_parameters);

    cloth_ = BlackBirdBox::CreateRef<BlackBirdBox::Cloth>(num_cloth_dimension_size_, mat_generic_triangle);
    // TODO(Rok Kos): Move this
    compute_shader_simulation_configuration_.SetHorizontalVerticalDistanceBetweenVertexes(cloth_->GetHorizontalVerticalDistanceBetweenVertexes());
    compute_shader_simulation_configuration_.SetDiagonalDistanceBetweenVertexes(cloth_->GetDiagonalDistanceBetweenVertexes());
    compute_shader_simulation_configuration_.SetBendDistanceBetweenVertexes(cloth_->GetBendDistanceBetweenVertexes());
    scene_.AddShape(cloth_);

    const auto& verlet_integration_shader = shader_library_.Get("VerletIntegrationShader");
    vertlet_compute_material_ = BlackBirdBox::CreateRef<BlackBirdBox::Material>(verlet_integration_shader, "VerletIntegration_MAT");

    const auto& constrains_shader = shader_library_.Get("ConstraintsShader");
    constraint_compute_material_ = BlackBirdBox::CreateRef<BlackBirdBox::Material>(constrains_shader, "Constraints_MAT");

    const auto& cloth_normals_shader = shader_library_.Get("ClothNormalsShader");
    cloth_compute_normals_material_ = BlackBirdBox::CreateRef<BlackBirdBox::Material>(cloth_normals_shader, "ClothNormals_MAT");
}

void ClothSimulationLayer::OnAttach() { Layer::OnAttach(); }

void ClothSimulationLayer::OnDetach() { Layer::OnDetach(); }

void ClothSimulationLayer::OnUpdate(BlackBirdBox::TimeStep ts)
{
    Layer::OnUpdate(ts);
    perspective_camera_controller_->OnUpdate(ts);

    BlackBirdBox::Renderer::BeginScene(perspective_camera_controller_->GetCamera(), scene_.GetLightSources());

    prev_time_step_ = ts;

    BlackBirdBox::RenderCommand::SetClearColor(bg_color_);
    BlackBirdBox::RenderCommand::Clear();

    BlackBirdBox::Renderer::Submit(enviroment_map_material_, enviroment_map_, vertex_array_box_);

    BlackBirdBox::RenderCommand::SetPolygonMode((BlackBirdBox::RendererAPI::PolygonMode)polygon_mode_);

    SetSimualtionMaterialProperties();

    BlackBirdBox::Renderer::DispatchComputeShader(vertlet_compute_material_, cloth_->GetClothStorageArray(0), compute_shader_configuration_);

    for (size_t batch_id = 0; batch_id < cloth_->GetNumberOfBatches(); ++batch_id) {
        const size_t number_of_edges = cloth_->GetBatches()[batch_id].size();
        const BlackBirdBox::ComputeShaderConfiguration compute_shader_configuration(
            { static_cast<unsigned int>(number_of_edges), 1, 1 }, { 1, 1, 1 });
        BlackBirdBox::Renderer::DispatchComputeShader(
            constraint_compute_material_, cloth_->GetClothStorageArray(batch_id), compute_shader_configuration);
    }

    BlackBirdBox::Renderer::DispatchComputeShader(cloth_compute_normals_material_, cloth_->GetClothStorageArray(0), compute_shader_configuration_);

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
    Layer::OnImGuiRender();

    for (const auto& menu : menus_) {
        menu->OnImGuiRender();
    }

    ImGui::Begin("Cloth");

    ImGui::InputFloat4("Point To Move", glm::value_ptr(fixed_point_to_move_), 10);

    const std::vector<BlackBirdBox::Ref<BlackBirdBox::ShaderStorageBuffer>>& cloth_shader_storage_buffers
        = cloth_->GetClothStorageArray(0)->GetShaderStorageBuffers();
    const BlackBirdBox::Ref<BlackBirdBox::ShaderStorageBuffer> fixed_points_shader_storage_buffer = cloth_shader_storage_buffers[2];
    fixed_points_shader_storage_buffer->SetPersistentDataIndex(fixed_point_to_move_, num_cloth_dimension_size_ * num_cloth_dimension_size_ - 1);

    ImGui::End();
}

void ClothSimulationLayer::OnEvent(BlackBirdBox::Event& e)
{
    perspective_camera_controller_->OnEvent(e);
    Layer::OnEvent(e);
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

void ClothSimulationLayer::SetPhongParameters(
    const BlackBirdBox::Ref<BlackBirdBox::Material>& material, BlackBirdBox::PhongLightingParameters phong_lighting_parameters)
{
    material->SetUniform("u_DiffuseColor", phong_lighting_parameters.diffuse_color_);
    material->SetUniform("u_SpecularColor", phong_lighting_parameters.specular_color_);
    material->SetUniform("u_SpecularScatering", phong_lighting_parameters.specular_scattering_);
    material->SetUniform("u_AmbientColor", phong_lighting_parameters.ambient_color_);
    material->SetUniform("u_AmbientIntensity", phong_lighting_parameters.ambient_color_);
}

void ClothSimulationLayer::SetSimualtionMaterialProperties()
{
    vertlet_compute_material_->SetUniform("u_Gravity", compute_shader_simulation_configuration_.GetGravity());
    vertlet_compute_material_->SetUniform("u_DeltaTime", compute_shader_simulation_configuration_.GetDeltaTime());
    vertlet_compute_material_->SetUniform("u_External_Force", compute_shader_simulation_configuration_.GetExternalForce());
    vertlet_compute_material_->SetUniform("u_Wind_Resistance", compute_shader_simulation_configuration_.GetWindResistance());

    constraint_compute_material_->SetUniform("u_Iterations", compute_shader_simulation_configuration_.GetIterations());
    constraint_compute_material_->SetUniform(
        "u_Horizontal_Vertical_Rest_Lenght", compute_shader_simulation_configuration_.GetHorizontalVerticalDistanceBetweenVertexes());
    constraint_compute_material_->SetUniform("u_Diagonal_Rest_Lenght", compute_shader_simulation_configuration_.GetDiagonalDistanceBetweenVertexes());
    constraint_compute_material_->SetUniform("u_Bend_Lenght", compute_shader_simulation_configuration_.GetBendDistanceBetweenVertexes());
    constraint_compute_material_->SetUniform("u_Structural_Stiffness", compute_shader_simulation_configuration_.GetStructuralStiffness());
    constraint_compute_material_->SetUniform("u_Shear_Stiffness", compute_shader_simulation_configuration_.GetShearStiffness());
    constraint_compute_material_->SetUniform("u_Flexion_Stiffness", compute_shader_simulation_configuration_.GetFlexionStiffness());
}
} // namespace ClothSimulation
