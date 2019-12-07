#ifndef IFCONTROL_H
#define IFCONTROL_H

#include "interface.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class CInterface;

class IControl : public sf::Drawable
{
public:
	class CEventListener
	{
	public:
		virtual ~CEventListener() {}
		virtual void onEvent(IControl* Control, int EventId) {}
	};
protected:
	//sf::Rect<int> m_Pos;
private:
    CInterface* m_Interface;
	std::vector<CEventListener*> m_Listeners;
	sf::Rect<int> m_Pos;
public:
    IControl(CInterface* Interface);
    virtual ~IControl();

    CInterface* getInterface(){return m_Interface;}

	void setPosition(const sf::Rect<int>& Pos);
	const sf::Rect<int>& getPosition() const { return m_Pos; }

	void addListener(CEventListener* Listener);
	void removeListener(CEventListener* Listener);
    virtual void handleInput(sf::Event) = 0;

	virtual void update() {}
    virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const = 0;
protected:
	void event(int eventId);
};

#endif
