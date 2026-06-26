/*
#pragma once
#ifndef NDEBUG
#include "imgui.h"
#include <entt/entt.hpp>

namespace FlowEngine {
    class DebugOverlay {
    public:
        // Your Input Manager will flip this boolean when '~' is pressed
        static inline bool bIsVisible = false;

        // Pass in the registry and a specific hardcoded Entity ID you want to track
        static void Draw(float currentFPS, entt::registry& registry, entt::entity trackedEntityID) {
            if (!bIsVisible) return;

            ImGuiIO& io = ImGui::GetIO();

            // Strip away all window features (No background dragging, no title bar, no resizing)
            ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav |
                ImGuiWindowFlags_NoMove;

            // Math to lock it to the BOTTOM-LEFT corner of the screen
            ImVec2 window_pos = ImVec2(10.0f, io.DisplaySize.y - 10.0f);
            ImVec2 window_pos_pivot = ImVec2(0.0f, 1.0f); // Pivot at the bottom-left of the ImGui window
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowBgAlpha(0.35f); // Transparent dark background

            if (ImGui::Begin("BottomOverlay", &bIsVisible, window_flags)) {

                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "FLOW ENGINE DEBUG (~) ");
                ImGui::Separator();
                ImGui::Text("FPS: %.1f", currentFPS);
                ImGui::Separator();

                // BRUTE FORCE TRACKING: Just check if the hardcoded ID exists
                if (registry.valid(trackedEntityID)) {
                    // Try to get its transform (Assuming you have a Transform component)
                    if (auto* transform = registry.try_get<Transform>(trackedEntityID)) {
                        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Tracking Entity: %d", (uint32_t)trackedEntityID);
                        ImGui::Text("PosX: %.2f | PosY: %.2f", transform->x, transform->y);
                    }
                }
                else {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Tracked Entity %d is DEAD or INVALID", (uint32_t)trackedEntityID);
                }
            }
            ImGui::End();
        }
    };
}
#endif