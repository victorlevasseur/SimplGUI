#include "simplgui/TextBox.h"

namespace simplgui
{

std::shared_ptr<TextBox> TextBox::create()
{
    return std::shared_ptr<TextBox>(new TextBox());
}

TextBox::TextBox() : Widget(), m_string(), m_text()
{
    setSize(sf::Vector2f(150.f, 40.f));
    m_text.setColor(sf::Color(0, 0, 0));
}

void TextBox::setFont(const sf::Font &font)
{
    m_text.setFont(font);
}

void TextBox::doProcessEvent(sf::Event event)
{
    if(event.type == sf::Event::TextEntered)
    {
        sf::Uint32 character = event.text.unicode;
        if(character > 30 && (character < 127 || character > 159))
        {
            m_string.push_back(character);
        }
        else
        {
            m_string.pop_back();
        }
        m_text.setString((sf::Uint32*)m_string.c_str());
    }
}

void TextBox::doUpdate(sf::Time dt)
{

}

void TextBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape bgShape(getSize());
    bgShape.setFillColor(sf::Color(255, 255, 255));
    
    target.draw(bgShape, getGlobalTransform());
    
    target.draw(m_text, getGlobalTransform());
}

}
