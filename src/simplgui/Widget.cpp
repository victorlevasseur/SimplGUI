#include "simplgui/Widget.h"

namespace simplgui
{

Widget::Widget() : 
    sf::Drawable(), 
    sf::Transformable(), 
    m_parent(), 
    m_size(AUTOSIZE, AUTOSIZE), 
    m_minSize(0.f, 0.f),
    m_focus(false)
{

}

void Widget::processEvent(sf::Event event)
{
    sf::Transform globalTr = getGlobalTransform();
    sf::FloatRect widgetRect(sf::Vector2f(0.f, 0.f), getEffectiveSize());
    
    widgetRect = globalTr.transformRect(widgetRect);
    
    if(event.type == sf::Event::MouseButtonPressed)
    {
        if(widgetRect.contains(event.mouseButton.x, event.mouseButton.y))
        {
            setFocused(true);
        }
        else //TODO: Allow widgets to go outside their effective size rectangle (like combobox with their unfoldable list)
        {
            setFocused(false);
        }
    }
    
    doProcessEvent(event);
}

void Widget::update(sf::Time dt)
{
    doUpdate(dt);
}

sf::Vector2f Widget::getSize() const
{
    return m_size;
}

void Widget::setSize(sf::Vector2f size)
{
    m_size = size;
}

sf::Vector2f Widget::getEffectiveSize() const
{
    sf::Vector2f autoSize = doCalculateAutoSize();
    
    return sf::Vector2f(
        std::max(m_size.x == AUTOSIZE ? autoSize.x : m_size.x, m_minSize.x),
        std::max(m_size.y == AUTOSIZE ? autoSize.y : m_size.y, m_minSize.y)
        );
}

sf::Vector2f Widget::getMinSize() const
{
    return m_minSize;
}

void Widget::setMinSize(sf::Vector2f minSize)
{
    m_minSize = minSize;
}

sf::Transform Widget::getGlobalTransform() const
{
    return (m_parent.expired() ? getTransform() : (m_parent.lock()->getGlobalTransform() * getTransform()));
}

sf::Vector2f Widget::doCalculateAutoSize() const
{
    return sf::Vector2f(0.f, 0.f);
}


}
