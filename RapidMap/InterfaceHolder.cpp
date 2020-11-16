#include "InterfaceHolder.hpp"

InterfaceHolderNode::InterfaceHolderNode()
{
	borderVertcal = false;
	borderSize = 0;
	stableNode = 0;

	inth1 = NULL;
	inth2 = NULL;
}

InterfaceHolderNode::~InterfaceHolderNode()
{
	delete inth1;
	delete inth2;
}

void InterfaceHolderNode::setWindow(sf::RenderWindow* win)
{
	window = win;
	if (inth1)
		inth1->setWindow(win);
	if (inth2)
		inth2->setWindow(win);
}

void InterfaceHolderNode::updateEvents(sf::Event e)
{
	if (inth1)
		inth1->updateEvents(e);
	if (inth2)
		inth2->updateEvents(e);
}

void InterfaceHolderNode::updateInterface(sf::FloatRect space)
{
	view = sf::View(space);

	sf::FloatRect firstSpace = space;
	sf::FloatRect secondSpace = space;

	if (inth1 && inth2)
	{
		if (borderVertcal)
		{
			int h = space.height;
			int w1 = borderSize;
			int w2 = space.width - borderSize;
			firstSpace = sf::FloatRect(space.left, space.top, w1, h);
			secondSpace = sf::FloatRect(space.left + w1, space.top, w2, h);
		}
		else
		{
			int w = space.width;
			int h1 = borderSize;
			int h2 = space.height - borderSize;
			firstSpace = sf::FloatRect(space.left, space.top, w, h1);
			secondSpace = sf::FloatRect(space.left, space.top + h1, w, h2);
		}
	}
	if (inth1)
		inth1->updateInterface(firstSpace);
	if (inth2)
		inth2->updateInterface(secondSpace);
}

void InterfaceHolderNode::render()
{
	if (inth1)
		inth1->render();
	if (inth2)
		inth2->render();
}

sf::Vector2f InterfaceHolderNode::getMinSize()
{
	sf::Vector2f s1, s2;
	if (inth1)
		s1 = inth1->getMinSize();
	if (inth2)
		s2 = inth2->getMinSize();

	minSize.x = (s1.x > s2.x) ? s1.x : s2.x;
	minSize.y = (s1.y > s2.y) ? s1.y : s2.y;

	std::cout << minSize.x << " " << minSize.y << std::endl;

	return minSize;
}
//
//void InterfaceHolderLeaf::updateEvents(sf::Event e)
//{
//	if (inter)
//		inter->updateEvents(e);
//}
//
//void InterfaceHolderLeaf::updateInterfaces(sf::FloatRect space)
//{
//	if (inter)
//		inter->updateInterface(space);
//}
//
//void InterfaceHolderLeaf::render()
//{
//	if (inter)
//		inter->render();
//}
//
//void InterfaceHolderLeaf::setWindow(sf::RenderWindow* win)
//{
//	window = win;
//	if (inter)
//		inter->setWindow(win);
//}