#pragma once
#include "panel.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class CPanel;

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
    CPanel* m_Panel;
	std::vector<CEventListener*> m_Listeners;
	sf::Rect<float> m_Pos;
public:
    IControl(CPanel* Panel);
    virtual ~IControl();
	IControl(const IControl& r) = delete;
	IControl& operator=(const IControl& r) = delete;
	IControl(IControl&& r) = delete;
	IControl& operator=(IControl&& r) = delete;

    CPanel* getPanel(){return m_Panel;}

	void setPosition(const sf::Rect<float>& Pos);
	const sf::Rect<float>& getPosition() const;

	void addListener(CEventListener* Listener);
	void removeListener(CEventListener* Listener);
    virtual void handleInput(sf::Event) = 0;

	virtual void update() {}
    virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const = 0;
protected:
	void event(int eventId);
};