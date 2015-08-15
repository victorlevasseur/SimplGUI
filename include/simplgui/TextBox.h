#ifndef SIMPLGUI_TEXTBOX_H
#define SIMPLGUI_TEXTBOX_H

#include <SFML/Graphics.hpp>
#include "simplgui/Widget.h"

namespace simplgui
{

class TextBox : public Widget
{
public:
    using Ptr = std::shared_ptr<TextBox>;

    static std::shared_ptr<TextBox> create();
    virtual ~TextBox() {};
    
    void setFont(const sf::Font &font);
    
    void setSelection(std::size_t cursor);
    void setSelection(std::size_t begin, std::size_t end);

protected:
    TextBox();
    
    virtual void doProcessEvent(sf::Event event);
    virtual void doUpdate(sf::Time dt);
    
    virtual sf::Vector2f doCalculateAutoSize() const;
    
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    void updateText();

    std::u32string m_string;
    sf::Text m_text;
    
    std::size_t m_selectionBegin;
    std::size_t m_selectionEnd;
};

}

#endif
