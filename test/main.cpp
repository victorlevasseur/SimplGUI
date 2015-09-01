#include <codecvt>
#include <iostream>
#include <locale>

#include <SFML/Graphics.hpp>

#include "simplgui/TextBox.h"

void whenTextChanged(const std::u32string &str)
{
    
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "A test of SimplGUI");
    
    simplgui::Theme myTheme = simplgui::Theme::defaultTheme();
    myTheme.setProperty<std::string>("font", "DejaVu.ttf");
    myTheme.setProperty<unsigned int>("font_size", 50);
    
    simplgui::TextBox::Ptr textBox = simplgui::TextBox::create();
    textBox->setPosition(sf::Vector2f(100, 100));
    textBox->setText(U"Some text");
    //textBox->setHideCharacter();
    textBox->setMinSize(sf::Vector2f(200.f, simplgui::NO_MIN_SIZE));
    textBox->setMaxSize(sf::Vector2f(400.f, simplgui::NO_MAX_SIZE));
    textBox->setSize(sf::Vector2f(simplgui::AUTO_SIZE, simplgui::AUTO_SIZE));
    textBox->setTheme(myTheme);
    
    unsigned int bindId = textBox->onTextChanged.bind(whenTextChanged);
    
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
                
            textBox->processEvent(event);
        }
        
        auto dt = clock.restart();
        
        textBox->update(dt);

        window.clear(sf::Color(128, 128, 128));
        window.draw(*textBox);
        window.display();
    }

    return 0;
}
