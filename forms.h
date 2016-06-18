#ifndef FORMS_H_INCLUDED
#define FORMS_H_INCLUDED

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include <gl/GL.h>
#include <glm/glm.hpp>
#include "geometry.h"
#include "animation.h"
#include "picojson.h"
#include "model_manager.h"

using namespace std;

class Color
{
public:
	float r, g, b;
	Color(float rr = 1.0f, float gg = 1.0f, float bb = 1.0f) { r = rr; g = gg; b = bb; }
};

// Constant Colors
const Color RED(1.0f, 0.0f, 0.0f);
const Color BLUE(0.0f, 0.0f, 1.0f);
const Color GREEN(0.0f, 1.0f, 0.0f);
const Color YELLOW(1.0f, 1.0f, 0.0f);
const Color WHITE(1.0f, 1.0f, 1.0f);

class Drawable
{
public:
	virtual void render() = 0;
	virtual void update(float dt) = 0;
};

// Generic class to render and animate an object
template<typename T>
class Form : public Drawable
{
protected:
	Point position;
	Color col;
	float alpha;
	Animation anim;
	vector<T*> childs;
public:
	Form() 
	{
		alpha = 1;
	}

	Animation& getAnim() { return anim; }
	void setAnim(Animation ani) { anim = ani; }
	Point getPosition() { return position; }
	void setPosition(Point p) { position = p; }

	void addChild(T* child)
	{
		childs.push_back(child);
	}
	void removeChild(T* child)
	{
		childs.erase(child);
	}
	void update(float dt)
	{
		// Mise à jour de l'animation
		anim.update(dt);

		// Mise à jour des enfants, récursivement
		for (auto element : childs) 
		{
			element->update(dt);
		}
	}
	void render()
	{
		glPushMatrix();

		// Couleur de la forme
		glColor4f(col.r, col.g, col.b, alpha);

		// Déplacement sur position
		glTranslated(position.x, position.y, position.z);

		// Rotation locale
		glRotated(anim.getCurrentAngle(), anim.getRotVect().x, anim.getRotVect().y, anim.getRotVect().z);

		// Rendu de la forme
		renderSpecific();

		// On trace les enfants, recursivement
		for (auto child : childs)
		{
			child->render();
		}
		
		glPopMatrix();
	}
	virtual void renderSpecific() = 0;
};

class BasicForm : public Form<BasicForm> { };


// A particular Form
class Sphere : public BasicForm
{
private:
	double radius;
public:
	Sphere(Point position = Point(), double r = 1.0, Color cl = Color());
	void renderSpecific();
};

class Cube : public BasicForm
{
private:
	double width;
	double height;
	double depth;
public:
	Cube(Point position = Point(), double width = 10, double height = 10, double depth = 10, Color cl = Color());
	void renderSpecific(); 
};

class Skybox3D : public BasicForm
{
private:
	vector<const GLchar*> faces;
	SkyboxTextures* textures;
	Point position;


	GLfloat skyboxVertices[108] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint skyboxVAO, skyboxVBO;


public:
	Skybox3D(Point position = Point());
	void renderSpecific();
};


class Model3D : public BasicForm
{
private:
	Model* model;
public:
	Model3D(string file, Point position = Point(), Color cl = WHITE);
	void renderSpecific();
};

class Pales : public Model3D
{
public:
	Pales(Point pos = Point(), Color cl = WHITE);
	void updateSpeed(double windSpeed, double attackAngle);
};

class Eolienne : public Model3D
{
private:
	Pales* pales;
public:
	Eolienne(Point pos = Point(), Color cl = WHITE);
	Pales* getPales() { return pales; }
};

class Skybox : public Skybox3D
{
public:
	Skybox(Point pos = Point());
};


#endif // FORMS_H_INCLUDED
