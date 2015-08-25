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
    m_selectionLen(0),
    m_leftClicking(false)
{
    setSize(sf::Vector2f(150.f, 40.f));
    m_text.setColor(sf::Color(0, 0, 0));
    m_text.setPosition(3.f, 3.f);
}

void TextBox::setFont(const sf::Font &font)
{
    m_text.setFont(font);
    
    updateText();
    needAutoSizeUpdate();
}

int TextBox::getSelectionStart() const
{
    return m_selectionStart;
}

int TextBox::getSelectionLen() const
{
    return m_selectionLen;
}

void TextBox::setSelection(int start, int len)
{
    m_selectionStart = start;
    m_selectionLen = len;
    
    if(m_selectionStart + m_selectionLen == m_string.size() && !m_string.empty())
        ensureCharacterIsVisible(m_selectionStart + m_selectionLen - 1);
    else
        ensureCharacterIsVisible(m_selectionStart + m_selectionLen);
}

void TextBox::setText(const std::u32string &text)
{
    m_string = text;
    
    updateText();
    setSelection(0);
    needAutoSizeUpdate();
}

void TextBox::doProcessEvent(sf::Event event)
{
    if(isFocused())
    {
        //Get the bounding box of the widget
        sf::Transform globalTr = getGlobalTransform();
        sf::FloatRect widgetRect(sf::Vector2f(0.f, 0.f), getEffectiveSize());
        widgetRect = globalTr.transformRect(widgetRect);
        
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                if(widgetRect.contains(event.mouseButton.x, event.mouseButton.y))
                {
                    sf::Vector2f relativeMousePosition(globalTr.getInverse().transformPoint(event.mouseButton.x, event.mouseButton.y));   
                    setSelection(getCharacterIndexAt(relativeMousePosition.x, relativeMousePosition.y));
                    m_leftClicking = true;
                }
            }
            
            needAutoSizeUpdate();
            updateText();
        }
        else if(event.type == sf::Event::MouseMoved)
        {
            if(m_leftClicking)
            {
                if(widgetRect.contains(event.mouseMove.x, event.mouseMove.y))
                {
                    sf::Vector2f relativeMousePosition(globalTr.getInverse().transformPoint(event.mouseMove.x, event.mouseMove.y));   
                    setSelection(m_selectionStart, getCharacterIndexAt(relativeMousePosition.x, relativeMousePosition.y) - m_selectionStart);
                }
            }
            
            needAutoSizeUpdate();
            updateText();
        }
        else if(event.type == sf::Event::MouseButtonReleased)
        {
            if(event.mouseButton.button == sf::Mouse::Left && m_leftClicking)
            {
                if(widgetRect.contains(event.mouseButton.x, event.mouseButton.y))
                {
                    sf::Vector2f relativeMousePosition(globalTr.getInverse().transformPoint(event.mouseButton.x, event.mouseButton.y));   
                    setSelection(m_selectionStart, getCharacterIndexAt(relativeMousePosition.x, relativeMousePosition.y) - m_selectionStart);
                }
                m_leftClicking = false;
            }
            
            needAutoSizeUpdate();
            updateText();
        }
        else if(event.type == sf::Event::TextEntered)
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
                        eraseSelection();
                    }
                }
            }
            else //Avoid useless update (below) if the text is not processed
                return;
            
            needAutoSizeUpdate();
            updateText();
            
            onTextChanged.call(m_string);
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Delete)
            {
                if(!m_string.empty())
                {
                    if(!hasMultipleCharSelected() && m_selectionStart < m_string.size())
                    {
                        m_string.replace(m_selectionStart, 1, U"");
                        setSelection(m_selectionStart);
                        
                        needAutoSizeUpdate();
                        updateText();
                    }
                    else if(hasMultipleCharSelected())
                    {
                        eraseSelection();
                        
                        needAutoSizeUpdate();
                        updateText();
                    }
                }
            }
            else if(event.key.code == sf::Keyboard::Left)
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
            !hasMultipleCharSelected() ? 2 : (getCharacterPosition(m_selectionLen + m_selectionStart).x - getCharacterPosition(m_selectionStart).x), 
            getEffectiveSize().y - 6.f
            )
        );
    cursorShape.setPosition(sf::Vector2f(
        getCharacterPosition(m_selectionStart).x, 
        3.f
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
        it != m_string.end() && (m_text.getLocalBounds().left + m_text.getLocalBounds().width < (getSize().x == AUTO_SIZE ? getMaxSize().x : getSize().x) - 6.f);
        ++it)
    {
        textString.insert(textString.getSize(), static_cast<sf::Uint32>(*it));
        m_text.setString(textString);
    }
    
    if(m_text.getLocalBounds().left + m_text.getLocalBounds().width >= (getSize().x == AUTO_SIZE ? getMaxSize().x : getSize().x) - 6.f)
        textString.erase(textString.getSize()-1);
        
    m_lastDisplayedCharIndex = textString.getSize() > 0 ? m_firstDisplayedCharIndex + textString.getSize()-1 : 0;
    
    m_text.setString(textString);
}

void TextBox::ensureCharacterIsVisible(std::size_t pos)
{
    updateText();

    //Move the display margin until the character is visible
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
    
    //Try to avoid a blank space on the right of the TextBox if the text is too much offsetted
    while(m_lastDisplayedCharIndex >= m_string.size() - 1 && m_firstDisplayedCharIndex > 0)
    {
        --m_firstDisplayedCharIndex;
        needAutoSizeUpdate();
        updateText();
        if(m_lastDisplayedCharIndex >= 0 && m_lastDisplayedCharIndex < m_string.size() - 1)
        {
            ++m_firstDisplayedCharIndex;
            needAutoSizeUpdate();
            updateText();
            break;
        }
    }
}

bool TextBox::hasMultipleCharSelected() const
{
    return (m_selectionLen != 0);
}

sf::Vector2f TextBox::getCharacterPosition(int index) const
{
    if(index <= 0)
        return m_text.getPosition();
    else if(index >= m_string.size())
        return sf::Vector2f(m_text.getGlobalBounds().left + m_text.getGlobalBounds().width, m_text.getGlobalBounds().top + m_text.getGlobalBounds().height);
    else 
        return m_text.findCharacterPos(index - m_firstDisplayedCharIndex);
}

int TextBox::getCharacterIndexAt(float x, float y) const
{
    int charIndex = 0;
    
    while((m_text.findCharacterPos(charIndex).x + (charIndex < (m_lastDisplayedCharIndex - m_firstDisplayedCharIndex) ? m_text.findCharacterPos(charIndex+1).x : (m_text.getLocalBounds().left + m_text.getLocalBounds().width)))/2.f < x && 
        charIndex < (m_lastDisplayedCharIndex - m_firstDisplayedCharIndex + 1))
    {
        ++charIndex;
    }
    
    return charIndex + m_firstDisplayedCharIndex;
}

void TextBox::eraseSelection()
{
    m_string.replace(
        std::min(m_selectionStart, m_selectionStart + m_selectionLen), 
        std::max(m_selectionLen, -m_selectionLen), 
        U""
    );
                        
    setSelection(m_selectionStart + std::min(0, m_selectionLen));
}

}

