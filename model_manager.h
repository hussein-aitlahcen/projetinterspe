#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "picojson.h"

using namespace std;

template <class X>
X& singleton()
{
	static X x;
	return x;
}

class Model
{
private:
	GLuint verticesBuffer;
	GLuint normalsBuffer;
	GLuint indicesBuffer;

	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLuint> indices;
	string name;

public:
	Model(string fileName) 
	{
		name = fileName;
		fstream fstream(fileName, ios::in);
		picojson::value model;
		picojson::parse(model, fstream);
		for (auto vertice : model.get("vertices").get(0).get("values").get<picojson::array>())
		{
			vertices.push_back(vertice.get<double>());
		}
		for (auto normal : model.get("vertices").get(1).get("values").get<picojson::array>())
		{
			normals.push_back(normal.get<double>());
		}
		for (auto indice : model.get("connectivity").get(0).get("indices").get<picojson::array>())
		{
			indices.push_back(indice.get<double>());
		}
		glGenBuffers(1, &verticesBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);

		glGenBuffers(1, &indicesBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices.front(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenBuffers(1, &normalsBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), &normals.front(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	GLuint getVerticesBuffer()
	{
		return verticesBuffer;
	}
	GLuint getNormalsBuffer()
	{
		return normalsBuffer;
	}
	GLuint getIndicesBuffer()
	{
		return indicesBuffer;
	}
	int getIndicesSize()
	{
		return indices.size();
	}
	string getName() 
	{
		return name;
	}
};


template<typename T>
class GenericManager
{
private:
	map<string, T*> models;

public:
	T* loadData(string name)
	{
		if (models.count(name) == 0)
		{
			T* model = new T(name);
			models[name] = model;
			return model;
		}
		return models.at(name);
	}
};

class ModelManager : public GenericManager<Model>
{
};