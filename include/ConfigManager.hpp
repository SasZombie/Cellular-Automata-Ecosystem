#pragma once
#include <string>
#include "../Extern/json.hpp"
namespace sas
{

    struct PlantConfig
    {
        size_t DaysAlive;
        size_t RangeSeeds;
        size_t WaterRange;
        size_t Size;
        size_t NumberOfSeeds;
        size_t WaterConsumption;
    };

    struct Config
    {
        PlantConfig Flower;
        PlantConfig Tree;
        PlantConfig Weed;
    };

    class ConfigManager
    {
    public:
        static Config &get();
        static void load(const std::string &path);

    private:
        static Config config;
    };

} // namespace sas