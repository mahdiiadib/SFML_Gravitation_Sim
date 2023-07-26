//#include "rough.cpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>


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


int descnt=0;

class Particle
{
	sf::Vector2f pos, vel;
	sf::CircleShape s;

public:
	bool destroyed=false;

	Particle(float posx, float posy, float velx, float vely, sf::Color color=sf::Color::White)
	{
		pos=sf::Vector2f(posx, posy);
		vel=sf::Vector2f(velx, vely);

		s.setPosition(pos);
		s.setFillColor(sf::Color::White);
		s.setRadius(2.5);
		s.setFillColor(color);
	}

	void render(sf::RenderWindow& windo)
	{
		//if(!destroyed)
		{
			s.setPosition(pos);
			windo.draw(s);
		}
	}

	sf::Vector2f get_pos()
	{
		return pos;
	}

	sf::Vector2f get_vel()
	{
		return vel;
	}

	void set_color(sf::Color col)
	{
		s.setFillColor(col);
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

		if(pos.x<=510 && pos.x>=490 && pos.y<=510 && pos.y>=490) destroyed=true, set_color(sf::Color::Yellow), std::cout<<++descnt<<'\n';
		if(pos.x<=1210 && pos.x>=1190 && pos.y<=510 && pos.y>=490) destroyed=true, set_color(sf::Color::Cyan), std::cout<<++descnt<<'\n';
	}
};


sf::Color map_val_to_color(float value) //value is 0-1
{
	if(value<0) value=0;
	if(value>1) value=1;

	int r=0, g=0, b=0;

	if(value < 0.5f)
	{
		b = 255 * (1.0 - 2 * value);
		g = 255 * 2 * value;
	}
	else
	{
		g = 255 * (2.0 - 2 * value);
		r = 255 * (2 * value - 1);
	}

	return sf::Color(r, g, b);
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 1000), "Orbital Gravity Simulation");
	window.setFramerateLimit(120);

	std::vector<GravitySource> sources;
	sources.push_back(GravitySource(500, 500, 7000, sf::Color::Yellow));
	sources.push_back(GravitySource(1200, 500, 7000, sf::Color::Cyan));

	int num_particles=2000;

	std::vector<Particle> particles;

	for(int i=0; i<num_particles; i++)
	{
		particles.push_back(Particle(600, 700, 4, 0.2+(0.1/num_particles)*i));

		//change colors
		float val=(float)i/(float)num_particles;
		particles[i].set_color(map_val_to_color(val));
	}

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type==sf::Event::Closed) window.close();
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		}

		window.clear();
		
		for(GravitySource& src : sources)
		{
			for(Particle& p : particles) if(!p.destroyed) p.update_physics(src);
		}

		for(GravitySource& src : sources) src.render(window);
		for(Particle& p : particles) p.render(window);

		window.display();
	}
}