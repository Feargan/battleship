#include "text.h"



CText::CText(CPanel* Panel) : IControl(Panel), m_Timer(0)
{
}


CText::~CText()
{
}

void CText::setProperties(const sf::Text & Text)
{
	m_Text = Text;
}

void CText::setText(const char * Text)
{
	m_Text.setString(Text);
}

void CText::setTimer(int Millis)
{
	m_Timer = Millis;
}

void CText::disableTimer()
{
	m_Timer = 0;
}

void CText::flash()
{
	m_Clock.restart();
}

void CText::update()
{
	m_Text.setFillColor(sf::Color(0, 0, 0));
	m_Text.setPosition({ static_cast<float>(getPosition().left), static_cast<float>(getPosition().top) });
}

void CText::handleInput(sf::Event)
{
	return;
}

void CText::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	if (m_Timer > 0 && m_Clock.getElapsedTime().asMilliseconds() >= m_Timer)
		return;

	Target.draw(m_Text, States);
}
