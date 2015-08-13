#include <SFML/Graphics.hpp>

#include "simplgui/TextBox.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "A test of SimplGUI");
    
    sf::Font myFont;
    myFont.loadFromFile("LiberationSans.ttf");
    
    simplgui::TextBox::Ptr textBox = simplgui::TextBox::create();
    textBox->setPosition(sf::Vector2f(100, 100));
    textBox->setSize(sf::Vector2f(250, 30));
    textBox->setFont(myFont);
    
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

        window.clear();
        window.draw(*textBox);
        window.display();
    }

    return 0;
}
