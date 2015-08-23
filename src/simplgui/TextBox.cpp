#include "simplgui/TextBox.h"

#include <iostream>

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
    m_firstDisplayedCharIndex(0),
    m_lastDisplayedCharIndex(0),
    m_selectionStart(0),
    m_selectionLen(0)
{
    setSize(sf::Vector2f(150.f, 40.f));
    m_text.setColor(sf::Color(0, 0, 0));
    m_text.setPosition(3.f, 3.f);
}

void TextBox::setFont(const sf::Font &font)
{
    m_text.setFont(font);
}

void TextBox::setSelection(std::ptrdiff_t start, std::ptrdiff_t len)
{
    m_selectionStart = start;
    m_selectionLen = len;
    
    if(m_selectionStart > m_lastDisplayedCharIndex && m_selectionStart < m_string.size() )
        ensureCharacterIsVisible(m_selectionStart);
    else if(m_selectionStart < m_firstDisplayedCharIndex)
        ensureCharacterIsVisible(m_selectionStart);
    else if(m_selectionStart == m_string.size())
        ensureCharacterIsVisible(m_selectionStart-1);
    
    removeBlankSpace();
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
                m_string.replace(
                    std::min(m_selectionStart, m_selectionStart + m_selectionLen), 
                    std::max(m_selectionLen, -m_selectionLen), 
                    std::u32string(1, static_cast<char32_t>(character))
                    );
                
                setSelection(std::min(m_selectionStart + 1, m_selectionStart + m_selectionLen + 1));
            }
            else if(character == 8)
            {
                if(!m_string.empty())
                {
                    if(!hasMultipleCharSelected() && m_selectionStart > 0)
                    {
                        m_string.replace(m_selectionStart-1, 1, U"");
                        setSelection(m_selectionStart-1);
                    }
                    else if(hasMultipleCharSelected())
                    {
                        m_string.replace(
                            std::min(m_selectionStart, m_selectionStart + m_selectionLen), 
                            std::max(m_selectionLen, -m_selectionLen), 
                            U""
                            );
                        
                        setSelection(m_selectionStart + std::min(std::ptrdiff_t(0), m_selectionLen));
                    }
                }
            }
            
            needAutoSizeUpdate();
            updateText();
            
            onTextChanged.call(m_string);
            
            if(!m_string.empty())
                ensureCharacterIsVisible(m_string.size()-1);
            else
                ensureCharacterIsVisible(0);
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Left)
            {
                if(event.key.shift)
                {
                    if(m_selectionStart + m_selectionLen > 0)
                        setSelection(m_selectionStart, m_selectionLen-1);
                }
                else
                {
                    if(m_selectionStart > 0)
                    {
                        if(!hasMultipleCharSelected())
                            setSelection(m_selectionStart - 1);
                        else
                            setSelection(std::min(m_selectionStart, m_selectionStart + m_selectionLen));
                    }
                }
            }
            else if(event.key.code == sf::Keyboard::Right)
            {
                if(event.key.shift)
                {
                    if(m_selectionStart + m_selectionLen < m_string.size())
                        setSelection(m_selectionStart, m_selectionLen+1);
                }
                else
                {
                    if(m_selectionStart < m_string.size())
                    {
                        if(!hasMultipleCharSelected())
                            setSelection(m_selectionStart + m_selectionLen + 1);
                        else
                            setSelection(std::max(m_selectionStart, m_selectionStart + m_selectionLen));
                    }
                }
            }
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
    
    sf::RectangleShape cursorShape(
        sf::Vector2f(
            !hasMultipleCharSelected() ? 2 : (m_text.findCharacterPos(std::min(m_lastDisplayedCharIndex - m_firstDisplayedCharIndex, m_selectionLen + m_selectionStart - m_firstDisplayedCharIndex)).x - m_text.findCharacterPos(std::max(std::ptrdiff_t(0), m_selectionStart - m_firstDisplayedCharIndex)).x), 
            m_text.getLocalBounds().top + m_text.getLocalBounds().height
            )
        );
    cursorShape.setPosition(sf::Vector2f(
        m_text.findCharacterPos(std::max(std::ptrdiff_t(0), m_selectionStart - m_firstDisplayedCharIndex)).x, 
        m_text.getLocalBounds().top
        ));
    cursorShape.setFillColor(isFocused() ? (!hasMultipleCharSelected() ? sf::Color(0, 0, 0, 255) : sf::Color(128, 128, 255, 255)) : sf::Color(0, 0, 0, 0));
    
    target.draw(bgShape, getGlobalTransform());
    target.draw(cursorShape, getGlobalTransform());
    target.draw(m_text, getGlobalTransform());
}

void TextBox::updateText()
{
    sf::String textString("");
    m_text.setString("");

    for(auto it = m_string.begin() + m_firstDisplayedCharIndex; 
        it != m_string.end() && (m_text.getLocalBounds().left + m_text.getLocalBounds().width < (getSize().x == Widget::AUTO_SIZE ? getMaxSize().x : getSize().x) - 6.f);
        ++it)
    {
        textString.insert(textString.getSize(), static_cast<sf::Uint32>(*it));
        m_text.setString(textString);
    }
    
    if(m_text.getLocalBounds().left + m_text.getLocalBounds().width >= (getSize().x == Widget::AUTO_SIZE ? getMaxSize().x : getSize().x) - 6.f)
        textString.erase(textString.getSize()-1);
        
    m_lastDisplayedCharIndex = textString.getSize() > 0 ? m_firstDisplayedCharIndex + textString.getSize()-1 : 0;
    
    m_text.setString(textString);
}

void TextBox::ensureCharacterIsVisible(std::size_t pos)
{
    updateText();

    while(pos < m_firstDisplayedCharIndex)
    {
        --m_firstDisplayedCharIndex;
        needAutoSizeUpdate();
        updateText();
    }
    while(pos > m_lastDisplayedCharIndex)
    {
        ++m_firstDisplayedCharIndex;
        needAutoSizeUpdate();
        updateText();
    }
    
    removeBlankSpace();
}

void TextBox::removeBlankSpace()
{
    /*if(m_string.empty())
        return;

    while(m_lastDisplayedCharIndex == m_string.size()-1 && m_firstDisplayedCharIndex > 0)
    {
        std::cout << "Moving back ! "<<std::endl;
        --m_firstDisplayedCharIndex;
        updateText();
    }
    if(m_lastDisplayedCharIndex < m_string.size()-1)
        ++m_firstDisplayedCharIndex;

    updateText();*/
}

bool TextBox::hasMultipleCharSelected() const
{
    return (m_selectionLen != 0);
}

}

