// FILE: ./Flow/src/Utils/DebugOverlay.h
#pragma once

#include <imgui.h>
#include <entt/entt.hpp>
#include "ECS/Components.h"

namespace Flow
{
    class DebugOverlay
    {
    public:
        static inline bool bIsVisible = false;

        static void Draw(float CurrentFPS, entt::registry& Registry, entt::entity TrackedEntityID)
        {
            if (!bIsVisible)
            {
                return;
            }

            ImGuiIO& IO = ImGui::GetIO();

            // Configure immediate UI layout flags to pin down overlay workspace
            ImGuiWindowFlags WindowFlags =
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav |
                ImGuiWindowFlags_NoMove;

            // Compute alignment anchors to pin window viewports to the bottom-left corner
            ImVec2 WindowPos = ImVec2(10.0f, IO.DisplaySize.y - 10.0f);
            ImVec2 WindowPosPivot = ImVec2(0.0f, 1.0f);
            ImGui::SetNextWindowPos(WindowPos, ImGuiCond_Always, WindowPosPivot);
            ImGui::SetNextWindowBgAlpha(0.35f);

            if (ImGui::Begin("BottomOverlay", &bIsVisible, WindowFlags))
            {
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "FLOW ENGINE DEBUG (~) ");
                ImGui::Separator();
                ImGui::Text("FPS: %.1f", CurrentFPS);
                ImGui::Separator();

                // Safely assert memory tracking scopes before accessing components
                if (Registry.valid(TrackedEntityID))
                {
                    if (auto* TransformPtr = Registry.try_get<Transform>(TrackedEntityID))
                    {
                        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Tracking Entity: %d", static_cast<uint32_t>(TrackedEntityID));
                        ImGui::Text("PosX: %.2f | PosY: %.2f", TransformPtr->x, TransformPtr->y);
                    }
                }
                else
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Tracked Entity %d is DEAD or INVALID", static_cast<uint32_t>(TrackedEntityID));
                }
            }
            ImGui::End();
        }
    };
}