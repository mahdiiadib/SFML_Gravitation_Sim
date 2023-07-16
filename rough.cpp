#pragma once

#include <SFML/Graphics.hpp>

inline void bouncy_rect()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "bouncy_rect");
	window.setFramerateLimit(75);

	sf::RectangleShape rect;
	sf::Vector2f rectPos(600, 350);
	rect.setPosition(rectPos);
	rect.setSize(sf::Vector2f(100, 100));
	rect.setFillColor(sf::Color::Green);

	float xvel=3, yvel=3;

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type==sf::Event::Closed) window.close();
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		}

		if(rectPos.x<=0 || rectPos.x+rect.getSize().x>=window.getSize().x) xvel*=-1;
		if(rectPos.y<=0 || rectPos.y+rect.getSize().y>=window.getSize().y) yvel*=-1;

		rectPos.x+=xvel, rectPos.y+=yvel;
		rect.setPosition(rectPos);

		window.clear();
		window.draw(rect);
		window.display();
    }
}

inline void write_text()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "bouncy_rect");
	window.setFramerateLimit(75);

	sf::Font font;
	font.loadFromFile("fonts\\AGENCYR.TTF");
	sf::Text text;
	text.setFont(font);
	text.setString("Helo World");
	text.setCharacterSize(30);
	text.setStyle(sf::Text::Italic);
	text.setPosition(300, 400);
	text.setFillColor(sf::Color::Green);

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type==sf::Event::Closed) window.close();
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		}

		window.clear();
		window.draw(text);
		window.display();
	}
}



class GravitySource
{
	sf::Vector2f pos;
	float strength;
	sf::CircleShape s;

public:
	GravitySource(float posx, float posy, float strength, sf::Color color=sf::Color::White)
	{
		pos=sf::Vector2f(posx, posy);
		this->strength=strength;

		s.setPosition(pos);
		s.setFillColor(sf::Color::White);
		s.setRadius(10);
		s.setFillColor(color);
	}

	void render(sf::RenderWindow& windo)
	{
		s.setPosition(pos);
		windo.draw(s);
	}

	sf::Vector2f get_pos()
	{
		return pos;
	}

	float get_strength()
	{
		return strength;
	}
};


class Particle
{
	sf::Vector2f pos, vel;
	sf::CircleShape s;

public:
	Particle(float posx, float posy, float velx, float vely, sf::Color color=sf::Color::White)
	{
		pos=sf::Vector2f(posx, posy);
		vel=sf::Vector2f(velx, vely);

		s.setPosition(pos);
		s.setFillColor(sf::Color::White);
		s.setRadius(8);
		s.setFillColor(color);
	}

	void render(sf::RenderWindow& windo)
	{
		s.setPosition(pos);
		windo.draw(s);
	}

	sf::Vector2f get_pos()
	{
		return pos;
	}

	sf::Vector2f get_vel()
	{
		return vel;
	}

	void update_physics(GravitySource& s)
	{
		float distance_x=s.get_pos().x-pos.x;
		float distance_y=s.get_pos().y-pos.y;
		float distance=sqrt(distance_x*distance_x+distance_y*distance_y);

		float inverse_distance=1.0/distance;
		float normalized_x=inverse_distance*distance_x;
		float normalized_y=inverse_distance*distance_y;

		float inverse_square_dropoff=inverse_distance*inverse_distance;
		float acceleration_x=normalized_x*s.get_strength()*inverse_square_dropoff;
		float acceleration_y=normalized_y*s.get_strength()*inverse_square_dropoff;

		vel+={acceleration_x, acceleration_y};
		pos+=vel;
	}
};


inline void orbital_gravity()
{
	sf::RenderWindow window(sf::VideoMode(1600, 1000), "Orbital Gravity Simulation");
	window.setFramerateLimit(75);

	GravitySource source(800, 500, 7000, sf::Color::Yellow);
	Particle particle(600, 700, 4, 0, sf::Color::Cyan);

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type==sf::Event::Closed) window.close();
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		}

		window.clear();
		particle.update_physics(source);
		//draw calls
		source.render(window);
		particle.render(window);
		window.display();
	}
}