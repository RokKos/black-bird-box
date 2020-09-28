﻿#include "BlackBirdCageLayer.h"

#include <imgui.h>

#include "glm/gtc/type_ptr.hpp"

namespace BlackBirdCage {
BlackBirdCageLayer::BlackBirdCageLayer()
    : Layer("BlackBirdCageLayer")
{
}

void BlackBirdCageLayer::OnAttach()
{
    PROFILE_FUNCTION();

    BlackBirdBox::FramebufferSpecification frame_buffer_specification;
    frame_buffer_specification.width = 1280;
    frame_buffer_specification.height = 720;
    frame_buffer_ = BlackBirdBox::FrameBuffer::Create(frame_buffer_specification);
}

void BlackBirdCageLayer::OnDetach() { PROFILE_FUNCTION(); }

void BlackBirdCageLayer::OnUpdate(BlackBirdBox::TimeStep ts)
{
    PROFILE_FUNCTION();

    // Resize
    BlackBirdBox::FramebufferSpecification spec = frame_buffer_->GetFrameBufferSpecification();
    if (viewport_size_.x > 0.0f && viewport_size_.y > 0.0f && (spec.width != viewport_size_.x || spec.height != viewport_size_.y)) {
        frame_buffer_->Resize((uint32_t)viewport_size_.x, (uint32_t)viewport_size_.y);
    }

    // Render
    frame_buffer_->Bind();
    BlackBirdBox::RenderCommand::SetClearColor(bg_color_);
    BlackBirdBox::RenderCommand::Clear();

    // Update scene
    // m_ActiveScene->OnUpdate(ts);

    frame_buffer_->Unbind();
}

void BlackBirdCageLayer::OnImGuiRender()
{
    PROFILE_FUNCTION();

    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to
    // not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

            if (ImGui::MenuItem("Exit"))
                BlackBirdBox::Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("BG Properties");

    ImGui::ColorEdit4("BG Color", glm::value_ptr(bg_color_));

    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");

    viewport_focused_ = ImGui::IsWindowFocused();
    viewport_hovered_ = ImGui::IsWindowHovered();
    BlackBirdBox::Application::Get().GetImGuiLayer()->SetBlocksEvents(!viewport_focused_ || !viewport_hovered_);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    viewport_size_ = { viewportPanelSize.x, viewportPanelSize.y };

    uint64_t textureID = frame_buffer_->GetTextureAttachment(BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT0)->GetRenderID();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewport_size_.x, viewport_size_.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void BlackBirdCageLayer::OnEvent(BlackBirdBox::Event& e) { PROFILE_FUNCTION(); }
}