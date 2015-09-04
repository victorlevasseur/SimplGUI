#include <codecvt>
#include <iostream>
#include <locale>

#include <SFML/Graphics.hpp>

#include "simplgui/FileResourcesGetter.h"
#include "simplgui/TextBox.h"

void whenTextChanged(const std::u32string &str)
{
    
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "A test of SimplGUI");
    
    //THEME
    simplgui::Theme myTheme = simplgui::Theme::defaultTheme();
    myTheme.setProperty<std::string>("font", "DejaVu.ttf");
    myTheme.setProperty<unsigned int>("font_size", 30);
    
    //RESOURCES GETTER
    auto resGetter = simplgui::FileResourcesGetter::create();
    
    //TEXTBOX
    simplgui::TextBox::Ptr textBox = simplgui::TextBox::create(resGetter);
    textBox->setPosition(sf::Vector2f(100, 100));
    textBox->setText(U"Some text");
    textBox->setSize(sf::Vector2f(300, simplgui::AUTO_SIZE));
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

        window.clear(sf::Color(200, 200, 200));
        window.draw(*textBox);
        window.display();
    }

    return 0;
}
