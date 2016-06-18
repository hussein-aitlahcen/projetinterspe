#include "particle_system.h"

Particle::Particle(float life, Vector velo, Vector accel, Point pos, Color color)
{
	lifeSpan = life;
	position = pos;
	col = color;
	velocity = velo;
	acceleration = accel;
}

void Particle::update(float dt)
{
	velocity = velocity * acceleration;
	position = position + (velocity * dt);
	lifeSpan -= dt;
}

void WindParticle::renderSpecific()
{
	GLUquadric *quad = gluNewQuadric();
	{
		gluSphere(quad, 0.2, 10, 10);
	}
	gluDeleteQuadric(quad);
}

WindSystem::WindSystem(Vector direction, float speed, Point position, Color color) : ParticleSystem(position, color)
{
	this->speed = speed;
	this->direction = direction;
}

void WindSystem::generateParticles()
{
	for (size_t i = 0; i < MAX_PARTICLE; i++)
	{
		addChild(generateWindParticle());
	}
}

WindParticle* WindSystem::generateWindParticle()
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

	float randomSpeed = fmod(rand(),  15 + (speed * 2));

	return new WindParticle(
		direction * randomSpeed,
		Vector(1, 1, 1), 
		Point(currentX, currentY, currentZ)
	);
}