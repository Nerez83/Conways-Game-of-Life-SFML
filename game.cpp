#include "game.hpp"
#include <cstdlib>
#include "rand_bool.hpp"

Game::Game()
    : VGrid(sf::VertexArray(sf::PrimitiveType::Triangles, 0)), lGrid(LGrid()), width_cells(0), height_cells(0), cell_size(0), isplaying(false), gridLines(sf::PrimitiveType::Triangles, 0) {}

Game::Game(int init_width_cells, int init_height_cells, float init_cell_size)
    : VGrid(sf::VertexArray(sf::PrimitiveType::Triangles, init_width_cells * init_height_cells * 6)), lGrid(LGrid(init_width_cells, init_height_cells)), width_cells(init_width_cells), height_cells(init_height_cells), cell_size(init_cell_size), isplaying(false), gridLines(sf::PrimitiveType::Triangles, (init_width_cells + init_height_cells + 2) * 6)
{
    int idx = 0;
    for (int x = 0; x < init_width_cells; x++)
    {
        for (int y = 0; y < init_height_cells; y++)
        {
            idx = (x + y * init_width_cells) * 6;
            // first triangle
            VGrid[idx].position = {x * init_cell_size, y * init_cell_size};
            VGrid[idx + 1].position = {(x + 1) * init_cell_size, y * init_cell_size};
            VGrid[idx + 2].position = {(x + 1) * init_cell_size, (y + 1) * init_cell_size};

            // second triangle
            VGrid[idx + 3].position = {(x + 1) * init_cell_size, (y + 1) * init_cell_size};
            VGrid[idx + 4].position = {x * init_cell_size, (y + 1) * init_cell_size};
            VGrid[idx + 5].position = {x * init_cell_size, y * init_cell_size};

            setVGridCell({x, y}, false);
        }
    }

    for (int x = 0; x <= init_width_cells; x++)
    {

        gridLines[6 * x].position = {(x - grid_lines_width) * cell_size, -grid_lines_width * cell_size};
        gridLines[(6 * x) + 1].position = {(x + grid_lines_width) * cell_size, -grid_lines_width * cell_size};
        gridLines[(6 * x) + 2].position = {(x + grid_lines_width) * cell_size, (init_height_cells + grid_lines_width) * cell_size};

        gridLines[(6 * x) + 3].position = {(x + grid_lines_width) * cell_size, (init_height_cells + grid_lines_width) * cell_size};
        gridLines[(6 * x) + 4].position = {(x - grid_lines_width) * cell_size, (init_height_cells + grid_lines_width) * cell_size};
        gridLines[(6 * x) + 5].position = {(x - grid_lines_width) * cell_size, -grid_lines_width * cell_size};

        for (int i = 0; i < 6; i++)
        {
            gridLines[(6 * x) + i].color = grid_line_color;
        }
    }

    for (int y = 0; y <= init_height_cells; y++)
    {

        gridLines[6 * (init_width_cells + 1 + y)].position = {-grid_lines_width * cell_size, (y + grid_lines_width) * cell_size};
        gridLines[(6 * (init_width_cells + 1 + y)) + 1].position = {-grid_lines_width * cell_size, (y - grid_lines_width) * cell_size};
        gridLines[(6 * (init_width_cells + 1 + y)) + 2].position = {(init_width_cells + grid_lines_width) * cell_size, (y - grid_lines_width) * cell_size};

        gridLines[(6 * (init_width_cells + 1 + y)) + 3].position = {(init_width_cells + grid_lines_width) * cell_size, (y - grid_lines_width) * cell_size};
        gridLines[(6 * (init_width_cells + 1 + y)) + 4].position = {(init_width_cells + grid_lines_width) * cell_size, (y + grid_lines_width) * cell_size};
        gridLines[(6 * (init_width_cells + 1 + y)) + 5].position = {-grid_lines_width * cell_size, (y + grid_lines_width) * cell_size};

        for (int i = 0; i < 6; i++)
        {
            gridLines[(6 * (init_width_cells + 1 + y)) + i].color = grid_line_color;
        }
    }
}

Game::~Game() {}

void Game::setVGridCell(sf::Vector2i coor, bool alive)
{
    for (int i = 0; i < 6; i++)
        VGrid[(coor.x + coor.y * width_cells) * 6 + i].color = (alive ? sf::Color::White : sf::Color::Black);
}

void Game::clearGrid()
{
    lGrid.clearGrid();

    for (int x = 0; x < width_cells; x++)
    {
        for (int y = 0; y < height_cells; y++)
        {
            setVGridCell({x, y}, false);
        }
    }
}
void Game::setCell(sf::Vector2i coor, bool alive)
{
    lGrid.setCell(coor.x, coor.y, alive);
    setVGridCell(coor, alive);
}
void Game::nextFrame()
{
    lGrid.nextFrame();
    for (int x = 0; x < width_cells; x++)
    {
        for (int y = 0; y < height_cells; y++)
        {
            setVGridCell({x, y}, lGrid.isAlive(x, y));
        }
    }
}

void Game::randGrid()
{
    RandBool randBool = RandBool();
    bool curr_randBool;
    for (int x = 0; x < width_cells; x++)
    {
        for (int y = 0; y < height_cells; y++)
        {
            curr_randBool = randBool.next();
            lGrid.setCell(x, y, curr_randBool);
            setVGridCell({x, y}, curr_randBool);
        }
    }
}
sf::VertexArray &Game::getVGrid()
{
    return VGrid;
}

bool Game::getCell(sf::Vector2i cell)
{
    return lGrid.isAlive(cell.x, cell.y);
}
sf::VertexArray &Game::getGridLines()
{
    return gridLines;
}