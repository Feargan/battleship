#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <vector>

class IControl;

class CPanel : public sf::Drawable
{
    sf::Rect<int> m_Area;
	sf::RenderTexture m_RenderTexture;
    std::vector<IControl*> m_Controls;
    IControl* m_Focus;
    IControl* m_Released;
public:
	CPanel(const sf::Rect<int>& Area = { 0, 0, 1, 1 });
	~CPanel();
	CPanel(const CPanel&) = delete;
	CPanel(CPanel&&) = delete;

    class ControlKey
    {
        friend class IControl;
        ControlKey(){}
    };

	void setArea(const sf::Rect<int>& Area);
	const sf::Rect<int>& getArea() const;
	void setPosition(sf::Vector2i Pos);
	sf::Vector2i getPosition() const;
	void autoSize();

    void addControl(ControlKey Key, IControl* Control);
    void deleteControl(ControlKey Key, IControl* Control);
    void destroy();

    void handleInput(sf::Event);

	void update();

	const IControl* getCurrentFocus() const;
	const IControl* getLastReleased() const;

private:
    virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
	void normalize(sf::Event& Event) const;
};

#endif
