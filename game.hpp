#pragma once
#include "logic_grid.hpp"
#include <SFML/Graphics.hpp>

class Game
{
private:
    sf::VertexArray VGrid;
    LGrid lGrid; 
    int width_cells;
    int height_cells;
    float cell_size;
    bool isplaying;
    sf::VertexArray gridLines;
    const float grid_lines_width = 0.05f; //percentage of cell_size  
    sf::Color alive_color = sf::Color::White;
    sf::Color dead_color = sf::Color::Black;
    sf::Color grid_line_color = sf::Color(100,100,100);
    
    void setVGridCell(sf::Vector2i, bool);
public:
    Game();
    Game(int , int, float );
    ~Game();
    void startGame(){isplaying = true;}
    void stopGame(){isplaying = false;}
    bool isPlaying(){return isplaying;}
    void clearGrid();
    void randGrid();
    void setCell(sf::Vector2i, bool);
    void nextFrame();
    sf::VertexArray& getVGrid();
    bool getCell(sf::Vector2i);
    sf::VertexArray& getGridLines();
};




