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
		// Arri�re
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

Skybox3D::Skybox3D(Point c)
{
	position = c;

	textures = singleton<SkyboxManager>().loadData("");

	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}

void Skybox3D::renderSpecific()
{
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);

	//glDisable(GL_DEPTH_TEST);
	glScaled(1, 1, 1);
	//glDepthMask(GL_FALSE);
	// Avant

	float t = 50.0;
	glPushMatrix();
	glTranslatef(0, 50, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures->getTextureID());
	glBegin(GL_TRIANGLE_STRIP);			// X N�gatif		
	glTexCoord3f(-t, -t, -t); glVertex3f(-t, -t, -t);
	glTexCoord3f(-t, t, -t); glVertex3f(-t, t, -t);
	glTexCoord3f(-t, -t, t); glVertex3f(-t, -t, t);
	glTexCoord3f(-t, t, t); glVertex3f(-t, t, t);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// X Positif
	glTexCoord3f(t, -t, -t); glVertex3f(t, -t, -t);
	glTexCoord3f(t, -t, t); glVertex3f(t, -t, t);
	glTexCoord3f(t, t, -t); glVertex3f(t, t, -t);
	glTexCoord3f(t, t, t); glVertex3f(t, t, t);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// Y N�gatif	
	glTexCoord3f(-t, -t, -t); glVertex3f(-t, -t, -t);
	glTexCoord3f(-t, -t, t); glVertex3f(-t, -t, t);
	glTexCoord3f(t, -t, -t); glVertex3f(t, -t, -t);
	glTexCoord3f(t, -t, t); glVertex3f(t, -t, t);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// Y Positif		
	glTexCoord3f(-t, t, -t); glVertex3f(-t, t, -t);
	glTexCoord3f(t, t, -t); glVertex3f(t, t, -t);
	glTexCoord3f(-t, t, t); glVertex3f(-t, t, t);
	glTexCoord3f(t, t, t); glVertex3f(t, t, t);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// Z N�gatif	
	glTexCoord3f(-t, -t, -t); glVertex3f(-t, -t, -t);
	glTexCoord3f(t, -t, -t); glVertex3f(t, -t, -t);
	glTexCoord3f(-t, t, -t); glVertex3f(-t, t, -t);
	glTexCoord3f(t, t, -t); glVertex3f(t, t, -t);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// Z Positif	

	glTexCoord3f(-t, -t, t); glVertex3f(-t, -t, t);
	glTexCoord3f(-t, t, t); glVertex3f(-t, t, t);
	glTexCoord3f(t, -t, t); glVertex3f(t, -t, t);
	glTexCoord3f(t, t, t); glVertex3f(t, t, t);
	glEnd();
	glPopMatrix();

	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	/*glBegin(GL_QUADS);
	{
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		// Arri�re
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		// Droite
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		// Gauche
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		// Haut
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		// Bas
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
	}
	glDepthMask(GL_TRUE);*/
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

Pale::Pale(Point pos, Color cl) : Model3D("model/Pales.json", pos, cl)
{
	updateSpeed(25, M_PI/12);
}


void Pale::updateSpeed(double windSpeed, double attackAngle)
{
	const int NB_PALES = 1;
	double tanAttackAngle = tan(attackAngle);
	if (tanAttackAngle > 0)
	{
		const double rayonPale = 15;
		const double contrainteMeca = 15; // m/s

		double vitesseTangentielle = 0;//windSpeed / tan(attackAngle);
		double vitesseRotation = sqrt(pow(windSpeed, 2) + pow(vitesseTangentielle, 2));
	
		double radians = 2 * M_PI * vitesseRotation / rayonPale;
		double degrees = radians * 180 / M_PI;
		double tourmin = degrees * 60 / 360;
		printf("tanAttack=%f\nvitesseTan=%f\nvitesseRot=%f\nrad/s=%f\ndeg/s=%f\ntourmin=%f\n",
			tanAttackAngle, vitesseTangentielle, vitesseRotation, radians, degrees, tourmin);
		setAnim(Animation(anim.getCurrentAngle(), degrees / 100, Vector(1, 0, 0)));
	}
}

Eolienne::Eolienne(Point pos, Color cl) : Model3D("model/Mat.json", pos, cl)
{
	Form* nacelle = new Model3D("model/Tete.json", Point(0, 15, 0));
	Pale* pales = new Pale(Point(-2.8, -0.3, 0));
	nacelle->addChild(pales);
	addChild(nacelle);
}

Skybox::Skybox(Point pos) : Skybox3D(pos)
{

}