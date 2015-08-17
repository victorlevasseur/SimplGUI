#include "simplgui/TextBox.h"

namespace simplgui
{

std::shared_ptr<TextBox> TextBox::create()
{
    return std::shared_ptr<TextBox>(new TextBox());
}

TextBox::TextBox() : 
    Widget(), 
    onTextChanged(),
    m_string(), 
    m_text(),
    m_selectionBegin(0),
    m_selectionEnd(0)
{
    setSize(sf::Vector2f(150.f, 40.f));
    m_text.setColor(sf::Color(0, 0, 0));
    m_text.setPosition(3.f, 3.f);
}

void TextBox::setFont(const sf::Font &font)
{
    m_text.setFont(font);
}

void TextBox::setSelection(std::size_t cursor)
{
    setSelection(cursor, cursor);
}

void TextBox::setSelection(std::size_t begin, std::size_t end)
{
    m_selectionBegin = begin;
    m_selectionEnd = end;
}

void TextBox::doProcessEvent(sf::Event event)
{
    if(isFocused())
    {
        if(event.type == sf::Event::TextEntered)
        {
            sf::Uint32 character = event.text.unicode;
            if(character > 30 && (character < 127 || character > 159))
            {
                m_string.push_back(character);
            }
            else if(character == 8)
            {
                if(!m_string.empty())
                    m_string.pop_back();
            }
            
            needAutoSizeUpdate();
            updateText();
            
            onTextChanged.call(m_string);
        }
    }
}

void TextBox::doUpdate(sf::Time dt)
{

}

void TextBox::onSizeUpdated()
{
    updateText();
}

sf::Vector2f TextBox::doCalculateAutoSize() const
{
    //Calculate the height using a custom text (so as the height is always correct even if the text is empty)
    sf::Text testText("ablkj", *m_text.getFont(), m_text.getCharacterSize());
    
    return sf::Vector2f(
        m_text.getLocalBounds().left + m_text.getLocalBounds().width + 6.f, 
        testText.getLocalBounds().top + testText.getLocalBounds().height + 6.f
        );
}

void TextBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape bgShape(getEffectiveSize());
    bgShape.setFillColor(isFocused() ? sf::Color(255, 255, 255) : sf::Color(255, 255, 255, 180));
    
    target.draw(bgShape, getGlobalTransform());
    
    target.draw(m_text, getGlobalTransform());
}

void TextBox::updateText()
{
    sf::String textString("");
    m_text.setString("");
    
    for(auto it = m_string.begin(); 
        it != m_string.end() && (m_text.getLocalBounds().left + m_text.getLocalBounds().width < (getSize().x == Widget::AUTO_SIZE ? getMaxSize().x : getSize().x) - 6.f);
        ++it)
    {
        textString.insert(textString.getSize(), static_cast<sf::Uint32>(*it));
        m_text.setString(textString);
    }
    
    if(m_text.getLocalBounds().left + m_text.getLocalBounds().width >= (getSize().x == Widget::AUTO_SIZE ? getMaxSize().x : getSize().x) - 6.f)
        textString.erase(textString.getSize()-1);
    
    m_text.setString(textString);
}

}
