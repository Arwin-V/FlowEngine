#pragma once
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include "ECS/Components.h"
#include "Utils/Log.h"

namespace Flow
{
    class LevelManager
    {
    public:
        static bool LoadLevel(const std::string& FilePath, entt::registry& Registry)
        {
            std::ifstream File(FilePath);
            if (!File.is_open())
            {
                FLOW_LOG_ERROR("LevelManager: Failed to open map file -> " + FilePath);
                return false;
            }

            nlohmann::json MapData;
            try
            {
                File >> MapData;
            }
            catch (nlohmann::json::parse_error& Error)
            {
                FLOW_LOG_ERROR(std::string("LevelManager JSON Parse Error: ") + Error.what());
                return false;
            }

            // Extract core variables (Unreal Naming Convention)
            int MapWidth = MapData["MapWidth"];
            int MapHeight = MapData["MapHeight"];
            float TileSize = MapData["TileSize"];

            int Index = 0;

            // Loop through the 1D Array
            for (int TileID : MapData["TileData"])
            {
                if (TileID > 0)
                {
                    // The Magic Math: 1D Index to 2D Coordinates
                    int GridX = Index % MapWidth;
                    int GridY = Index / MapWidth;

                    float WorldX = GridX * TileSize;
                    float WorldY = GridY * TileSize;

                    // Spawn the Tile Entity
                    auto TileEntity = Registry.create();

                    // Add Transform
                    Registry.emplace<Transform>(TileEntity, WorldX, WorldY);

                    // Add Collider (We subtract a tiny amount to prevent getting stuck on seams)
                    auto& Box = Registry.emplace<BoxCollider>(TileEntity, TileSize, TileSize);
                    Box.objectType = CollisionChannel::WorldStatic;

                    // (Later, we will add a SpriteComponent here to render actual dirt/stone textures!)
                }

                Index++;
            }

            FLOW_LOG_INFO("LevelManager: Successfully loaded level -> " + FilePath);
            return true;
        }
    };
}