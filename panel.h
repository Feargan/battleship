#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>

class IControl;

class CPanel : public sf::Drawable
{
    sf::Rect<int> m_Area;
    std::vector<IControl*> m_Controls;
    IControl* m_Focus;
    IControl* m_Released;
public:
    CPanel();
	~CPanel();

    class ControlKey
    {
        friend class IControl;
        ControlKey(){}
    };

	void setArea(const sf::Rect<int>& Area);

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
