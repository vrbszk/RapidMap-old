#include "Interface.hpp"

class MenuStrip : public Interface
{
public:
	void updateEvents(sf::Event e);
	void updateInterface(sf::FloatRect space);
	void render();
};