#include <cctype>

constexpr std::size_t ScreenWidth = 800;
constexpr std::size_t ScreenHeight = 800;
//Multiplication of the said values
constexpr std::size_t ScreenWidthHeight = ScreenWidth * ScreenHeight; 
constexpr std::size_t cellSize = 40;
constexpr std::size_t WidthCells = ScreenWidth / cellSize, HeightCells = ScreenHeight / cellSize;
constexpr std::size_t WidthHeightCells = WidthCells * HeightCells;