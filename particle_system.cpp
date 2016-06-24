#include "particle_system.h"

Particle::Particle(float life, Vector velo, Vector accel, Point pos, Color color)
{
	lifeSpan = life;
	position = pos;
	col = color;
	velocity = velo;
	acceleration = accel;

	initialLifespan = life;
	initialPosition = pos;
}

/* Update pour chaque particule
*/
void Particle::update(float dt)
{
	velocity = velocity * acceleration;
	position = position + (velocity * dt);
	lifeSpan -= dt;
	//Les particules sont plus en plus en transparentes avec le temps
	alpha = lifeSpan / initialLifespan;
}

WindParticle::WindParticle(Vector velocity, Vector accel, Point initialPosition) 
	: Particle(WIND_PARTICLE_LIFE, velocity, accel, initialPosition, WHITE)
{
	alpha = 1;
}

void WindParticle::renderSpecific()
{

	GLUquadric *quad = gluNewQuadric();
	{
		gluSphere(quad, 0.3, 10, 10);
	}
	gluDeleteQuadric(quad);
}

WindSystem::WindSystem(Vector direction, float speed, Point position, Color color) 
	: ParticleSystem(WIND_MAX_PARTICLE, position, color)
{
	this->speed = speed;
	this->direction = direction;
	for (size_t i = 0; i < maxParticles; i++)
	{
		particles[i] = generateParticle();
	}
}

void WindSystem::renderSpecific()
{
	ParticleSystem::renderSpecific();
}

void WindSystem::respawnParticle(WindParticle* particle)
{
	const float maxPerLine = sqrt(WIND_MAX_PARTICLE);
	
	const float minX = 0;
	const float maxX = 0;
	const float dX = maxX - minX;
	const float xStep = dX / maxPerLine;

	const float minZ = -WIND_GRILL_HALF_WIDTH;
	const float maxZ = WIND_GRILL_HALF_WIDTH;
	const float dZ = maxZ - minZ;
	const float zStep = dZ / maxPerLine;

	const float minY = -WIND_GRILL_HALF_HEIGHT;
	const float maxY = +WIND_GRILL_HALF_HEIGHT;
	const float dY = maxY - minY;
	const float yStep = dY / maxPerLine;

	const float currentLine = currentParticle / maxPerLine;
	const float currentIndex = fmod(currentParticle, maxPerLine);

	const float currentX = minX + xStep * currentIndex;
	const float currentY = minY + yStep * currentLine;
	const float currentZ = minZ + zStep * currentIndex;

	currentParticle = fmod(currentParticle + 1, WIND_MAX_PARTICLE);
	float randomSpeed = 5 + fmod(rand(), 10 + (speed * 3));

	particle->setLifespan(WIND_PARTICLE_LIFE);
	particle->setVelocity(direction * randomSpeed);
	particle->setAcceleration(Vector(1, 1, 1));
	particle->setPosition(Point(currentX, currentY, currentZ));
}

WindParticle* WindSystem::generateParticle()
{
	return new WindParticle(
		direction,
		Vector(1, 1, 1),
		Point()
	);
}