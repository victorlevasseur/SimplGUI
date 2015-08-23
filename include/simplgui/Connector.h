#ifndef SIMPLGUI_CONNECTOR_H
#define SIMPLGUI_CONNECTOR_H

#include <functional>
#include <unordered_set>

namespace simplgui
{

template<class... Args>
class Connector
{
public:
    Connector() {};
    
    void bind(std::function<void(Args...)> &function)
    {
        m_bindedFunctions.push_back(function);
    }
    
    void bind(std::function<void(Args...)> &&function)
    {
        m_bindedFunctions.push_back(function);
    }
    
    void call(Args... args)
    {
        for(auto it = m_bindedFunctions.begin(); it != m_bindedFunctions.end(); ++it)
        {
            (*it)(args...);
        }
    }

private:
    std::vector<std::function<void(Args...)>> m_bindedFunctions;
};

}

#endif
