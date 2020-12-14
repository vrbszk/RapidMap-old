#pragma once

class Button;

#include "Entity.hpp"

class Button : public Entity
{
public:
	Button();
	Button(sf::Vector2f size);
	~Button();

	virtual void update(sf::Vector2f mousePos);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setActiveColor(sf::Color c);// = sf::Color(200, 200, 200));
	void setPassiveColor(sf::Color c);// = sf::Color(100, 100, 100));
	void setPressedColor(sf::Color c);//	= sf::Color(150, 150, 150));

	virtual sf::FloatRect getCollideBox();
	
	bool pressed;

private:
	sf::RectangleShape shape;

	void init();

	

protected:
	sf::Color activeColor;
	sf::Color passiveColor;
	sf::Color pressedColor;
};