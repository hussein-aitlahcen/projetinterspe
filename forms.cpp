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


Fleche::Fleche(Point c, double w, double h, double d, Color color)
{
	position = c;
	width = w;
	height = h;
	depth = d;
	Form::col = color;
}

void Fleche::renderSpecific()
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

double calculTimeFactor(double coupleVisqueux, double inertie, double simulationTime)
{
	return 1 - (exp(-coupleVisqueux / inertie * simulationTime));
}

double calculPuissanceVent(double surface, double windspeed)
{
	static double rhoAir = 1.225;
	return rhoAir * surface * pow(windspeed, 3) / 2;
}

double calculCoupleVent(double windspeed, double radius)
{
	return calculPuissanceVent(M_PI * pow(radius, 2), windspeed) / windspeed * radius;
}

double calculVitesseGeneratrice(double windspeed, double radius, double couplevisqueux, double coupleresistance,  double timeFactor, double factorK)
{
	return ((2 * (calculCoupleVent(windspeed, radius) - factorK * coupleresistance)) / (factorK * couplevisqueux))
		* timeFactor;
}

void Pales::update(float dt)
{
	Model3D::update(dt);

	totalSimulationTime += dt;

	static double inertieGeneratrice = 13.3; // Kg.m^-2
	static double coupleResGeneratrice = 104.4; // Nm
	static double coupleVisqueuxGeneratrice = 10; // Nm.s^-1
	static double k = 50; // multiplicateur 
	static double rayon = 11; // m
	static double rhoAir = 1.225; // Kg/m^3
	
	const double timeFactor = calculTimeFactor(coupleVisqueuxGeneratrice, inertieGeneratrice, totalSimulationTime - lastSimulationTime);
	const double vitesseGeneratriceT = calculVitesseGeneratrice(
		windSpeed,
		rayon,
		coupleVisqueuxGeneratrice,
		coupleResGeneratrice,
		timeFactor,
		k
	);
	const double vitesseGeneratriceMax = calculVitesseGeneratrice(
		windSpeed,
		rayon,
		coupleVisqueuxGeneratrice,
		coupleResGeneratrice,
		1,
		k
	) * abs(sin(getAttackAngle()));


	
	double vitesseT = vitesseGeneratriceT / k;
	double vitesseMax = vitesseGeneratriceMax / k;

	if (simulationNum > 1)
	{
		// vent plus fort
		if (windSpeed > lastWindSpeed || vitesseMax >= currentSpeed)
		{
			currentSpeed = min(currentSpeed + ((currentSpeed + 1) * 0.25 * dt), vitesseMax);
		}
		else if(windSpeed < lastWindSpeed || vitesseMax < currentSpeed)
		{
			currentSpeed = max(currentSpeed - (currentSpeed * 0.25 * dt), 0.0);
		}
	}
	else
	{
		currentSpeed = vitesseT;
	}
	
	const double degree = currentSpeed * 360 / 60.0 * dt;

	getAnim().setCurrentAngle(fmod(getAnim().getCurrentAngle() + degree, 360.0));
}


void Pales::updateSpeed(double windSpeed, double attackAngle)
{
	this->simulationNum++;
	this->lastSimulationTime = this->totalSimulationTime;
	this->lastSpeed = this->currentSpeed;
	this->lastWindSpeed = this->windSpeed;
	this->windSpeed = windSpeed;
	this->attackAngle = attackAngle;
}

Eolienne::Eolienne(Point pos, Color cl) : Model3D("model/Mat.json", pos, cl)
{
	stats = new EolienneStats(this);
	BasicForm* nacelle = new Model3D("model/Tete.json", Point(0, 15, 0));
	pales = new Pales(Point(-2.8, -0.3, 0));
	//Fleche* test = new Fleche(Point(0, 0, 0));
	//pales->addChild(test);
	nacelle->addChild(pales);
	addChild(nacelle);
}

Skybox::Skybox(Point pos) : Skybox3D(pos)
{
}

EolienneStats::EolienneStats(Eolienne * eolienne)
{
	parent = eolienne;
	col = WHITE;
}

void EolienneStats::renderSpecific()
{
	Pales* pales = parent->getPales(); 
	const Vector position(3.6,-4.3,0);


	const float a = 0.5;
	const float b = 0.2;

	glDepthFunc(GL_ALWAYS);
	glDisable(GL_LIGHTING);

	glTranslated(position.x, position.y, position.z);
	glRotated(-45,1,0 , 0);

	glColor4f(0, pales->getWindSpeed() * 0.04 , 1- pales->getWindSpeed() * 0.04, 0.7);
	glBegin(GL_TRIANGLE_STRIP);
	{
		// vitesse vent
		glVertex3f(-a, 0, 0);
		glVertex3f(-a, pales->getWindSpeed() * 1.5/40, 0);
		glVertex3f(-b, 0, 0);
		glVertex3f(-b, pales->getWindSpeed() * 1.5/40, 0);
	}
	glEnd();

	glColor4f(1, 1 - pales->getCurrentSpeed() * 0.003, 0, 0.7);
	glBegin(GL_TRIANGLE_STRIP);
	{
		// vitesse turbine
		glVertex3f(a, 0, 0);
		glVertex3f(a, pales->getCurrentSpeed() * 1.5/327, 0);
		glVertex3f(b, 0, 0);
		glVertex3f(b, pales->getCurrentSpeed() * 1.5/327, 0);
	}
	glEnd();
	glRotated(45, 1, 0, 0);
	glTranslated(-position.x, -position.y, -position.z);

	glEnable(GL_LIGHTING);
	glDepthFunc(GL_LESS);
}