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