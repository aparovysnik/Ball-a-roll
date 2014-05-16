/******************************************************************************
Class:Mesh
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:Wrapper around OpenGL primitives, geometry and related 
OGL functions.

There's a couple of extra functions in here that you didn't get in the tutorial
series, to draw debug normals and tangents. 


-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../nclgl/OGLRenderer.h"
#include <vector>
#include <string>
#include <fstream>
#include "BoundingBox.h"

using std::ifstream;
using std::string;

//A handy enumerator, to determine which member of the bufferObject array
//holds which data
enum MeshBuffer {
	VERTEX_BUFFER	=0,
	COLOUR_BUFFER	=1, 
	NORMAL_BUFFER	=2,
	TEXTURE_BUFFER	,
	INDEX_BUFFER	,
	MAX_BUFFER
};
struct Light
{
	Light() {
		for (int i = 0; i < 10; i++)
		{
			colour.push_back(Vector4());
			position.push_back(Vector3());
			radius.push_back(0);
		}
		index = 0;
	}

	Light(vector<Vector4> c, vector<Vector3> p, vector<float> r)
	{
		colour = c;
		position = p;
		radius = r;
	}

	int AddLight(Vector4 c, Vector3 p, float r)
	{
		colour[index] = c;
		position[index] = p;
		radius[index] = r;
		
		if (index < 10)
		{
			index++; return (index - 1);
		}
		else
		{
			return index;
		}
	}
	vector<Vector4> GetColours() { return colour; }
	vector<Vector3> GetPositions() { return position; }
	vector<float> GetRadii() { return radius; }
	void RemoveLight(int i)
	{
		if (i < index)
		{
			position.erase(position.begin() + i);
			colour.erase(colour.begin() + i);
			radius.erase(radius.begin() + i);
		}
	}
private:
	int index = 0;
	vector<Vector4> colour;
	vector<Vector3> position;
	vector<float> radius;
};
class Mesh	{
public:
	Mesh(void);
	Mesh(const Mesh& m)
	{
		type = m.type;

		edges.resize(m.edges.size());
		for (int i = 0; i < m.edges.size(); i++)
		{
			edges[i] = new Vector3(*m.edges[i]);
		}
		arrayObject = m.arrayObject;
		std::copy(std::begin(m.bufferObject), std::end(m.bufferObject), std::begin(bufferObject));
		numVertices = m.numVertices;
		texture = m.texture;
		numIndices = m.numIndices;
		vertices = new Vector3(*m.vertices);
		colours = new Vector4(*m.colours);
		textureCoords = new Vector2(*m.textureCoords);
		indices = new unsigned int[numIndices];
		std::copy(m.indices, m.indices + m.numIndices, indices);
		if (m.normals != NULL)
			normals = new Vector3(*m.normals);
	}
	virtual ~Mesh(void);

	virtual void Draw();

	//Generates a single triangle, with RGB colours
	static Mesh*	GenerateTriangle();

	//void Add(Mesh* m);

	static Mesh*	LoadMeshFile(const string &filename);
	
	//Generates a single white quad, going from -1 to 1 on the x and z axis.
	static Mesh*	GenerateQuad();

	static Mesh*	GenerateQuadPatch();

	static Mesh*	GenerateQuadAlt();

	static Mesh* GenerateSphere(const float &radius, const unsigned int &rings, const unsigned int &sectors);

	static Mesh* GenerateStrip(const int& triNum, const Vector3 originA, const Vector3 originB, const Vector3 originC);

	//Sets the Mesh's diffuse map. Takes an OpenGL texture 'name'
	void	SetTexture(GLuint tex)	{texture = tex;}
	//Gets the Mesh's diffuse map. Returns an OpenGL texture 'name'
	GLuint  GetTexture()			{return texture;}

	static Mesh* GenerateQuad(Vector2 texCoord[4], Vector3 vert[4], Vector4 col[4]);
	static Mesh* GenerateQuad(Vector3 bottomLeft, Vector3 bottomRight, Vector3 topRight, Vector3 topLeft);

	GLuint	type;
	Vector3* GetNormals()
	{
		if (normals == NULL)
			GenerateNormals();
		return normals;
	}

	BoundingBox CalculateBoundingBox(Matrix4 transform);
	static BoundingBox CalculateBoundingBox(vector<Mesh*> meshes, Matrix4 transform);

	vector<Vector3> GetEdgePoints()
	{
		vector<Vector3> result;
		for (int i = 0; i < numVertices; i++)
			result.push_back(**(edges.begin() + i));
		return result;
	}

	vector<Vector3> GetEdgePoints(Matrix4 transform)
	{
		vector<Vector3> result;
		for (int i = 0; i < numVertices; i++)
			result.push_back(transform * **(edges.begin() + i));
		return result;
	}

	int AddLight(Vector4 color, Vector3 position, float radius)
	{
		return lighting.AddLight(color, position, radius);
	}
	
	void RemoveLight(int i)
	{
		lighting.RemoveLight(i);
	}

	Light GetLight() { return lighting; }

protected:

	//Edges of the generated object
	vector<Vector3*> edges;

	Light lighting;

	//Buffers all VBO data into graphics memory. Required before drawing!
	void	BufferData();


	//Generates normals for all facets. Assumes geometry type is GL_TRIANGLES...
	void	GenerateNormals();

	//Generates tangents for all facets. Assumes geometry type is GL_TRIANGLES...
	void	GenerateTangents();
	//Helper function for GenerateTangents
	Vector3 GenerateTangent(const Vector3 &a,const Vector3 &b,const Vector3 &c,const Vector2 &ta,const Vector2 &tb,const Vector2 &tc);

	//VAO for this mesh
	GLuint	arrayObject;
	//VBOs for this mesh
	GLuint	bufferObject[MAX_BUFFER];
	//Number of vertices for this mesh
	GLuint	numVertices;
	//Primitive type for this mesh (GL_TRIANGLES...etc)
	
	//OpenGL texture name for the diffuse map
	GLuint	texture;

	//Stuff introduced later on in the tutorials!!

	//Number of indices for this mesh
	GLuint			numIndices;

	//Pointer to vertex position attribute data (badly named...?)
	Vector3*		vertices;
	//Pointer to vertex colour attribute data
	Vector4*		colours;
	//Pointer to vertex texture coordinate attribute data
	Vector2*		textureCoords;
	//Pointer to vertex indices attribute data
	unsigned int*	indices;

	Vector3* normals;

	//Push sphere indices
	void push_indices(int sectors, int r, int s) {
		int curRow = r * sectors;
		int nextRow = (r + 1) * sectors;

		indices[(r * sectors) + (s*6)] = curRow + s;
		indices[(r * sectors) + (s * 6) + 1] = nextRow + s;
		indices[(r * sectors) + (s * 6) + 2] = nextRow + (s + 1);

		indices[(r * sectors) + (s * 6) + 5] = curRow + s;
		indices[(r * sectors) + (s * 6) + 3] = nextRow + (s + 1);
		indices[(r * sectors) + (s * 6) + 4] = curRow + (s + 1);
	}

	//Adds a triangle to an existing mesh
	static Mesh* AddTriangle(Mesh* m, const Vector3& a, const Vector3& b, const Vector3& c,
		const Vector2& t1, const Vector2& t2, const Vector2& t3, const int& index);
};

