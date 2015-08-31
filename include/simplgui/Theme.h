#ifndef SIMPLGUI_THEME_H
#define SIMPLGUI_THEME_H

#include <map>
#include <string>

#include "simplgui/Any.h"

namespace simplgui
{

class Theme
{
public:
    Theme() : m_properties() {};
    
    bool hasProperty(const std::string &name) const
    {
        return m_properties.count(name) != 0;
    }
    
    template<class T>
    bool hasPropertyOfType(const std::string &name) const
    {
        return hasProperty(name) && m_properties.at(name).is<T>();
    }
    
    template<class T>
    const T& getProperty(const std::string &name) const
    {
        return m_properties.at(name).as<T>();
    }
    
    template<class T>
    void setProperty(const std::string &name, T &&value)
    {
        m_properties[name] = Any(std::forward<T>(value));
    }
    
    static Theme defaultTheme()
    {
        Theme th;
        th.setProperty<std::string>("font", "LiberationSans.ttf");
        th.setProperty<unsigned int>("font_size", 30);
        
        return th;
    }
    
private:
    std::map<std::string, Any> m_properties;
};

}

#endif

