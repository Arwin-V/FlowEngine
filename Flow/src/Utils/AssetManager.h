#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include "Utils/Log.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace Flow {
    class AssetManager {
    public:

        bool BuildAnimatorFromJson(const std::string& jsonPath, AnimatorComponent& animComp) {
            std::ifstream file(jsonPath);
            if (!file.is_open()) {
                FLOW_LOG_ERROR("Failed to open JSON manifest: " + jsonPath);
                return false;
            }

            nlohmann::json data;
            try {
                file >> data;
            }
            catch (nlohmann::json::parse_error& e) {
                FLOW_LOG_ERROR(std::string("JSON Parse Error: ") + e.what());
                return false;
            }

            for (auto& [stateName, stateData] : data["states"].items()) {
                std::string texPath = stateData["texture_path"];

                // Ensure texture is loaded in memory
                LoadTexture(texPath, texPath);

                Animation anim;
                anim.texture = GetTexture(texPath);
                anim.frameTime = stateData["frame_time"];

                int w = stateData["frame_width"];
                int h = stateData["frame_height"];
                int count = stateData["frame_count"];

                for (int i = 0; i < count; ++i) {
                    anim.frames.push_back(sf::IntRect({ i * w, 0 }, { w, h }));
                }

                animComp.animations[stateName] = anim;
            }

            // Default to the first state found
            animComp.currentState = data["states"].begin().key();
            return true;
        }

        // Delete copy/move constructors (Singleton pattern)
        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;

        static AssetManager& Get() {
            static AssetManager instance;
            return instance;
        }

        void LoadTexture(const std::string& name, const std::string& filepath) {
            // Only load if it doesn't already exist
            if (Textures.find(name) == Textures.end()) {
                auto texture = std::make_shared<sf::Texture>();
                if (texture->loadFromFile(filepath)) {
                    Textures[name] = texture;
                    FLOW_LOG_INFO("AssetManager: Loaded texture -> " + name);
                }
                else {
                    FLOW_LOG_ERROR("AssetManager: Failed to load texture -> " + filepath);
                }
            }
        }

        std::shared_ptr<sf::Texture> GetTexture(const std::string& name) {
            auto it = Textures.find(name);
            if (it != Textures.end()) {
                return it->second;
            }
            FLOW_LOG_WARN("AssetManager: Texture not found -> " + name);
            return nullptr;
        }

    private:
        AssetManager() = default;
        ~AssetManager() = default;

        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> Textures;
    };
}