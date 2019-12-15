#pragma once
#include "panel.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class CPanel;

class IControl : public sf::Drawable
{
public:
	class IEventListener
	{
	public:
		virtual ~IEventListener() {}
		virtual void onEvent(IControl* Control, int EventId) {}
	};
protected:
	//sf::Rect<int> m_Pos;
private:
    CPanel* m_Panel;
	std::vector<IEventListener*> m_Listeners;
	sf::Rect<int> m_Pos;
public:
    IControl(CPanel* Panel);
    virtual ~IControl();
	IControl(const IControl& r) = delete;
	IControl& operator=(const IControl& r) = delete;
	IControl(IControl&& r) = delete;
	IControl& operator=(IControl&& r) = delete;

    CPanel* getPanel(){return m_Panel;}

	void setPosition(const sf::Rect<int>& Pos);
	const sf::Rect<int>& getPosition() const;

	void addListener(IEventListener* Listener);
	void removeListener(IEventListener* Listener);
    virtual void handleInput(sf::Event) = 0;

	virtual void update() {}
    virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const = 0;
protected:
	void event(int eventId);
};