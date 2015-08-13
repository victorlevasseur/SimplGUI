#include "simplgui/Widget.h"

namespace simplgui
{

Widget::Widget() : sf::Drawable(), sf::Transformable(), m_parent()
{

}

void Widget::processEvent(sf::Event event)
{
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

sf::Transform Widget::getGlobalTransform() const
{
    return (m_parent.expired() ? getTransform() : (m_parent.lock()->getGlobalTransform() * getTransform()));
}


}
