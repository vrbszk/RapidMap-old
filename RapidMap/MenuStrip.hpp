#include "Interface.hpp"

class MenuStrip : public Interface
{
public:
	MenuStrip();
	void updateEvents(sf::Event e);
	void updateInterface(sf::FloatRect space);
	void render();

	sf::Vector2f getMinSize();
};