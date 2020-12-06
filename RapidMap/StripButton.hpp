#pragma once

class StripButton;

#include "Button.hpp"

class StripButton : public Button
{
public:
	StripButton(sf::Vector2f size);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	sf::Text text;
};