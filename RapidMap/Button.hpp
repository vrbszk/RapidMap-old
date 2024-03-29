#pragma once

#include "Entity.hpp"

class Button : public Entity
{
public:
	Button();
	Button(sf::Vector2f size);
	~Button();

	void update(sf::Vector2f mousePos);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::RectangleShape shape;
};