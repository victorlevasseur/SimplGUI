#ifndef SIMPLGUI_RENDERER_H
#define SIMPLGUI_RENDERER_H

#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace simplgui
{

class Widget;

/**
 * Class providing tools to draw the most common shapes for widgets.
 */
class Renderer
{
public:
    /**
     * Draw a background rectangle.
     */
    static void DrawBackgroundRectangle(sf::RenderTarget &target, std::shared_ptr<const Widget> widget, sf::FloatRect rectangle);
    
    /**
     * Draw a selection rectangle.
     */
    static void DrawSelectionRectangle(sf::RenderTarget &target, std::shared_ptr<const Widget> widget, sf::FloatRect rectangle);
    
    /**
     * Draw a rectangle with custom colors, position and outline thickness.
     */
    static void DrawRectangle(
        sf::RenderTarget &target, 
        sf::FloatRect rectangle, 
        float outline,
        sf::Color fillColor,
        sf::Color outlineColor,
        sf::Transform transform = sf::Transform()
        );
    
private:
};

}

#endif

