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
	Particle(float life, Vector velo, Vector accel, Point pos, Color color) 
	{
		lifeSpan = life;
		position = pos;
		col = color;
		velocity = velo;
		acceleration = accel;
	}
	Vector getVelocity() { return velocity; }
	Vector getAcceleration() { return acceleration; }
	bool isDead() { return lifeSpan <= 0; }
	void update(float dt)
	{
		velocity = velocity * acceleration;
		position = position + (velocity * dt);
		lifeSpan -= dt;
	}
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

	virtual void generateParticles() = 0;

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
};

#define WIND_PARTICLE_LIFE 1
#define WIND_GRILL_Z_FACTOR 14
#define WIND_GRILL_Y_FACTOR WIND_GRILL_Z_FACTOR

class WindParticle : public Particle
{
public:
	WindParticle(Vector velocity, Vector accel, Point initialPosition) : Particle(WIND_PARTICLE_LIFE, velocity, accel, initialPosition, WHITE)
	{
		alpha = 0.5;
	}

	void renderSpecific()
	{
		GLUquadric *quad = gluNewQuadric();
		{
			gluSphere(quad, 0.2, 10, 10);
		}
		gluDeleteQuadric(quad);
	}
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

	void generateParticles()
	{
		for (size_t i = 0; i < MAX_PARTICLE; i++)
		{
			addChild(generateWindParticle());
		}
	}

	WindParticle* generateWindParticle()
	{
		const float maxPerLine = sqrt(MAX_PARTICLE);
		const float minZ = -WIND_GRILL_Z_FACTOR;
		const float maxZ = +WIND_GRILL_Z_FACTOR;
		const float dZ = maxZ - minZ;
		const float zStep = dZ / maxPerLine;
		const float minY = -WIND_GRILL_Y_FACTOR;
		const float maxY = +WIND_GRILL_Y_FACTOR;
		const float dY = maxY - minY;
		const float yStep = dY / maxPerLine;
		const float currentLine = currentParticle / maxPerLine;
		const float currentIndex = fmod(currentParticle, maxPerLine);
		const float currentX = 0;
		const float currentY = minY + yStep * currentLine;
		const float currentZ = minZ + zStep * currentIndex;

		currentParticle = fmod(currentParticle + 1, MAX_PARTICLE);

		return new WindParticle(Vector(fmod(rand(), (speed * 1.5)) + 5, 0, 0), Vector(1, 0, 0), Point(currentX, currentY, currentZ));
	}
};