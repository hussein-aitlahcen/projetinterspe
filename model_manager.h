#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include "picojson.h"
#include "SOIL\SOIL.h"

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

class SkyboxTextures
{
private:
	GLuint textureID;
	vector<const GLchar*> faces;

	int width, height;
	unsigned char* image;
	int channels;

	GLenum cube_map_target[6] = 
	{
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z
	};

public:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front) 
	// -Z (back)
	SkyboxTextures(string name)
	{
		faces.push_back("model/right.tga");
		faces.push_back("model/left.tga");
		faces.push_back("model/bottom.tga");
		faces.push_back("model/top.tga");
		faces.push_back("model/back.tga");
		faces.push_back("model/front.tga");

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		for (GLuint i = 0; i < faces.size(); i++)
		{

			image = SOIL_load_image(faces[i], &width, &height, &channels, SOIL_LOAD_RGB);
			glTexImage2D(cube_map_target[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}

	GLuint getTextureID()
	{
		return textureID;
	}
};

class Texture
{
private:
	GLuint textureID;
	int width, height;
	unsigned char* image;

public:
	Texture(string filename)
	{
		image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		SOIL_free_image_data(image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	GLuint getTextureID()
	{
		return textureID;
	}

};

class Shader
{
private:
	GLuint programID;

	string name, nameVertexShader, nameFragmentShader, nameGeometryShader, nameComputeShader;

	GLuint fShaderID, vShaderID, gShaderID, cShaderID;
	GLint successFShader, successVShader, successGShader, successCShader, successLink;
	GLchar infoLog[512];
public:
	Shader(string filename)
	{
		name = filename; 
		nameVertexShader = name + ".vertex";
		nameFragmentShader = name + ".fragment";
		nameGeometryShader = name + ".geometry";
		nameComputeShader = name + ".compute";

		std::stringstream vShaderStream, fShaderStream, gShaderStream, cShaderStream;
		ifstream fShaderFile = ifstream(nameFragmentShader.c_str(), ios::in);
		ifstream vShaderFile =  ifstream(nameVertexShader.c_str(), ios::in);
		ifstream gShaderFile = ifstream(nameGeometryShader.c_str(), ios::in);
		ifstream cShaderFile = ifstream(nameComputeShader.c_str(), ios::in);

		if (fShaderFile.is_open())
		{
			fShaderStream << fShaderFile.rdbuf();
			string result = fShaderStream.str();
			const GLchar* fShaderCode = result.c_str();


			fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fShaderID, 1, &fShaderCode, NULL);
			glCompileShader(fShaderID);

			glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &successFShader);

			if (!successFShader)
			{
				glGetShaderInfoLog(fShaderID, 512, NULL, infoLog);
				printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
			};
		}

		if (vShaderFile.is_open())
		{
			vShaderStream << vShaderFile.rdbuf();
			string result = vShaderStream.str();
			const GLchar* vShaderCode = result.c_str();


			vShaderID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vShaderID, 1, &vShaderCode, NULL);
			glCompileShader(vShaderID);

			glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &successVShader);

			if (!successVShader)
			{
				glGetShaderInfoLog(vShaderID, 512, NULL, infoLog);
				printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
			};
		}

		if (gShaderFile.is_open())
		{
			gShaderStream << gShaderFile.rdbuf();
			string result = gShaderStream.str();
			const GLchar* gShaderCode = result.c_str();


			gShaderID = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gShaderID, 1, &gShaderCode, NULL);
			glCompileShader(gShaderID);

			glGetShaderiv(gShaderID, GL_COMPILE_STATUS, &successGShader);

			if (!successGShader)
			{
				glGetShaderInfoLog(gShaderID, 512, NULL, infoLog);
				printf("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n%s", infoLog);
			};
		}

		if (cShaderFile.is_open())
		{
			cShaderStream << cShaderFile.rdbuf();
			string result = cShaderStream.str();
			const GLchar* cShaderCode = result.c_str();


			cShaderID = glCreateShader(GL_COMPUTE_SHADER);
			glShaderSource(cShaderID, 1, &cShaderCode, NULL);
			glCompileShader(cShaderID);

			glGetShaderiv(cShaderID, GL_COMPILE_STATUS, &successCShader);

			if (!successCShader)
			{
				glGetShaderInfoLog(cShaderID, 512, NULL, infoLog);
				printf("ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n%s", infoLog);
			};
		}

		programID = glCreateProgram();

		if(successFShader)
			glAttachShader(programID, fShaderID);

		if(successVShader)
			glAttachShader(programID, vShaderID);

		if(successGShader)
			glAttachShader(programID, gShaderID);

		if(successCShader)
			glAttachShader(programID, cShaderID);

		glLinkProgram(programID);

		glGetProgramiv(this->programID, GL_LINK_STATUS, &successLink);
		if (!successLink)
		{
			glGetProgramInfoLog(this->programID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vShaderID);
		glDeleteShader(fShaderID);
		glDeleteShader(gShaderID);
		glDeleteShader(cShaderID);

		fShaderFile.close();
		vShaderFile.close();
		gShaderFile.close();
		cShaderFile.close();
	}

	GLuint getProgramID()
	{
		return  programID;
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
			printf("loading new object - %s\n", name.c_str());
			T* model = new T(name);
			models[name] = model;
			return model;
		}
		printf("cached object - %s\n", name.c_str());
		return models.at(name);
	}
};

class ModelManager : public GenericManager<Model>
{
};

class TextureManager : public GenericManager<Texture>
{
};

class ShaderManager :  public GenericManager<Shader>
{
};

class SkyboxManager : public GenericManager<SkyboxTextures>
{
};