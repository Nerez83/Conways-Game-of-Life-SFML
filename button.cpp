#include "button.hpp"
inline sf::Color offsetColor(sf::Color color, int offset)
{
    
    float red_out = color.r + offset <= 0 ? 0 : (color.r + offset >= 255 ? 255 : color.r + offset);
    float green_out = color.g + offset <= 0 ? 0 : (color.g + offset >= 255 ? 255 : color.g + offset);
    float blue_out = color.b + offset <= 0 ? 0 : (color.b + offset >= 255 ? 255 : color.b + offset);
    return sf::Color(red_out,green_out,blue_out);
}

Button::Button(sf::Vector2f size, sf::Vector2f position, std::string text_str, sf::Text::Style style, sf::Color color)
{
    rectangle = sf::RectangleShape(size);
    rectangle.setOrigin(rectangle.getGeometricCenter());
    rectangle.setPosition(position);
    rectangle.setOutlineThickness(border_thickness);
    rectangle.setOutlineColor(border_color);

    font = sf::Font("C:/Windows/Fonts/arial.ttf");
    text.setFont(font);
    text.setString(text_str);
    text.setCharacterSize(static_cast<unsigned int>((rectangle.getSize().y) / 1.7));
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2, bounds.position.y + bounds.size.y / 2});
    text.setPosition(position);
    text.setFillColor(sf::Color::Black);
    text.setStyle(style);
    setColor(color);
}

Button::~Button()
{
}
sf::RectangleShape& Button::getRectangle()
{
    return rectangle;
}
sf::Text& Button::getText()
{
    return text;
}

void Button::setColor(sf::Color color)
{
    int outline_color_offset = -40;
    int hovered_color_offset = 20;
    int clicked_color_offset = -40;
    def_color = sf::Color(color.r, color.g, color.b);
    rectangle.setOutlineColor(offsetColor(color, outline_color_offset));
    hover_color = offsetColor(color, hovered_color_offset);
    clicked_color = offsetColor(color, clicked_color_offset);
    rectangle.setFillColor(def_color);
}
void Button::setPosition(sf::Vector2f position)
{
    rectangle.setPosition(position);
    text.setPosition(position);
}
void Button::clicked()
{
    isClicked = true;
    rectangle.setFillColor(clicked_color);
}
void Button::hovered()
{
    if (!isClicked)
    {
        rectangle.setFillColor(hover_color);
    }
}
void Button::released()
{
    isClicked = false;
    rectangle.setFillColor(def_color);
}
void Button::showDefColor()
{
    rectangle.setFillColor(def_color);
}

sf::FloatRect Button::getGlobalBounds()
{
    return rectangle.getGlobalBounds();
}
void Button::setBold(){
    text.setStyle(sf::Text::Bold);
}
