#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Entity;

typedef std::unique_ptr<Entity> EntityPtr;

class Entity : public sf::Drawable, public sf::Transformable
{
public:
	Entity() { }
	virtual ~Entity() { };
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void update(sf::Vector2f mousePos) = 0;
};