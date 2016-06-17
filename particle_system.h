#pragma once

#include "forms.h"

#define MAX_PARTICLES 10000

class Particle : public BasicForm
{
private:
	Vector velocity;
	Vector acceleration;
	float lifeSpan;
public:
	Particle(float life, Vector velo, Vector accel, Point pos, Color color);
	Vector getVelocity() { return velocity; }
	Vector getAcceleration() { return acceleration; }
	bool isDead() { return lifeSpan <= 0; }

	void update(float dt);
	virtual void renderSpecific() = 0;
};

template<typename T>
class ParticleSystem : public Form<T> 
{
private:
	float nextGenerationTimespan;
public:
	ParticleSystem(Point pos, Color color)
	{
		position = pos;
		col = color;
	}

	void renderSpecific()
	{
		GLUquadric *quad;
		quad = gluNewQuadric();
		{
			gluSphere(quad, 0.5, 20, 20);
		}
		gluDeleteQuadric(quad);
	}
	
	void update(float dt)
	{
		nextGenerationTimespan -= dt;
		if (nextGenerationTimespan <= 0)
		{
			generateParticles();
			nextGenerationTimespan = 0.1;
		}
		childs.erase(std::remove_if(childs.begin(), childs.end(), [](T* child) 
		{
			if (child->isDead())
			{
				delete child;
				return true;
			}
			return false;
		}), childs.end());
		Form<T>::update(dt);
	}

	virtual void generateParticles() = 0;
};

#define WIND_PARTICLE_LIFE 1
#define WIND_GRILL_Z_FACTOR 14
#define WIND_GRILL_Y_FACTOR WIND_GRILL_Z_FACTOR

class WindParticle : public Particle
{
public:
	WindParticle(Vector velocity, Vector accel, Point initialPosition) : Particle(WIND_PARTICLE_LIFE, velocity, accel, initialPosition, WHITE)
	{
		alpha = 0.8;
	}

	void renderSpecific();
};

class WindSystem : public ParticleSystem<WindParticle>
{
private:
	const int MAX_PARTICLE = WIND_PARTICLE_LIFE * 50;
	int currentParticle;
	float speed;
public:
	WindSystem(float windSpeed, Point position, Color color) : ParticleSystem(position, color)
	{
		speed = windSpeed;
	}

	void setWindSpeed(float windSpeed) { speed = windSpeed; }
	float getWindSpeed() { return speed; }

	void generateParticles();
	WindParticle* generateWindParticle();
};