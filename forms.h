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

const int MAX_FORM_CHILDS = 20;


// Generic class to render and animate an object
class Form
{
protected:
	Point position;
	Color col;
	Animation anim;
	Form* childs[MAX_FORM_CHILDS];
private:
	int nextChildIndex()
	{
		for (int i = 0; i < MAX_FORM_CHILDS; i++)
		{
			if (childs[i] == NULL)
			{
				return i;
			}
		}
		return -1;
	}
public:
	Form()
	{
		for (int i = 0; i < MAX_FORM_CHILDS; i++)
		{
			childs[i] = NULL;
		}
	}
	Animation& getAnim() { return anim; }
	void setAnim(Animation ani) { anim = ani; }
	void addChild(Form* child)
	{
		int childIndex = nextChildIndex();
		if (childIndex == -1)
			return;
		childs[childIndex] = child;
	}
	void removeChild(Form* child)
	{
		unsigned int i;
		for (i = 0; i < MAX_FORM_CHILDS; i++)
		{
			if (childs[i] == child)
			{
				childs[i] = NULL;
			}
		}
	}
	void update()
	{
		// Mise à jour de l'animation
		anim.update();

		// Mise à jour des enfants, récursivement
		unsigned short i;
		for (i = 0; i < MAX_FORM_CHILDS; i++)
		{
			if (childs[i] != NULL)
			{
				childs[i]->update();
			}
		}
	}
	void render()
	{
		glPushMatrix();

		// Couleur de la forme
		glColor3f(col.r, col.g, col.b);

		// Déplacement sur position
		glTranslated(position.x, position.y, position.z);

		// Rotation locale
		glRotated(anim.getCurrentAngle(), anim.getRotVect().x, anim.getRotVect().y, anim.getRotVect().z);

		// Rendu de la forme
		renderSpecific();

		// On trace les enfants, recursivement
		unsigned short i;
		for (i = 0; i < MAX_FORM_CHILDS; i++)
		{
			if (childs[i] != NULL)
			{
				childs[i]->render();
			}
		}
		glPopMatrix();
	}
	virtual void renderSpecific() = 0;
};


// A particular Form
class Sphere : public Form
{
private:
	double radius;
public:
	Sphere(Point position = Point(), double r = 1.0, Color cl = Color());
	void renderSpecific();
};

class Cube : public Form
{
private:
	double width;
	double height;
	double depth;
public:
	Cube(Point position = Point(), double width = 10, double height = 10, double depth = 10, Color cl = Color());
	void renderSpecific(); 
};

class Model3D : public Form
{
private:
	Model* model;
public:
	Model3D(string file, Point position = Point(), Color cl = WHITE);
	void renderSpecific();
};

class Pale : public Model3D
{
public:
	Pale(Point pos = Point(), Color cl = WHITE);
	void updateSpeed(double windSpeed, double attackAngle);
};

class Eolienne : public Model3D
{
public:
	Eolienne(Point pos = Point(), Color cl = WHITE);
};

#endif // FORMS_H_INCLUDED
