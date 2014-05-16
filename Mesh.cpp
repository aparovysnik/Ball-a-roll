#include "Mesh.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <list>

Mesh::Mesh(void)	{
	glGenVertexArrays(1, &arrayObject);
	
	for(int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}

	texture		 = 0;
	numVertices  = 0;
	type		 = GL_TRIANGLES;

	//Later tutorial stuff
	numIndices    = 0;
	vertices	  = NULL;
	textureCoords = NULL;
	indices		  = NULL;
	normals		  = NULL;
	colours		  = NULL;
}

Mesh::~Mesh(void)	{
	glDeleteVertexArrays(1, &arrayObject);			//Delete our VAO
	glDeleteBuffers(MAX_BUFFER, bufferObject);		//Delete our VBOs

	glDeleteTextures(1,&texture);					//We'll be nice and delete our texture when we're done with it

	//Later tutorial stuff
	delete[]vertices;
	delete[]indices;
	delete[]textureCoords;
	delete[]colours;
}

void Mesh::Draw()	{
		glBindVertexArray(arrayObject);
		//std::cout << "Drawing! " << numVertices << std::endl;
		if(bufferObject[INDEX_BUFFER]) {
			glDrawElements(type, numIndices, GL_UNSIGNED_INT, NULL);
		}
		else{
			glDrawArrays(type, 0, numVertices);	//Draw the triangle!
		}
		glBindVertexArray(0);	

	glBindTexture(GL_TEXTURE_2D, 0);
}

Mesh* Mesh::GenerateTriangle()	{
	Mesh*m = new Mesh();
	m->numVertices = 3;

	m->vertices = new Vector3[m->numVertices];
	m->vertices[0] = Vector3(0.0f,	0.5f,	0.0f);
	m->vertices[1] = Vector3(0.5f,  -0.5f,	0.0f);
	m->vertices[2] = Vector3(-0.5f, -0.5f,	0.0f);

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.5f,	0.0f);
	m->textureCoords[1] = Vector2(1.0f,	1.0f);
	m->textureCoords[2] = Vector2(0.0f,	1.0f);

	m->colours = new Vector4[m->numVertices];
	m->colours[0] = Vector4(1.0f, 0.0f, 0.0f,1.0f); 
	m->colours[1] = Vector4(0.0f, 1.0f, 0.0f,1.0f);
	m->colours[2] = Vector4(0.0f, 0.0f, 1.0f,1.0f);

	m->GenerateNormals();
	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuadPatch()	{
	Mesh* m			= new Mesh();

	m->numVertices	= 4;
	m->type			= GL_PATCHES;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];


	m->vertices[0] = 	Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = 	Vector3(-1.0f,	1.0f, 0.0f);
	m->vertices[2] = 	Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = 	Vector3(1.0f,  1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f,	1.0f);
	m->textureCoords[1] = Vector2(0.0f,	0.0f);
	m->textureCoords[2] = Vector2(1.0f,	1.0f);
	m->textureCoords[3] = Vector2(1.0f,	0.0f);

	m->GenerateNormals();
	m->BufferData();

	return m;
}

Mesh*	Mesh::LoadMeshFile(const string &filename) {
	ifstream f(filename);

	if(!f) {
		return NULL;
	}

	Mesh*m = new Mesh();
	f >> m->numVertices;

	int hasTex = 0;
	int hasColour = 0;

	f >> hasTex;
	f >> hasColour;

	m->vertices = new Vector3[m->numVertices];

	if(hasTex) {
		m->textureCoords = new Vector2[m->numVertices];
		m->colours		 = new Vector4[m->numVertices];
	}

	for(int i = 0; i << m->numVertices; ++i) {
		f >> m->vertices[i].x;
		f >> m->vertices[i].y;
		f >> m->vertices[i].z;

		if(hasTex) {
			f >> m->textureCoords[i].x;
			f >> m->textureCoords[i].y;
		}

		if(hasColour) {
			char r,g,b,a;

			f >> r;
			f >> g;
			f >> b;
			f >> a;

			m->colours[i] = Vector4(r / 255.0f, g / 255.0f,b / 255.0f,a / 255.0f);
		}
	}
	return m;
}
Mesh* Mesh::GenerateQuad(Vector3 bottomLeft, Vector3 bottomRight, Vector3 topRight, Vector3 topLeft)
{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];

	m->vertices[0] = bottomLeft;
	m->vertices[1] = topLeft;
	m->vertices[2] = bottomRight;
	m->vertices[3] = topRight;

	m->edges.push_back(&bottomLeft);
	m->edges.push_back(&bottomRight);
	m->edges.push_back(&topRight);
	m->edges.push_back(&topLeft);

	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);

	for (int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 0.5f);
	}

	m->GenerateNormals();
	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuad()	{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];
	m->colours			= new Vector4[m->numVertices];

	m->vertices[0] = 	Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = 	Vector3(-1.0f,	1.0f, 0.0f);
	m->vertices[2] = 	Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = 	Vector3(1.0f,  1.0f, 0.0f);

	m->edges.push_back(&m->vertices[0]);
	m->edges.push_back(&m->vertices[2]);
	m->edges.push_back(&m->vertices[3]);
	m->edges.push_back(&m->vertices[1]);

	m->textureCoords[0] = Vector2(0.0f,	1.0f);
	m->textureCoords[1] = Vector2(0.0f,	0.0f);
	m->textureCoords[2] = Vector2(1.0f,	1.0f);
	m->textureCoords[3] = Vector2(1.0f,	0.0f);

	for(int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f,1.0f,0.7f);
	}

	m->GenerateNormals();
	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuad(Vector2 texCoord[4], Vector3 vert[4], Vector4 col[4])	{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];

	m->vertices[0] = vert[0];
	m->vertices[1] = vert[1];
	m->vertices[2] = vert[2];
	m->vertices[3] = vert[3];

	m->textureCoords[0] = texCoord[0];
	m->textureCoords[1] = texCoord[1];
	m->textureCoords[2] = texCoord[2];
	m->textureCoords[3] = texCoord[3];

	m->colours[0] = col[0];
	m->colours[1] = col[1];
	m->colours[2] = col[2];
	m->colours[3] = col[3];

	m->GenerateNormals();
	m->BufferData();

	return m;
}



Mesh* Mesh::AddTriangle(Mesh* m, const Vector3& a, const Vector3& b, const Vector3& c,
	const Vector2& t1, const Vector2& t2, const Vector2& t3, const int& index)
{
	int max = m->numVertices;

	if (index > max - 2)
	{
		return m;
	}

	m->vertices[index] = a;
	m->vertices[index+1] = b;
	m->vertices[index+2] = c;

	m->colours[index] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m->colours[index + 1] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m->colours[index + 2] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	
	m->textureCoords[index] = t1;
	m->textureCoords[index + 1] = t2;
	m->textureCoords[index + 2] = t3;

	std::cout << "Added triangle between " << a.x << "x " << a.y << "y " << a.z << "z, " << b.x << "x " << b.y << "y " << b.z << "z and" << c.x << "x " << c.y << "y " << c.z << std::endl;

	return m;
}

Mesh* Mesh::GenerateStrip(const int& triNum, const Vector3 originA, const Vector3 originB, const Vector3 originC)
{
	Mesh* m = new Mesh();

	m->numVertices = 2 + triNum ;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];

	int vertex;
	std::list<Vector3> coord = { originA, originB, originC };

	m->type = GL_TRIANGLE_STRIP;
	m->vertices[0] = originA;
	m->vertices[1] = originB;
	Vector3 front;
	Vector3 middle;
	Vector3 back;
	m->textureCoords[0] = Vector2(0.0f, 0.0f);
	m->textureCoords[1] = Vector2(1.0 / triNum, 0.0f);
	m->textureCoords[2] = Vector2((1.0 / triNum) * ((originC.x - originA.x) / (originB.x - originA.x)), 1.0f);
	for (int i = 3; i < triNum; i++)
	{
		front = coord.front();
		coord.pop_front();
		middle = coord.front();
		back = coord.back();
		if (i % 2)
		{
			coord.push_back(Vector3(back.x + (middle.x - front.x), back.y + (middle.y - front.y), back.z + (middle.z - front.z)));
			m->vertices[i] = coord.back();
			m->textureCoords[i] = m->textureCoords[i - 1] + Vector2((1.0 / triNum), 1.f);
		}
		else
		{
			coord.push_back(Vector3(front.x + (back.x - middle.x), front.y + (back.y - middle.y), front.z + (back.z - middle.z)));
			m->vertices[i] = coord.back();
			m->textureCoords[i] = m->textureCoords[i - 3] + Vector2((1.0 / triNum), 0.f);
		}
	}
	return m;
}


Mesh* Mesh::GenerateQuadAlt()	{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];
	m->colours			= new Vector4[m->numVertices];

	m->vertices[0] = 	Vector3(0.0f, 0.0f, 0.0f);
	m->vertices[1] = 	Vector3(0.0f, 1.0f, 0.0f);
	m->vertices[2] = 	Vector3(1.0f, 0.0f, 0.0f);
	m->vertices[3] = 	Vector3(1.0f,  1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f,	0.0f);
	m->textureCoords[1] = Vector2(0.0f,	1.0f);
	m->textureCoords[2] = Vector2(1.0f,	0.0f);
	m->textureCoords[3] = Vector2(1.0f,	1.0f);

	for(int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f,1.0f,1.0f);
	}

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateSphere(const float &radius, const unsigned int &rings, const unsigned int &sectors)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;
	Mesh* m = new Mesh();

	m->type = GL_TRIANGLES;
	m->numVertices = rings * sectors * 3;
	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];

	Vector2 *t = m->textureCoords;
	Vector3 *v = m->vertices;
	Vector4 *c = m->colours;

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const y = sin(-M_PI_2 + M_PI * r * R); // y
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R); // x
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R); // z

			m->textureCoords[r * sectors + s] = Vector2(s*S, r*R);

			m->vertices[r * sectors + s] = Vector3(x * radius, y * radius, z * radius);
			m->edges.push_back(&m->vertices[r * sectors + s]);

			m->colours[r * sectors + s] = Vector4(x, y, z, 255);
		}
	}
	m->numIndices = (rings - 1) * (sectors - 1) * 6;
	m->indices = new unsigned int[(rings - 1) * (sectors - 1) * 6];
	for (r = 0; r < rings - 1; r++)
	{
		for (s = 0; s < sectors - 1; s++)
		{
			int test = r * (sectors - 1) * 6 + s * 6;
			m->indices[test + 0] = r * sectors + s;
			m->indices[test + 1] = (r + 1) * sectors + s;
			m->indices[test + 2] = (r + 1) * sectors + (s + 1);

			m->indices[test + 3] = r * sectors + s;
			m->indices[test + 4] = (r + 1) * sectors + (s + 1);
			m->indices[test + 5] = r * sectors + (s + 1);
		}
	}
	m->GenerateNormals();
	m->BufferData();

	return m;
}

void	Mesh::BufferData()	{
	glBindVertexArray(arrayObject);

	//Buffer vertex data
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(VERTEX_BUFFER);

	////Buffer texture data
	if(textureCoords) {
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector2), textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	//buffer colour data
	if (colours)	{
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector4), colours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}

	//buffer normal data
	if (normals)	{
		glGenBuffers(1, &bufferObject[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}

	//buffer index data
	if(indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

void Mesh::GenerateNormals()
{
	if (!normals)
	{
		normals = new Vector3[numVertices];
	}
	switch (type)
	{
	case(GL_TRIANGLES) :
	{

						   for (unsigned int i = 0; i < numVertices; i += 3)
						   {
							   Vector3 &a = vertices[i];
							   Vector3 &b = vertices[i + 1];
							   Vector3 &c = vertices[i + 2];

							   Vector3 normal = Vector3::Cross(b - a, c - a);
							   normal.Normalise();

							   normals[i] = normal;
							   normals[i + 1] = normal;
							   normals[i + 2] = normal;
						   }
						   break;
	}
	case(GL_TRIANGLE_STRIP) :
	{
								for (unsigned int i = 0; i < numVertices - 2; i++)
								{
									Vector3 &a = vertices[i];
									Vector3 &b = vertices[i + 1];
									Vector3 &c = vertices[i + 2];

									Vector3 normal = Vector3::Cross(b - a, c - a);
									normal.Normalise();

									normals[i] = normal;
									normals[i + 1] = normal;
									normals[i + 2] = normal;
								}
								break;
	}
	}
}

BoundingBox Mesh::CalculateBoundingBox(Matrix4 transform)
{
	if (numVertices > 0)
	{
		Vector3 vertex = vertices[0];
		vertex = transform * vertex;
		float minX = vertex.x, maxX = vertex.x;
		float minY = vertex.y, maxY = vertex.y;
		float minZ = vertex.z, maxZ = vertex.z;
		for (int i = 1; i < numVertices; i++)
		{
			vertex = vertices[i];
			vertex = transform * vertex;
			if (vertex.x < minX)
				minX = vertex.x;
			else if (vertex.x > maxX)
				maxX = vertex.x;
			if (vertex.y < minY)
				minY = vertex.y;
			else if (vertex.y > maxY)
				maxY = vertex.y;
			if (vertex.z < minZ)
				minZ = vertex.z;
			else if (vertex.z > maxZ)
				maxZ = vertex.z;
		}
		return BoundingBox(Vector3(minX, minY, minZ), Vector3(maxX, maxY, maxZ));
	}
	return BoundingBox(Vector3(0, 0, 0), Vector3(0, 0, 0));
}

BoundingBox Mesh::CalculateBoundingBox(vector<Mesh*> meshes, Matrix4 transform)
{
	//Overall minimum and maximum values
	float ovMinX = FLT_MAX, ovMaxX = FLT_MIN;
	float ovMinY = FLT_MAX, ovMaxY = FLT_MIN;
	float ovMinZ = FLT_MAX, ovMaxZ = FLT_MIN;

	bool hasVertices = false;
	for each(Mesh* mesh in meshes)
	{
		if (mesh->numVertices > 0)
		{
			hasVertices = true;
			Vector3 vertex = mesh->vertices[0];
			vertex = transform * vertex;
			float minX = vertex.x, maxX = vertex.x;
			float minY = vertex.y, maxY = vertex.y;
			float minZ = vertex.z, maxZ = vertex.z;
			for (int i = 1; i < mesh->numVertices; i++)
			{
				vertex = mesh->vertices[i];
				vertex = transform * vertex;
				if (vertex.x < minX)
					minX = vertex.x;
				else if (vertex.x > maxX)
					maxX = vertex.x;
				if (vertex.y < minY)
					minY = vertex.y;
				else if (vertex.y > maxY)
					maxY = vertex.y;
				if (vertex.z < minZ)
					minZ = vertex.z;
				else if (vertex.z > maxZ)
					maxZ = vertex.z;
			}
			if (minX < ovMinX)
				ovMinX = minX;
			if (minY < ovMinY)
				ovMinY = minY;
			if (minZ < ovMinZ)
				ovMinZ = minZ;
		}
	}
	if (!hasVertices)
		return BoundingBox(Vector3(0, 0, 0), Vector3(0, 0, 0));

	return BoundingBox(Vector3(ovMinX, ovMinY, ovMinZ), Vector3(ovMaxX, ovMaxY, ovMaxZ));
}

