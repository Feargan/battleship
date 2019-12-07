#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>

class IControl;

class CInterface : public sf::Drawable
{
    sf::Rect<int> m_Area;
    std::vector<IControl*> m_Controls;
    IControl* m_Focus;
    IControl* m_Released;
public:
    CInterface();
	~CInterface();

    class ControlKey
    {
        friend class IControl;
        ControlKey(){}
    };

    void addControl(ControlKey Key, IControl* Control);
    void deleteControl(ControlKey Key, IControl* Control);
    void destroy();

    void handleInput(sf::Event);

	void update();

    IControl* GetCurrentFocus(){return m_Focus;}
    IControl* GetLastReleased(){return m_Released;}

    virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;

};

#endif
