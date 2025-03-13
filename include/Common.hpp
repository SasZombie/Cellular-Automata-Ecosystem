#include <cctype>

constexpr std::size_t ScreenWidth = 800;
constexpr std::size_t ScreenHeight = 800;
//Multiplication of the said values
constexpr std::size_t ScreenWidthHeight = ScreenWidth * ScreenHeight; 
constexpr std::size_t cellSize = 100;
constexpr std::size_t WidthCells = ScreenWidth / cellSize, HeightCells = ScreenHeight / cellSize;
constexpr std::size_t WidthHeightCells = WidthCells * HeightCells;

constexpr std::size_t FlowerDaysAlive = 5;
constexpr std::size_t TreeDaysAlive   = 10;
constexpr std::size_t WeedDaysAlive   = 3;

constexpr std::size_t FlowerRangeSeeds = 100;
constexpr std::size_t FlowerWaterRange = 10;
constexpr std::size_t FlowerSize = 20;
constexpr std::size_t FlowerNumberOfSeeds = 3;

constexpr std::size_t TreeRangeSeeds = 500;
constexpr std::size_t TreeWaterRange = 40;
constexpr std::size_t TreeSize = 50;
constexpr std::size_t TreeNumberOfSeeds = 1;

constexpr std::size_t WeedRangeSeeds = 20;
constexpr std::size_t WeedWaterRange = 15;
constexpr std::size_t WeedSize = 10;
constexpr std::size_t WeedNumberOfSeeds = 15;