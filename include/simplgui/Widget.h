#ifndef SIMPLGUI_WIDGET_H
#define SIMPLGUI_WIDGET_H

#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

namespace simplgui
{

class Widget : public sf::Drawable, public sf::Transformable
{
public:
    using Ptr = std::shared_ptr<Widget>;
    
    static constexpr float AUTOSIZE = -1.f;

    Widget(const Widget &) = delete;
    Widget& operator=(const Widget &) = delete;
    virtual ~Widget() {};
    
    void processEvent(sf::Event event);
    void update(sf::Time dt);
    
    sf::Vector2f getSize() const;
    void setSize(sf::Vector2f size);
    
    sf::Vector2f getEffectiveSize() const;
    
    sf::Vector2f getMinSize() const;
    void setMinSize(sf::Vector2f minSize);
    
    sf::Transform getGlobalTransform() const;
    
protected:
    Widget();
    
    virtual void doProcessEvent(sf::Event event) = 0;
    virtual void doUpdate(sf::Time dt) = 0;
    
    virtual sf::Vector2f doCalculateAutoSize() const;

private:
    std::weak_ptr<Widget> m_parent;
    sf::Vector2f m_size;
    sf::Vector2f m_minSize;
};

}

#endif
