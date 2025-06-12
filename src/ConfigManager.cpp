#include "../include/ConfigManager.hpp"
#include <fstream>

namespace sas
{
    using json = nlohmann::json;

    Config ConfigManager::config;

    static void from_json(const nlohmann::json &j, PlantConfig &p)
    {
        j.at("DaysAlive").get_to(p.DaysAlive);
        j.at("RangeSeeds").get_to(p.RangeSeeds);
        j.at("WaterRange").get_to(p.WaterRange);
        j.at("Size").get_to(p.Size);
        j.at("NumberOfSeeds").get_to(p.NumberOfSeeds);
        j.at("WaterConsumption").get_to(p.WaterConsumption);
    }

    void ConfigManager::load(const std::string &path)
    {
        std::ifstream file(path);
        json j;
        file >> j;

        config.Flower = j["Flower"].get<PlantConfig>();
        config.Tree = j["Tree"].get<PlantConfig>();
        config.Weed = j["Weed"].get<PlantConfig>();
    }

    Config &ConfigManager::get()
    {
        return config;
    }

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlantConfig, DaysAlive, RangeSeeds, WaterRange, Size, NumberOfSeeds, WaterConsumption)

} // namespace sas