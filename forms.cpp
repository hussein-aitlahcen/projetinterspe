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

Cylinder::Cylinder(Point c, double h, double r, Color cl)
{
	position = c;
	height = h;
	radius = r;
	col = cl;
}

void Cylinder::renderSpecific()
{
	GLUquadric *quad, *disk1, *disk2;
	quad = gluNewQuadric();
	disk1 = gluNewQuadric();
	disk2 = gluNewQuadric();
	{
		gluCylinder(quad, radius, radius, height, 20, 20);
		gluDisk(disk1, 0, radius, 20, 20);
		glTranslated(0, 0, height);
		gluDisk(disk2, 0, radius, 20, 20);
		glTranslated(0, 0, -height);
	}
	gluDeleteQuadric(disk1);
	gluDeleteQuadric(disk2);
	gluDeleteQuadric(quad);
}

Skybox3D::Skybox3D(Point c)
{
	position = c;

	texturesSkybox = singleton<SkyboxManager>().loadData("const_skybox");
	//textureSol = singleton<TextureManager>().loadData("model/grass2.jpg");
}

void Skybox3D::renderSpecific()
{

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	//glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	// Avant

	float t = 200;
	float h = 4;
	glTranslatef(0, t/h, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texturesSkybox->getTextureID());
	glBegin(GL_TRIANGLE_STRIP);			// X Négatif		
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


	glBegin(GL_TRIANGLE_STRIP);			// Y Négatif	
	glTexCoord3f(-t, -t, -t); glVertex3f(-t, -t / h, -t);
	glTexCoord3f(-t, -t, t); glVertex3f(-t, -t / h, t);
	glTexCoord3f(t, -t, -t); glVertex3f(t, -t / h, -t);
	glTexCoord3f(t, -t, t); glVertex3f(t, -t / h, t);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// Y Positif		
	glTexCoord3f(-t, t, -t); glVertex3f(-t, t, -t);
	glTexCoord3f(t, t, -t); glVertex3f(t, t, -t);
	glTexCoord3f(-t, t, t); glVertex3f(-t, t, t);
	glTexCoord3f(t, t, t); glVertex3f(t, t, t);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// Z Négatif	
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

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);

	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureSol->getTextureID());


	glBegin(GL_TRIANGLE_STRIP);			// Y Négatif	
	glTexCoord3f(-t , -t, -t); glVertex3f(-t, -t / 3.5, -t);
	glTexCoord3f(-t, -t, t); glVertex3f(-t, -t / 3.5, t);
	glTexCoord3f(t, -t, -t); glVertex3f(t, -t / 3.5, -t);
	glTexCoord3f(t, -t, t); glVertex3f(t, -t / 3.5, t);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);*/
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
		getAnim().setAngle(degrees);
	}
}

Eolienne::Eolienne(Point pos, Color cl) : Model3D("model/Mat.json", pos, cl)
{
	BasicForm* nacelle = new Model3D("model/Tete.json", Point(0, 15, 0));
	pales = new Pales(Point(-2.8, -0.3, 0));
	nacelle->addChild(pales);
	addChild(nacelle);
}

Skybox::Skybox(Point pos) : Skybox3D(pos)
{
}

AirHokey::AirHokey(Point pos, Color color)
{
	position = pos;
	col = color;
	setAnim(Animation(90, 0, Vector(1, 0, 0)));
	x = new Cube(Point(-10, 0, 0), 1, 20, 1, WHITE);
	maxX = new Cube(Point(10, 0, 0), 1, 20, 1, WHITE);
	y = new Cube(Point(0, -10, 0), 20, 1, 1, WHITE);
	maxY = new Cube(Point(0, 10, 0), 20, 1, 1, WHITE);
	palet = new Cylinder(Point(), 0.2, 0.5, WHITE);
	palet->setAnim(Animation(180, 0, Vector(1, 0, 0)));
	direction = Vector(1, 0.5, 0);
	velocite = Vector(25, 25, 0);
	addChild(x);
	addChild(maxX);
	addChild(y);
	addChild(maxY);
	addChild(palet);
}

void AirHokey::renderSpecific()
{
}

Vector calcul_rebond(Vector vi, Vector normal)
{
	return vi + (vi * normal * normal * -2);
}

int collision(float rayon, float x, float maxX, float y, float maxY, Point position)
{
	int colX = position.x + rayon;
	if (colX >= maxX) return 3;
	if (colX <= x) return 1;
	int colY = position.y + rayon;
	if (colY >= maxY) return 4;
	if (colY <= y) return 2;
	return 0;
}


void AirHokey::update(float dt)
{
	palet->setPosition(palet->getPosition() + (direction * velocite * dt));
	int bordCollision = collision(0.5, -10, 10, -10, 10, palet->getPosition());
	if (bordCollision != 0)
	{
		//printf("collision %d\n", bordCollision);
		Vector normale;
		switch (bordCollision)
		{
		case 1:
			normale = Vector(1, 0, 0);
			break;
		case 2:
			normale = Vector(0, 1, 0);
			break;
		case 3:
			normale = Vector(-1, 0, 0);
			break;
		case 4:
			normale = Vector(0, -1, 0);
			break;
		}
		direction = calcul_rebond(direction, normale);
	}
}
