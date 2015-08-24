#ifndef SIMPLGUI_TEXTBOX_H
#define SIMPLGUI_TEXTBOX_H

#include <SFML/Graphics.hpp>
#include "simplgui/Widget.h"
#include "simplgui/Connector.h"

namespace simplgui
{

class TextBox : public Widget
{
public:
    using Ptr = std::shared_ptr<TextBox>; ///< Shared pointer to TextBox
    using ConstPtr = std::shared_ptr<const TextBox>; ///< Shared pointer to const TextBox

    static std::shared_ptr<TextBox> create();
    virtual ~TextBox() {};
    
    void setFont(const sf::Font &font);
    
    void setSelection(std::ptrdiff_t start, std::ptrdiff_t len = 0);
    
    std::u32string getText() const { return m_string; };
    void setText(const std::u32string &text);
    
    Connector<const std::u32string&> onTextChanged;

protected:
    TextBox();
    
    virtual void doProcessEvent(sf::Event event);
    virtual void doUpdate(sf::Time dt);
    
    virtual void onSizeUpdated();
    
    virtual sf::Vector2f doCalculateAutoSize() const;
    
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    void updateText();
    
    void ensureCharacterIsVisible(std::size_t pos);
    
    bool hasMultipleCharSelected() const;
    
    /**
     * Improved version of findCharacterPos of SFML so as it returns the full size of
     * the text if the index = m_string.size()
     */
    sf::Vector2f getCharacterPosition(std::ptrdiff_t index) const;
    
    std::ptrdiff_t getCharacterIndexAt(float x, float y) const;
    
    void eraseSelection();
 
    std::u32string m_string;
    
    sf::Text m_text;
    std::ptrdiff_t m_firstDisplayedCharIndex;
    std::ptrdiff_t m_lastDisplayedCharIndex;
    
    std::ptrdiff_t m_selectionStart;
    std::ptrdiff_t m_selectionLen;
};

}

#endif
