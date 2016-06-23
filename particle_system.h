#pragma once

#include "forms.h"

// max particle by system
#define MAX_PARTICLES 10000

// 0.1 sec 100 ms
#define PARTICLE_GENERATION_INTERVAL 0.1

// wind system spec
#define WIND_PARTICLE_LIFE 1 // in second
#define WIND_GRILL_HALF_WIDTH 14 // 14*14 grill
#define WIND_GRILL_HALF_HEIGHT WIND_GRILL_HALF_WIDTH
#define WIND_MAX_PARTICLE WIND_PARTICLE_LIFE * 100

class Particle : public BasicForm
{
protected:
	Vector velocity;
	Vector acceleration;
	float lifeSpan;

	float initialLifespan;
	Point initialPosition;
public:
	Particle(float life, Vector velo, Vector accel, Point pos, Color color);
	Vector getVelocity() { return velocity; }
	void setVelocity(Vector v) { velocity = v; }
	Vector getAcceleration() { return acceleration; }
	void setAcceleration(Vector accel) { acceleration = accel; }
	Point getInitialPosition() { return initialPosition; }
	float getLifespan() { return lifeSpan; }
	void setLifespan(float life) { lifeSpan = life; }

	bool isDead() { return lifeSpan <= 0; }

	void update(float dt);
	virtual void renderSpecific() = 0;
};

template<typename T>
class ParticleSystem : public Form<T> 
{
protected:
	T** particles;
	size_t maxParticles;
	size_t lastParticle;
	size_t particlePerGen;
	float nextGenerationTimespan;

public:

	ParticleSystem(size_t maxPerGen, Point pos, Color color)
	{
		maxParticles = maxPerGen * (1 / PARTICLE_GENERATION_INTERVAL);
		position = pos;
		col = color;
		particlePerGen = maxPerGen;
		particles = new T*[maxParticles];
	}

	void renderSpecific()
	{
		GLUquadric *quad;
		quad = gluNewQuadric();
		{
			gluSphere(quad, 0.5, 20, 20);
		}
		gluDeleteQuadric(quad);
		for (size_t i = 0; i < maxParticles; i++)
		{
			if (!particles[i]->isDead())
			{
				particles[i]->render();
			}
		}
	}
	
	size_t getDeadParticle()
	{
		size_t i;
		for (i = lastParticle; i < maxParticles; i++) {
			if (particles[i]->isDead()) {
				lastParticle = i;
				return i;
			}
		}

		for (i = 0; i < lastParticle; i++) {
			if (particles[i]->isDead()) {
				lastParticle = i;
				return i;
			}
		}
		return -1;
	}

	void update(float dt)
	{
		size_t i = 0;
		nextGenerationTimespan -= dt;
		if (nextGenerationTimespan <= 0)
		{
			size_t deadParticle;
			while ((deadParticle = getDeadParticle()) != -1 && (i < particlePerGen))
			{
				respawnParticle(particles[deadParticle]);
				i++;
			}
			nextGenerationTimespan = PARTICLE_GENERATION_INTERVAL;
		}
		for (i = 0; i < maxParticles; i++)
		{
			if (!particles[i]->isDead())
			{
				particles[i]->update(dt);
			}
		}
	}

	virtual void respawnParticle(T* particle) = 0;
};

class WindParticle : public Particle
{
public:
	WindParticle(Vector velocity, Vector accel, Point initialPosition);
	void renderSpecific();
};

class WindSystem : public ParticleSystem<WindParticle>
{
private:
	int currentParticle;
	float speed;
	Vector direction;
public:
	WindSystem(Vector direction, float speed, Point position, Color color);
	void setSpeed(float windSpeed) { speed = windSpeed; }
	float getSpeed() { return speed; }
	void setDirection(Vector direction) { this->direction = direction; }
	Vector getDirection() { return direction; }
	float getAngleFactor() { return atan(direction.x / direction.z); }
	float getAngle() 
	{
		return getAngleFactor() * 180 / M_PI;
	}

	void renderSpecific();
	void respawnParticle(WindParticle* particle);
	WindParticle* generateParticle();
};