#include "forms.h"

Sphere::Sphere(Point c, double r, Color cl)
{
	position = c;
	radius = r;
	col = cl;
}

void Sphere::renderSpecific()
{
	GLUquadric *quad;
	quad = gluNewQuadric();
	{
		gluSphere(quad, radius, 20, 20);
	}
	gluDeleteQuadric(quad);
}

Cube::Cube(Point c, double w, double h, double d, Color color)
{
	position = c;
	width = w;
	height = h;
	depth = d;
	Form::col = color;
}

void Cube::renderSpecific()
{
	const float half_w = width / 2;
	const float half_h = height / 2;
	const float half_d = depth / 2;
	glBegin(GL_QUADS);
	{
		// Avant
		glVertex3f(-half_w, -half_h, half_d);
		glVertex3f(half_w, -half_h, half_d);
		glVertex3f(half_w, half_h, half_d);
		glVertex3f(-half_w, half_h, half_d);
		// Arrière
		glVertex3f(-half_w, -half_h, -half_d);
		glVertex3f(half_w, -half_h, -half_d);
		glVertex3f(half_w, half_h, -half_d);
		glVertex3f(-half_w, half_h, -half_d);
		// Droite
		glVertex3f(half_w, -half_h, half_d);
		glVertex3f(half_w, -half_h, -half_d);
		glVertex3f(half_w, half_h, -half_d);
		glVertex3f(half_w, half_h, half_d);
		// Gauche
		glVertex3f(-half_w, -half_h, half_d);
		glVertex3f(-half_w, -half_h, -half_d);
		glVertex3f(-half_w, half_h, -half_d);
		glVertex3f(-half_w, half_h, half_d);
		// Haut
		glVertex3f(-half_w, half_h, half_d);
		glVertex3f(half_w, half_h, half_d);
		glVertex3f(half_w, half_h, -half_d);
		glVertex3f(-half_w, half_h, -half_d);
		// Bas
		glVertex3f(-half_w, -half_h, half_d);
		glVertex3f(half_w, -half_h, half_d);
		glVertex3f(half_w, -half_h, -half_d);
		glVertex3f(-half_w, -half_h, -half_d);
	}
	glEnd();
}

Model3D::Model3D(string file, Point pos, Color color)
{
	position = pos;
	col = color;
	model = singleton<ModelManager>().loadData(file);
}

void Model3D::renderSpecific()
{
	glBindBuffer(GL_ARRAY_BUFFER, model->getNormalsBuffer());
	glNormalPointer(GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, model->getVerticesBuffer());
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getIndicesBuffer());
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDrawElements(GL_TRIANGLES, model->getIndicesSize(), GL_UNSIGNED_INT, 0); 
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

Pales::Pales(Point pos, Color cl) : Model3D("model/Pales.json", pos, cl)
{
}


void Pales::updateSpeed(double windSpeed, double attackAngle)
{
	if (attackAngle > 0)
	{
		const double rayonPale = 11;
		const double rho = 1.225;
		const double inertiePale = 133000;

		double masseAirApplique = rho * M_PI * pow(rayonPale, 2) * windSpeed * sin(attackAngle);
		double energieCinetiqueAir = masseAirApplique * pow(windSpeed, 2);
		
	
		double radians = sqrt(energieCinetiqueAir / inertiePale);
		double degrees = radians * 180 / M_PI;
		double tourmin = degrees * 60 / 360;
		printf("masseAirApplique = %f\nenergieCinetique = %f\n radians = %f\n tour/min = %f\n",
			masseAirApplique, energieCinetiqueAir, radians, tourmin);
		setAnim(Animation(anim.getCurrentAngle(), degrees / 100, Vector(1, 0, 0)));
	}
}

Eolienne::Eolienne(Point pos, Color cl) : Model3D("model/Mat.json", pos, cl)
{
	BasicForm* nacelle = new Model3D("model/Tete.json", Point(0, 15, 0));
	pales = new Pales(Point(-2.8, -0.3, 0));
	nacelle->addChild(pales);
	addChild(nacelle);
}
