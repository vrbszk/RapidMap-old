#include "Window.hpp"

Window::Window() : sf::RenderWindow()
{
	standardView = this->getDefaultView();
}

Window::Window(sf::VideoMode mode, const sf::String& title, sf::Uint32 style, const sf::ContextSettings& settings) : 
	sf::RenderWindow(mode, title, style, settings)
{
	standardView = this->getDefaultView();
}

Window::Window(sf::WindowHandle handle, const sf::ContextSettings& settings) :
	sf::RenderWindow(handle, settings)
{
	standardView = this->getDefaultView();
}

Window::~Window()
{

}

void Window::addInterface(InterfacePtr ptr)
{
	ptr->setWindow(this);
	interfaces.push_back(std::move(ptr));
}

void Window::updateEvents()
{
	sf::Event event;
	while (this->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			core->closeWindow(this);
			break;
		case sf::Event::Resized:
		{
			this->standardView = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
			this->setSize(sf::Vector2u(event.size.width, event.size.height));
			break;
		}

		
		}

		for (auto it = interfaces.begin(); it != interfaces.end(); it++)
		{
			(*it)->processEvents(event);
		}
	}
}

void Window::updateWindow()
{
	sf::FloatRect spaceLeft(sf::Vector2f(0, 0), standardView.getSize());

	sf::FloatRect stripSpace(sf::Vector2f(spaceLeft.left, spaceLeft.top), sf::Vector2f(spaceLeft.width, 20));

	//menuStrip.updateInterface(stripSpace);
	interfaces[1]->createView(stripSpace);
	interfaces[1]->update();
	//interfaces[1]->updateInterface(stripSpace);

	spaceLeft.top = interfaces[1]->getView().getSize().y;
	spaceLeft.height = spaceLeft.height - interfaces[1]->getView().getSize().y;

	sf::FloatRect toolstripSpace(sf::Vector2f(spaceLeft.left, spaceLeft.top), sf::Vector2f(spaceLeft.width, 50));

	interfaces[3]->createView(toolstripSpace);
	interfaces[3]->update();
	//interfaces[3]->updateInterface(toolstripSpace);


	spaceLeft.top = interfaces[3]->getView().getSize().y + spaceLeft.top;
	spaceLeft.height = spaceLeft.height - interfaces[3]->getView().getSize().y;

	sf::FloatRect stateSpace(sf::Vector2f(spaceLeft.left + spaceLeft.width - 200, spaceLeft.top), sf::Vector2f(200, spaceLeft.height));

	//stateBlock.updateInterface(stateSpace);
	interfaces[2]->createView(stateSpace);
	interfaces[2]->update();
	//interfaces[2]->updateInterface(stateSpace);

	spaceLeft.width = spaceLeft.width - interfaces[2]->getView().getSize().x;

	interfaces[0]->createView(spaceLeft);
	interfaces[0]->update();
	//interfaces[0]->updateInterface(spaceLeft);
}

void Window::render()
{
	this->clear(sf::Color::White);

	this->setView(standardView);

	for (auto it = interfaces.begin(); it != interfaces.end(); it++)
	{
		(*it)->render();
	}

	this->setView(standardView);

	if (hintString != "")
	{
		sf::Text text(hintString, hintFont, 12);
		text.setFillColor(sf::Color::Black);
		
		text.setPosition(sf::Vector2f(sf::Mouse::getPosition(*this)));
		

		sf::RectangleShape shape(sf::Vector2f(text.getGlobalBounds().width + 6, text.getGlobalBounds().height + 6));
		shape.setFillColor(sf::Color::Yellow);
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(255, 150, 0));
		shape.setPosition(sf::Vector2f(sf::Mouse::getPosition(*this)));
		shape.setOrigin(shape.getGlobalBounds().width, 0);
		text.setOrigin(shape.getOrigin().x - 4, 2);

		this->draw(shape);
		this->draw(text);
	}

	this->display();
}

//void Window::setHintText(std::string str)
//{
//	hintString = str;
//}

void Window::setHintText(sf::String str)
{
	hintString = str;
}

void Window::setHintFont(sf::Font f)
{
	hintFont = f;
}