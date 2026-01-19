#include <iostream>
#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "line.hpp"
#include "game.hpp"
#include <vector>

const unsigned int WINDOW_WIDTH = 800u;
const unsigned int WINDOW_HEIGHT = 600u;

float move_per_frame = 1.0f;
float scale_per_frame = 1.02f;

const int init_cell_size = 2;
int grid_width = 20;
int grid_height = 20;
float cell_size = init_cell_size;
bool handleBtnEvents(Button &btn, std::optional<sf::Event> event, sf::Vector2i mouse_pos)
{
    if (btn.getGlobalBounds().contains(sf::Vector2f({float(mouse_pos.x), float(mouse_pos.y)})))
    {
        btn.hovered();

        if (auto *mouse = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                btn.clicked();
            }
        }
        else if (auto *mouse = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                btn.released();
                return true;
                // std::cout << "Game started!" << std::endl;
            }
        }
    }
    else
    {
        btn.showDefColor();
    }
    return false;
}

int main()
{

    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Game of Life");
    window.setFramerateLimit(60);
    sf::Vector2f win_size = sf::Vector2f(window.getSize());
    sf::Color background_color = sf::Color(70, 70, 70);

    std::vector<sf::Drawable *> rendered_objects;

    sf::Font font("C:/Windows/Fonts/arial.ttf");

    sf::Vector2i mouse_pos;
    bool mouse_in_window = false;
    bool space_was_pressed = false;

    // Grid:

    sf::Vector2i cell_clicked;
    sf::Vector2f seen_center;

    sf::Transform grid_transform;
    sf::Transform grid_lines_transform;
    float curr_scale = 1;

    bool is_grid_lines = true;

    Game GoL = Game(grid_width, grid_height, cell_size);

    // Panel:
    struct
    {
        const float pnl_width = 400.f;
        const float btn_width = 150.f;
        const float btn_height = 50.f;
        const float middle_space = 20.f;
        const float margin = 40.f;
        const float rcol_offset = margin + (btn_width / 2);
        const float lcol_offset = pnl_width - rcol_offset;
        const float row_height = 80.f;
        const float separator_width = 2.f;

        Line separator = Line({WINDOW_WIDTH - pnl_width, 0.f}, {WINDOW_WIDTH - pnl_width, WINDOW_HEIGHT}, separator_width, sf::Color::Black);

        Button start_button = Button({btn_width, btn_height}, {WINDOW_WIDTH - lcol_offset, row_height}, "Start", sf::Text::Style::Bold, sf::Color(70, 210, 70));
        Button stop_button = Button({btn_width, btn_height}, {WINDOW_WIDTH - rcol_offset, row_height}, "Stop", sf::Text::Style::Bold, sf::Color(210, 70, 70));
        Button clear_button = Button({btn_width, btn_height}, {WINDOW_WIDTH - lcol_offset, 2 * row_height}, "Clear", sf::Text::Style::Bold, sf::Color(120, 120, 120));
        Button rand_button = Button({btn_width, btn_height}, {WINDOW_WIDTH - rcol_offset, 2 * row_height}, "Rand", sf::Text::Style::Bold, sf::Color(120, 120, 120));
        Button toggle_glines_button = Button({2 * btn_width, btn_height}, {WINDOW_WIDTH - (pnl_width / 2), 3 * row_height}, "Toggle grid", sf::Text::Style::Bold, sf::Color(120, 120, 120));

        sf::RectangleShape background = sf::RectangleShape({pnl_width, WINDOW_HEIGHT});
    } Panel;

    // init background:
    Panel.background.setFillColor(sf::Color(50, 50, 50));
    Panel.background.setPosition({win_size.x - Panel.pnl_width, 0.f});

    // center grid
    seen_center = {float((grid_width * cell_size) / 2), float((grid_height * cell_size) / 2)};
    grid_transform.translate({(win_size.x - Panel.pnl_width - (grid_width * cell_size)) / 2, (win_size.y - (grid_height * cell_size)) / 2});
    grid_lines_transform.translate({(win_size.x - Panel.pnl_width - (grid_width * cell_size)) / 2, (win_size.y - (grid_height * cell_size)) / 2});

    // fit grid to scale
    float def_scale;
    float scale_to_fit_x = (win_size.x - Panel.pnl_width) / (cell_size * grid_width);
    float scale_to_fit_y = win_size.y / (cell_size * grid_height);

    scale_to_fit_x < scale_to_fit_y ? def_scale = scale_to_fit_x : def_scale = scale_to_fit_y;

    grid_transform.scale({def_scale, def_scale}, seen_center);
    grid_lines_transform.scale({def_scale, def_scale}, seen_center);
    cell_size *= def_scale;
    curr_scale *= def_scale;

    //sf::RectangleShape test({5, 5});
    //test.setFillColor(sf::Color::Red);
    //test.setPosition(seen_center);

   

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            mouse_pos = sf::Mouse::getPosition(window);
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (event->is<sf::Event::Resized>())
            {
                sf::FloatRect global_bounds = grid_transform.transformRect(GoL.getVGrid().getBounds());

                win_size = sf::Vector2f(window.getSize());

                seen_center.x = (((win_size.x - Panel.pnl_width) / 2) - (global_bounds.position.x)) / curr_scale;
                seen_center.y = ((win_size.y / 2) - global_bounds.position.y) / curr_scale;
                /*
                if (seen_center.x > grid_width * init_cell_size)
                {
                    seen_center.x = grid_width * init_cell_size;
                    grid_transform.translate({(win_size.x - Panel.pnl_width) / 2 - global_bounds.position.x - (grid_width * init_cell_size * curr_scale), 0});
                    grid_lines_transform.translate({(win_size.x - Panel.pnl_width) / 2 - global_bounds.position.x - (grid_width * init_cell_size * curr_scale), 0});
                }
                if (seen_center.x < 0)
                {
                    seen_center.x = 0;
                    grid_transform.translate({(win_size.x - Panel.pnl_width) / 2 - global_bounds.position.x, 0});
                    grid_lines_transform.translate({(win_size.x - Panel.pnl_width) / 2 - global_bounds.position.x, 0});
                }
                if (seen_center.y > grid_height * init_cell_size)
                {
                    seen_center.y = grid_height * init_cell_size;
                    grid_transform.translate({0, win_size.y / 2 - global_bounds.position.y - (grid_height * init_cell_size * curr_scale)});
                    grid_lines_transform.translate({0, win_size.y / 2 - global_bounds.position.y - (grid_height * init_cell_size * curr_scale)});
                }
                if (seen_center.y < 0)
                {
                    seen_center.y = 0;
                    
                    grid_transform.translate({0, win_size.y / 2 - global_bounds.position.y});
                    grid_lines_transform.translate({0, win_size.y / 2 - global_bounds.position.y });
                }

                */

                if (seen_center.x > grid_width * init_cell_size)
                {
                    seen_center.x = grid_width * init_cell_size;
                }
                if (seen_center.x < 0)
                {
                    seen_center.x = 0;
                }
                if (seen_center.y > grid_height * init_cell_size)
                {
                    seen_center.y = grid_height * init_cell_size;
                }
                if (seen_center.y < 0)
                {
                    seen_center.y = 0;
                }
                sf::View view(sf::FloatRect({0.f, 0.f}, win_size));
                window.setView(view);

                Panel.background.setSize({Panel.pnl_width, win_size.y});
                Panel.background.setPosition({window.getSize().x - Panel.pnl_width, 0.f});
                Panel.separator.resize({win_size.x - Panel.pnl_width, 0.f}, {win_size.x - Panel.pnl_width, win_size.y});
                Panel.start_button.setPosition({win_size.x - Panel.lcol_offset, Panel.row_height});
                Panel.stop_button.setPosition({win_size.x - Panel.rcol_offset, Panel.row_height});
                Panel.clear_button.setPosition({win_size.x - Panel.lcol_offset, 2 * Panel.row_height});
                Panel.rand_button.setPosition({win_size.x - Panel.rcol_offset, 2 * Panel.row_height});
                Panel.toggle_glines_button.setPosition({win_size.x - (Panel.pnl_width / 2), 3 * Panel.row_height});
            }
            /*
            else if (auto *mouse = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                mouse->delta;
            }
            */
            else if (event->is<sf::Event::MouseEntered>())
            {
                mouse_in_window = true;
            }
            else if (event->is<sf::Event::MouseLeft>())
            {
                mouse_in_window = false;
            }

            // Mouse click events

            bool mouse_on_panel;

            if (mouse_in_window)
                mouse_on_panel = Panel.background.getGlobalBounds().contains(sf::Vector2f({float(mouse_pos.x), float(mouse_pos.y)}));

            if (mouse_in_window && mouse_on_panel)
            {
                if (handleBtnEvents(Panel.start_button, event, mouse_pos))
                    GoL.startGame();
                if (handleBtnEvents(Panel.stop_button, event, mouse_pos))
                    GoL.stopGame();
                if (handleBtnEvents(Panel.clear_button, event, mouse_pos))
                    GoL.clearGrid();
                if (handleBtnEvents(Panel.rand_button, event, mouse_pos))
                    GoL.randGrid();
                if (handleBtnEvents(Panel.toggle_glines_button, event, mouse_pos))
                    is_grid_lines = !is_grid_lines;
            }
            if (mouse_in_window && !mouse_on_panel)
            {
                if (auto *mouse = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mouse->button == sf::Mouse::Button::Left)
                    {
                        sf::FloatRect global_bounds = grid_transform.transformRect(GoL.getVGrid().getBounds());
                        cell_clicked = {int((float(mouse_pos.x) - (global_bounds.position.x)) / cell_size), int((float(mouse_pos.y) - (global_bounds.position.y)) / cell_size)};
                    }
                }
                else if (auto *mouse = event->getIf<sf::Event::MouseButtonReleased>())
                {
                    if (mouse->button == sf::Mouse::Button::Left)
                    {
                        if (cell_clicked.x != -1)
                        {
                            GoL.setCell(cell_clicked, !GoL.getCell(cell_clicked));
                            cell_clicked.x = -1;
                        }
                    }
                }
            }
        }
        // End of event handling

        // Start/Stop game via spacebar
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            if (!space_was_pressed)
            {
                space_was_pressed = true;
                GoL.isPlaying() ? GoL.stopGame() : GoL.startGame();
            }
        }
        else
        {
            space_was_pressed = false;
        }

        sf::FloatRect global_bounds = grid_transform.transformRect(GoL.getVGrid().getBounds());

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            grid_transform.translate({-move_per_frame, 0});
            grid_lines_transform.translate({-move_per_frame, 0});
            seen_center.x = (((win_size.x - Panel.pnl_width) / 2) - (global_bounds.position.x)) / curr_scale;
            if (seen_center.x > grid_width * init_cell_size)
            {
                seen_center.x = grid_width * init_cell_size;
                grid_transform.translate({move_per_frame, 0});
                grid_lines_transform.translate({move_per_frame, 0});
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            grid_transform.translate({move_per_frame, 0});
            grid_lines_transform.translate({move_per_frame, 0});
            seen_center.x = (((win_size.x - Panel.pnl_width) / 2) - (global_bounds.position.x)) / curr_scale;

            if (seen_center.x < 0)
            {
                seen_center.x = 0;
                grid_transform.translate({-move_per_frame, 0});
                grid_lines_transform.translate({-move_per_frame, 0});
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
            {
                grid_transform.scale({1 / scale_per_frame, 1 / scale_per_frame}, seen_center);
                grid_lines_transform.scale({1 / scale_per_frame, 1 / scale_per_frame}, seen_center);
                cell_size = cell_size / scale_per_frame;
                curr_scale = curr_scale / scale_per_frame;
            }
            else
            {
                grid_transform.translate({0, -move_per_frame});
                grid_lines_transform.translate({0, -move_per_frame});
                seen_center.y = ((win_size.y / 2) - global_bounds.position.y) / curr_scale;
                if (seen_center.y > grid_height * init_cell_size)
                {
                    seen_center.y = grid_height * init_cell_size;
                    grid_transform.translate({0, move_per_frame});
                    grid_lines_transform.translate({0, move_per_frame});
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
            {
                grid_transform.scale({scale_per_frame, scale_per_frame}, seen_center);
                grid_lines_transform.scale({scale_per_frame, scale_per_frame}, seen_center);
                cell_size *= scale_per_frame;
                curr_scale *= scale_per_frame;
            }
            else
            {
                grid_transform.translate({0, move_per_frame});
                grid_lines_transform.translate({0, move_per_frame});
                seen_center.y = ((win_size.y / 2) - global_bounds.position.y) / curr_scale;
                if (seen_center.y < 0)
                {
                    seen_center.y = 0;
                    grid_transform.translate({0, -move_per_frame});
                    grid_lines_transform.translate({0, -move_per_frame});
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
            {
                scale_per_frame += 0.005f;
            }
            else
            {
                move_per_frame += 0.1f;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
            {
                scale_per_frame -= (scale_per_frame <= 0 ? 0 : 0.005f);
            }
            else
            {
                move_per_frame -= (move_per_frame <= 0 ? 0 : 0.1f);
            }
        }

        if (GoL.isPlaying())
        {
            GoL.nextFrame();
        }

        // render all
        window.clear(background_color);

        window.draw(GoL.getVGrid(), grid_transform);
        if (is_grid_lines)
        {
            window.draw(GoL.getGridLines(), grid_lines_transform);
        }

        window.draw(Panel.background);
        window.draw(Panel.separator.getLine());

        window.draw(Panel.start_button.getRectangle());
        window.draw(Panel.start_button.getText());
        window.draw(Panel.stop_button.getRectangle());
        window.draw(Panel.stop_button.getText());
        window.draw(Panel.clear_button.getRectangle());
        window.draw(Panel.clear_button.getText());
        window.draw(Panel.rand_button.getRectangle());
        window.draw(Panel.rand_button.getText());
        window.draw(Panel.toggle_glines_button.getRectangle());
        window.draw(Panel.toggle_glines_button.getText());

       // test.setPosition({global_bounds.position.x + (seen_center.x * curr_scale), global_bounds.position.y + (seen_center.y * curr_scale)});
        //window.draw(test);

        window.display();

        // std::cout << scale_to_fit_x  << ", " << scale_to_fit_y << ": " << def_scale << std::endl;
    }
    return 0;
}