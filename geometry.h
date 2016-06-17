#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

class Coordinates
{
public:
	double x, y, z;
	Coordinates(double xx = 0, double yy = 0, double zz = 0) { x = xx; y = yy; z = zz; }
};


class Point : public Coordinates
{
public:
	// Point constructor calls the base class constructor and do nothing more
	Point(double xx = 0, double yy = 0, double zz = 0) : Coordinates(xx, yy, zz) {}
};

class Vector : public Coordinates
{
public:
	// Instantiates a Vector from its coordinates
	Vector(double xx = 0, double yy = 0, double zz = 0) : Coordinates(xx, yy, zz) {}
	// Or with two points
	Vector(Point, Point);
	// Compute the vector norm
	double norm();
	Vector operator+(Coordinates& b)
	{
		return Vector(x + b.x, y + b.y, z + b.z);
	}
	Vector operator*(Coordinates& b)
	{
		return Vector(x * b.x, y * b.y, z * b.z);
	}
	Vector operator*(double factor)
	{
		return Vector(x * factor, y * factor, z * factor);
	}
};


// Compute the distance between two points
double distance(Point p1, Point p2);

#endif // GEOMETRY_H_INCLUDED
