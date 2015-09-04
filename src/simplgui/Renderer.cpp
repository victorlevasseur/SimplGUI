#include "simplgui/Renderer.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include "simplgui/Widget.h"


namespace simplgui
{

void Renderer::DrawBackgroundRectangle(sf::RenderTarget &target, std::shared_ptr<const Widget> widget, sf::FloatRect rectangle)
{
    DrawRectangle(
        target,
        rectangle,
        widget->getTheme().getProperty<float>("border_thickness"),
        widget->isFocused() ? 
            widget->getTheme().getProperty<StateColor>("background_color").focused : 
            widget->getTheme().getProperty<StateColor>("background_color").normal,
        widget->isFocused() ? 
            widget->getTheme().getProperty<StateColor>("border_color").focused : 
            widget->getTheme().getProperty<StateColor>("border_color").normal,
        widget->getGlobalTransform()
    );
}

void Renderer::DrawSelectionRectangle(sf::RenderTarget &target, std::shared_ptr<const Widget> widget, sf::FloatRect rectangle)
{
    DrawRectangle(
        target,
        rectangle,
        widget->getTheme().getProperty<float>("selection_border_thickness"),
        widget->isFocused() ? 
            widget->getTheme().getProperty<StateColor>("selection_color").focused : 
            widget->getTheme().getProperty<StateColor>("selection_color").normal,
        widget->isFocused() ? 
            widget->getTheme().getProperty<StateColor>("selection_border_color").focused : 
            widget->getTheme().getProperty<StateColor>("selection_border_color").normal,
        widget->getGlobalTransform()
    );
}

void Renderer::DrawRectangle(
    sf::RenderTarget &target,
    sf::FloatRect rectangle, 
    float outline,
    sf::Color fillColor,
    sf::Color outlineColor,
    sf::Transform transform
    )
{
    sf::RectangleShape rect(sf::Vector2f(rectangle.width, rectangle.height));
    rect.setPosition(sf::Vector2f(rectangle.left, rectangle.top));
    rect.setFillColor(fillColor);
    rect.setOutlineColor(outlineColor);
    rect.setOutlineThickness(outline);
    
    target.draw(rect, sf::RenderStates(transform));
}

}

