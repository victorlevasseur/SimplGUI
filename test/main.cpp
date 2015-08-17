#include <iostream>

#include <SFML/Graphics.hpp>

#include "simplgui/TextBox.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "A test of SimplGUI");
    
    sf::Font myFont;
    myFont.loadFromFile("LiberationSans.ttf");
    
    simplgui::TextBox::Ptr textBox = simplgui::TextBox::create();
    textBox->setPosition(sf::Vector2f(100, 100));
    textBox->setMinSize(sf::Vector2f(200.f, simplgui::Widget::NO_MIN_SIZE));
    textBox->setMaxSize(sf::Vector2f(600.f, simplgui::Widget::NO_MAX_SIZE));
    textBox->setSize(sf::Vector2f(simplgui::Widget::AUTO_SIZE, simplgui::Widget::AUTO_SIZE));
    textBox->setFont(myFont);
    
    textBox->onTextChanged.bind([](const std::u32string &str){std::cout << "Textbox text has been changed" << std::endl;});
    
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
