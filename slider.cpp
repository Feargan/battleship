#include "slider.h"
#include "sections.h"

#include <SFML/Graphics.hpp>

bool CSlider::CResources::load(const char* Filename)
{
	CSections Sections;
	Sections.loadFromFile(Filename);
	auto TxtBar = Sections.get("TXT_BAR");
	auto TxtPointer = Sections.get("TXT_POINTER");
	if (!TxtBar || !TxtPointer)
		return false;
	m_TxtBar.loadFromMemory(&(*TxtBar)[0], TxtBar->size());
	m_TxtPointer.loadFromMemory(&(*TxtPointer)[0], TxtPointer->size());
	return true;
}

CSlider::CSlider(CPanel * Panel) : IControl(Panel), m_Min(0), m_Max(0), m_Div(1), m_Value(0), m_Hover(false)
{
}

CSlider::~CSlider()
{
}

void CSlider::setResources(const CResources& Resources)
{
	m_Resources = Resources;
}

void CSlider::setMin(int Min)
{
	m_Min = Min;
}

int CSlider::getMin() const
{
	return m_Min;
}

void CSlider::setMax(int Max)
{
	m_Max = Max;
}

int CSlider::getMax() const
{
	return m_Max;
}

void CSlider::setDiv(int Div)
{
	if(Div != 0)
		m_Div = Div;
}

int CSlider::getDiv() const
{
	return m_Div;
}

int CSlider::getValue() const
{
	return m_Value;
}

void CSlider::setValue(int Value)
{
	m_Value = Value;
}

void CSlider::handleInput(sf::Event Event)
{
	if (getPanel()->getCurrentFocus() == this)
	{
		m_Hover = false;
		int x;
		if (Event.type == sf::Event::MouseMoved)
			x = Event.mouseMove.x - getPosition().left;
		else if (Event.type == sf::Event::MouseButtonPressed)
			x = Event.mouseButton.x - getPosition().left;
		else return;
		int Value = static_cast<int>(std::floor(static_cast<double>(x) / getPosition().width * (m_Max - m_Min) / m_Div + m_Min + 0.5))*m_Div;
		if (Value > m_Max)
			Value = m_Max;
		if (Value < m_Min)
			Value = m_Min;
		if (Value != m_Value)
		{
			m_Value = Value;
			event(VALUE_CHANGED);
		}
	}
	else
	{
		if ((Event.type == sf::Event::MouseMoved && getPosition().contains({ Event.mouseMove.x, Event.mouseMove.y }))
			|| (Event.type == sf::Event::MouseButtonReleased && getPosition().contains({ Event.mouseButton.x, Event.mouseButton.y })))
			m_Hover = true;
		else
			m_Hover = false;
	}
}

void CSlider::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	const auto& Pos = getPosition();
	sf::Sprite Spr(m_Resources.m_TxtBar);
	Spr.setScale(static_cast<float>(Pos.width) / m_Resources.m_TxtBar.getSize().x, static_cast<float>(Pos.height) / m_Resources.m_TxtBar.getSize().y);
	Spr.setPosition(static_cast<float>(Pos.left), static_cast<float>(Pos.top));
	Target.draw(Spr, States);
	Spr = sf::Sprite(m_Resources.m_TxtPointer);
	Spr.setPosition(Pos.left + static_cast<float>(m_Value-m_Min) / (m_Max - m_Min)*Pos.width - m_Resources.m_TxtPointer.getSize().x / 2.f, static_cast<float>(Pos.top));
	if (m_Hover)
		Spr.setColor(sf::Color(255, 200, 200));
	Target.draw(Spr, States);
}
