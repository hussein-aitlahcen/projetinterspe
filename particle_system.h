#pragma once

#include "forms.h"

class Particle : public Form
{
private:
	Vector velocity;
	Vector acceleration;
public:
	Particle(Vector velo, Vector accel, Point pos, Color color) 
	{
		position = pos;
		col = color;
		velocity = velo;
		acceleration = accel;
	}
	Vector getVelocity() { return velocity; }
	Vector getAcceleration() { return acceleration; }
	void update(float dt)
	{
		velocity = velocity * acceleration * dt;
		position.x *= velocity.x;
		position.y *= velocity.y;
		position.z *= velocity.z;
	}
	virtual void renderSpecific() = 0;
};

class ParticleSystem : public Form 
{

};