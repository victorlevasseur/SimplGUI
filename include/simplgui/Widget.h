#ifndef SIMPLGUI_WIDGET_H
#define SIMPLGUI_WIDGET_H

#include <limits>
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
    
    static constexpr float AUTO_SIZE = -1.f;
    static constexpr float NO_MIN_SIZE = 0.f;
    static constexpr float NO_MAX_SIZE = std::numeric_limits<float>::max();

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
    
    sf::Vector2f getMaxSize() const;
    void setMaxSize(sf::Vector2f maxSize);
    
    sf::Transform getGlobalTransform() const;
    
    bool isFocused() const { return m_focus; }
    void setFocused(bool focus) { m_focus = focus; }
    
protected:
    Widget();
    
    virtual void doProcessEvent(sf::Event event) = 0;
    virtual void doUpdate(sf::Time dt) = 0;
    
    virtual void onSizeUpdated() = 0;
    
    sf::Vector2f calculateAutoSize() const;
    virtual sf::Vector2f doCalculateAutoSize() const = 0;
    void needAutoSizeUpdate() const;

private:
    std::weak_ptr<Widget> m_parent;
    sf::Vector2f m_size;
    sf::Vector2f m_minSize;
    sf::Vector2f m_maxSize;
    bool m_focus;
    
    mutable sf::Vector2f m_tmp_autoSize;
    mutable bool m_tmp_autoSizeNeedUpdate;
};

}

#endif
