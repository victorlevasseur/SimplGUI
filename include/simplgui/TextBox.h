#ifndef SIMPLGUI_TEXTBOX_H
#define SIMPLGUI_TEXTBOX_H

#include <SFML/Graphics.hpp>
#include "simplgui/Widget.h"
#include "simplgui/Connector.h"

namespace simplgui
{

/**
 * \brief TextBox is a simple field to let the user enter a line of text.
 */
class TextBox : public Widget
{
public:
    using Ptr = std::shared_ptr<TextBox>; ///< Shared pointer to TextBox
    using ConstPtr = std::shared_ptr<const TextBox>; ///< Shared pointer to const TextBox

    static std::shared_ptr<TextBox> create();
    virtual ~TextBox() {};
    
    /**
     * Set the font used by this widget.
     */
    void setFont(const sf::Font &font);
    
    /**
     * \return the start position of the selection
     */
    int getSelectionStart() const;
    
    /**
     * \return the length of the selection.
     * \note This length can be negative if the user made its selection from right to left.
     */
    int getSelectionLen() const;
    
    /**
     * Set the selection.
     * \param start the start position of the selection
     * \param len the length of the selection
     * \note len can be negative to represent a "from right to left" selection.
     */
    void setSelection(int start, int len = 0);
    
    /**
     * \return the text contained in the textbox.
     */
    std::u32string getText() const { return m_string; };
    
    /**
     * Set the text of the textbox
     */
    void setText(const std::u32string &text);
    
    /**
     * \return the character used to hide all character in the textbox (or 0 if not)
     */
    char32_t getHideCharacter() const { return m_hideChar; };
    
    /**
     * Set the hide character, a character that replaces all characters to hide them (like in a password field).
     */
    void setHideCharacter(char32_t hideChar = U'\x25CF');
    
    Connector<const std::u32string&> onTextChanged; ///< Event emitted when the text changes

protected:
    TextBox();
    
    virtual void doProcessEvent(sf::Event event);
    virtual void doUpdate(sf::Time dt);
    
    virtual void onSizeUpdated();
    
    virtual sf::Vector2f doCalculateAutoSize() const;
    
    virtual void doThemeUpdate();
    
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    void updateText();
    
    void ensureCharacterIsVisible(unsigned int pos);
    
    bool hasMultipleCharSelected() const;
    
    /**
     * Improved version of findCharacterPos of SFML so as it returns the full size of
     * the text if the index = m_string.size()
     */
    sf::Vector2f getCharacterPosition(unsigned int index) const;
    
    int getCharacterIndexAt(float x, float y) const;
    
    void eraseSelection();
 
    std::u32string m_string;
    
    sf::Text m_text;
    sf::Font m_font;
    unsigned int m_firstDisplayedCharIndex;
    unsigned int m_lastDisplayedCharIndex;
    
    int m_selectionStart;
    int m_selectionLen;
    
    char32_t m_hideChar;
    
    bool m_leftClicking;
};

}

#endif
