#include "Mesh.h"

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
	bumpTexture	  = 0;
	vertices	  = NULL;
	textureCoords = NULL;
	normals		  = NULL;
	tangents	  = NULL;
	indices		  = NULL;
	colours		  = NULL;

	transformCoords = true;
}

Mesh::~Mesh(void)	{
	glDeleteVertexArrays(1, &arrayObject);			//Delete our VAO
	glDeleteBuffers(MAX_BUFFER, bufferObject);		//Delete our VBOs

	glDeleteTextures(1,&texture);					//We'll be nice and delete our texture when we're done with it
	glDeleteTextures(1,&bumpTexture);				//We'll be nice and delete our texture when we're done with it

	//Later tutorial stuff
	delete[]vertices;
	delete[]indices;
	delete[]textureCoords;
	delete[]tangents;
	delete[]normals;
	delete[]colours;
}

void Mesh::Draw(bool update)	{
	if(update) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bumpTexture);
	}

		glBindVertexArray(arrayObject);
		if(bufferObject[INDEX_BUFFER]) {
			glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
		}
		else{
			glDrawArrays(type, 0, numVertices);	//Draw the triangle!
		}
		glBindVertexArray(0);	
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
	m->GenerateTangents();
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
	m->normals			= new Vector3[m->numVertices];
	m->tangents			= new Vector3[m->numVertices];

	m->vertices[0] = 	Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = 	Vector3(-1.0f,	1.0f, 0.0f);
	m->vertices[2] = 	Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = 	Vector3(1.0f,  1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f,	1.0f);
	m->textureCoords[1] = Vector2(0.0f,	0.0f);
	m->textureCoords[2] = Vector2(1.0f,	1.0f);
	m->textureCoords[3] = Vector2(1.0f,	0.0f);

	for(int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f,1.0f,1.0f);
		m->normals[i] = Vector3(0.0f, 0.0f,-1.0f);
		m->tangents[i] = Vector3(1.0f, 0.0f,0.0f);
	}

	m->BufferData();

	return m;
}


Mesh * Mesh::GenerateWood()
{
	Mesh * m = new Mesh();
	m->numVertices = 90;
	m->type = GL_TRIANGLES;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->normals = new Vector3[m->numVertices];
	m->tangents = new Vector3[m->numVertices];

	for (int i = 0; i < 90; i += 3)
	{
		m->textureCoords[i] = Vector2(0.5f, 0.0f);
		m->textureCoords[i + 1] = Vector2(1.0f, 1.0f);
		m->textureCoords[i + 2] = Vector2(0.0f, 1.0f);
	}

	for (int i = 0; i < 90; ++i) {
		m->colours[i] = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
		m->tangents[i] = Vector3(1.0f, 0.0f, 0.0f);

	}

	/*for (int i = 0; i < m->numVertices; i++)
	{
	m->memory += sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector4) + sizeof(Vector3) + sizeof(Vector3);
	}*/



	//Sides
	m->vertices[0] = Vector3(1.0f, 0.0f, 0.0f);
	m->vertices[1] = Vector3(2.0f, 0.0f, 0.0f);
	m->vertices[2] = Vector3(2.0f, 1.0f, 0.0f);
	m->vertices[3] = Vector3(1.0f, 0.0f, 0.0f);
	m->vertices[4] = Vector3(2.0f, 1.0f, 0.0f);
	m->vertices[5] = Vector3(1.0f, 1.0f, 0.0f);

	m->vertices[6] = Vector3(2.0f, 0.0f, 0.0f);
	m->vertices[7] = Vector3(3.0f, 0.0f, -1.0f);
	m->vertices[8] = Vector3(3.0f, 1.0f, -1.0f);
	m->vertices[9] = Vector3(2.0f, 0.0f, 0.0f);
	m->vertices[10] = Vector3(3.0f, 1.0f, -1.0f);
	m->vertices[11] = Vector3(2.0f, 1.0f, 0.0f);

	m->vertices[12] = Vector3(3.0f, 0.0f, -1.0f);
	m->vertices[13] = Vector3(3.0f, 0.0f, -2.0f);
	m->vertices[14] = Vector3(3.0f, 1.0f, -2.0f);
	m->vertices[15] = Vector3(3.0f, 0.0f, -1.0f);
	m->vertices[16] = Vector3(3.0f, 1.0f, -2.0f);
	m->vertices[17] = Vector3(3.0f, 1.0f, -1.0f);

	m->vertices[18] = Vector3(3.0f, 0.0f, -2.0f);
	m->vertices[19] = Vector3(2.0f, 0.0f, -3.0f);
	m->vertices[20] = Vector3(2.0f, 1.0f, -3.0f);
	m->vertices[21] = Vector3(3.0f, 0.0f, -2.0f);
	m->vertices[22] = Vector3(2.0f, 1.0f, -3.0f);
	m->vertices[23] = Vector3(3.0f, 1.0f, -2.0f);

	m->vertices[24] = Vector3(2.0f, 0.0f, -3.0f);
	m->vertices[25] = Vector3(1.0f, 0.0f, -3.0f);
	m->vertices[26] = Vector3(1.0f, 1.0f, -3.0f);
	m->vertices[27] = Vector3(2.0f, 0.0f, -3.0f);
	m->vertices[28] = Vector3(1.0f, 1.0f, -3.0f);
	m->vertices[29] = Vector3(2.0f, 1.0f, -3.0f);

	m->vertices[30] = Vector3(1.0f, 0.0f, -3.0f);
	m->vertices[31] = Vector3(0.0f, 0.0f, -2.0f);
	m->vertices[32] = Vector3(0.0f, 1.0f, -2.0f);
	m->vertices[33] = Vector3(1.0f, 0.0f, -3.0f);
	m->vertices[34] = Vector3(0.0f, 1.0f, -2.0f);
	m->vertices[35] = Vector3(1.0f, 1.0f, -3.0f);

	m->vertices[36] = Vector3(0.0f, 0.0f, -2.0f);
	m->vertices[37] = Vector3(0.0f, 0.0f, -1.0f);
	m->vertices[38] = Vector3(0.0f, 1.0f, -1.0f);
	m->vertices[39] = Vector3(0.0f, 0.0f, -2.0f);
	m->vertices[40] = Vector3(0.0f, 1.0f, -1.0f);
	m->vertices[41] = Vector3(0.0f, 1.0f, -2.0f);

	m->vertices[42] = Vector3(0.0f, 0.0f, -1.0f);
	m->vertices[43] = Vector3(1.0f, 0.0f, -0.0f);
	m->vertices[44] = Vector3(1.0f, 1.0f, 0.0f);
	m->vertices[45] = Vector3(0.0f, 0.0f, -1.0f);
	m->vertices[46] = Vector3(1.0f, 1.0f, 0.0f);
	m->vertices[47] = Vector3(0.0f, 1.0f, -1.0f);

	//Top
	m->vertices[48] = Vector3(1.0f, 1.1f, -1.0f);
	m->vertices[49] = Vector3(2.0f, 1.1f, -1.0f);
	m->vertices[50] = Vector3(2.0f, 1.1f, -2.0f);
	m->vertices[51] = Vector3(1.0f, 1.1f, -1.0f);
	m->vertices[52] = Vector3(2.0f, 1.1f, -2.0f);
	m->vertices[53] = Vector3(1.0f, 1.1f, -2.0f);

	m->vertices[54] = Vector3(1.0f, 1.0f, 0.0f);
	m->vertices[55] = Vector3(2.0f, 1.0f, 0.0f);
	m->vertices[56] = Vector3(2.0f, 1.1f, -1.0f);
	m->vertices[57] = Vector3(1.0f, 1.0f, 0.0f);
	m->vertices[58] = Vector3(2.0f, 1.1f, -1.0f);
	m->vertices[59] = Vector3(1.0f, 1.1f, -1.0f);

	m->vertices[60] = Vector3(2.0f, 1.0f, 0.0f);
	m->vertices[61] = Vector3(3.0f, 1.0f, -1.0f);
	m->vertices[62] = Vector3(2.0f, 1.1f, -1.0f);

	m->vertices[63] = Vector3(3.0f, 1.0f, -1.0f);
	m->vertices[64] = Vector3(3.0f, 1.0f, -2.0f);
	m->vertices[65] = Vector3(2.0f, 1.1f, -2.0f);
	m->vertices[66] = Vector3(3.0f, 1.0f, -1.0f);
	m->vertices[67] = Vector3(2.0f, 1.1f, -2.0f);
	m->vertices[68] = Vector3(2.0f, 1.1f, -1.0f);

	m->vertices[69] = Vector3(3.0f, 1.0f, -2.0f);
	m->vertices[70] = Vector3(2.0f, 1.0f, -3.0f);
	m->vertices[71] = Vector3(2.0f, 1.1f, -2.0f);

	m->vertices[72] = Vector3(2.0f, 1.0f, -3.0f);
	m->vertices[73] = Vector3(1.0f, 1.0f, -3.0f);
	m->vertices[74] = Vector3(1.0f, 1.1f, -2.0f);
	m->vertices[75] = Vector3(2.0f, 1.0f, -3.0f);
	m->vertices[76] = Vector3(1.0f, 1.1f, -2.0f);
	m->vertices[77] = Vector3(2.0f, 1.1f, -2.0f);

	m->vertices[78] = Vector3(1.0f, 1.0f, -3.0f);
	m->vertices[79] = Vector3(0.0f, 1.0f, -2.0f);
	m->vertices[80] = Vector3(1.0f, 1.1f, -2.0f);

	m->vertices[81] = Vector3(0.0f, 1.0f, -2.0f);
	m->vertices[82] = Vector3(0.0f, 1.0f, -1.0f);
	m->vertices[83] = Vector3(1.0f, 1.1f, -1.0f);
	m->vertices[84] = Vector3(0.0f, 1.0f, -2.0f);
	m->vertices[85] = Vector3(1.0f, 1.1f, -1.0f);
	m->vertices[86] = Vector3(1.0f, 1.1f, -2.0f);

	m->vertices[87] = Vector3(0.0f, 1.0f, -1.0f);
	m->vertices[88] = Vector3(1.0f, 1.0f, 0.0f);
	m->vertices[89] = Vector3(1.0f, 1.1f, -1.0f);

	m->GenerateNormals();
	m->GenerateTangents();

	m->BufferData();
	return m;
}


Mesh* Mesh::GenerateQuadAlt()	{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];
	m->colours			= new Vector4[m->numVertices];
	m->normals			= new Vector3[m->numVertices];
	m->tangents			= new Vector3[m->numVertices];

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
		m->normals[i] = Vector3(0.0f, 0.0f,-1.0f);
		m->tangents[i] = Vector3(1.0f, 0.0f,0.0f);
	}

	m->BufferData();

	return m;
}
//NX 01/11/2012
Mesh* Mesh::GenerateQuadTexCoordCol(Vector2 scale, Vector2 texCoord, Vector4 colour)	{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];
	m->colours			= new Vector4[m->numVertices];
	m->normals			= new Vector3[m->numVertices];
	m->tangents			= new Vector3[m->numVertices];

	m->vertices[0] = 	Vector3(0.0f, 0.0f, 0.0f);
	m->vertices[1] = 	Vector3(0.0f, scale.y, 0.0f);
	m->vertices[2] = 	Vector3(scale.x, 0.0f, 0.0f);
	m->vertices[3] = 	Vector3(scale.x,  scale.y, 0.0f);

	m->textureCoords[0] = Vector2(texCoord.x,	texCoord.x);
	m->textureCoords[1] = Vector2(texCoord.x,	texCoord.y);
	m->textureCoords[2] = Vector2(texCoord.y,	texCoord.x);
	m->textureCoords[3] = Vector2(texCoord.y,	texCoord.y);

	for(int i = 0; i < 4; ++i) {
		m->colours[i] = colour;
		m->normals[i] = Vector3(0.0f, 0.0f,-1.0f);
		m->tangents[i] = Vector3(1.0f, 0.0f,0.0f);
	}

	m->BufferData();

	return m;
}
void	Mesh::BufferData()	{
	//GenerateNormals();
	//GenerateTangents();

	glBindVertexArray(arrayObject);

	//Buffer vertex data
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(VERTEX_BUFFER);

	//Buffer texture data
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

	//Buffer normal data
	if(normals) {
		glGenBuffers(1, &bufferObject[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}

	//Buffer tangent data
	if(tangents) {
		glGenBuffers(1, &bufferObject[TANGENT_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(TANGENT_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(TANGENT_BUFFER);
	}

	//buffer index data
	if(indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

/*
Stuff for later tutorials...
*/

void	Mesh::GenerateNormals()	{
	if(!normals) {
		normals = new Vector3[numVertices];
	}
	for(GLuint i = 0; i < numVertices; ++i){
		normals[i] = Vector3();
	}

	if(indices) {
		GLuint i = 0;

		int test = 0;
		for(i = 0; i < numIndices; i+=3){
			int a = indices[i];
			int b = indices[i+1];
			int c = indices[i+2];

			Vector3 normal = Vector3::Cross((vertices[b]-vertices[a]),(vertices[c]-vertices[a]));

			normals[a] += normal;
			normals[b] += normal;
			normals[c] += normal;

			test+=3;
		}
		bool asdf = true;
	}
	else{
		//It's just a list of triangles, so generate face normals
		for(GLuint i = 0; i < numVertices; i+=3){
			Vector3 &a = vertices[i];
			Vector3 &b = vertices[i+1];
			Vector3 &c = vertices[i+2];

			Vector3 normal = Vector3::Cross(b-a,c-a);

			normals[i]	 = normal;
			normals[i+1] = normal;
			normals[i+2] = normal;
		}
	}

	for(GLuint i = 0; i < numVertices; ++i){
		normals[i].Normalise();
	}
}

void Mesh::GenerateTangents() {
	//Extra! stops rare occurrence of this function being called
	//on a mesh without tex coords, which would break quite badly!
	if(!textureCoords) {
		return;
	}

	if(!tangents) {
		tangents = new Vector3[numVertices];
	}
	for(GLuint i = 0; i < numVertices; ++i){
		tangents[i] = Vector3();
	}

	if(indices) {
		for(GLuint i = 0; i < numIndices; i+=3){
			int a = indices[i];
			int b = indices[i+1];
			int c = indices[i+2];

			Vector3 tangent = GenerateTangent(vertices[a],vertices[b],vertices[c],textureCoords[a],textureCoords[b],textureCoords[c]);

			tangents[a] += tangent;
			tangents[b] += tangent;
			tangents[c] += tangent;
		}
	}
	else{
		for(GLuint i = 0; i < numVertices; i+=3){
			Vector3 tangent = GenerateTangent(vertices[i],vertices[i+1],vertices[i+2],textureCoords[i],textureCoords[i+1],textureCoords[i+2]);

			tangents[i]   += tangent;
			tangents[i+1] += tangent;
			tangents[i+2] += tangent;
		}
	}
	for(GLuint i = 0; i < numVertices; ++i){
		tangents[i].Normalise();
	}
}

Vector3 Mesh::GenerateTangent(const Vector3 &a,const Vector3 &b,const Vector3 &c,const Vector2 &ta,const Vector2 &tb,const Vector2 &tc)	 {
	Vector2 coord1  = tb-ta;
	Vector2 coord2  = tc-ta;

	Vector3 vertex1 = b-a;
	Vector3 vertex2 = c-a;

	Vector3 axis = Vector3(vertex1*coord2.y - vertex2*coord1.y);

	float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);

	return axis * factor;
}

void Mesh::DrawDebugNormals(float length)	{
	if(numVertices > 0) {
		GLuint array;
		GLuint buffer;
		GLuint cbuffer;

		glGenVertexArrays(1, &array);
		glBindVertexArray(array);

		Vector3 *tempV = new Vector3[numVertices*2];
		Vector3 *tempC = new Vector3[numVertices*2];

		for(GLuint i = 0; i < numVertices; ++i){
			tempC[(i*2)]  = Vector3(1,0,0);
			tempC[(i*2)+1] = Vector3(1,1,1);

			tempV[(i*2)]   = vertices[i];
			tempV[(i*2)+1] = vertices[i] + (normals[i] * length);
		}

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3)*2, tempV, GL_STREAM_DRAW);
		glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(VERTEX_BUFFER);

		glGenBuffers(1, &cbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3)*2, tempC, GL_STREAM_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(COLOUR_BUFFER);

		glPointSize(4.0f);
		glLineWidth(2.0f);
		glDrawArrays(GL_POINTS,  0, numVertices*2);	// draw ordered list of vertices
		glDrawArrays(GL_LINES,  0, numVertices*2);	// draw ordered list of vertices
		glLineWidth(1.0f);
		glPointSize(1.0f);

		glBindVertexArray(0);

		glDeleteVertexArrays(1, &array);
		glDeleteBuffers(1, &buffer);
		glDeleteBuffers(1, &cbuffer);

		delete[] tempV;
		delete[] tempC;
	}

	//for(unsigned int i = 0; i < children.size(); ++i) {
	//	children.at(i)->DrawDebugNormals();
	//}
}

void Mesh::DrawDebugTangents(float length)	{
	if(numVertices > 0) {
		GLuint array;
		GLuint buffer;
		GLuint cbuffer;

		glGenVertexArrays(1, &array);
		glBindVertexArray(array);

		Vector3 *tempV = new Vector3[numVertices*2];
		Vector3 *tempC = new Vector3[numVertices*2];

		for(GLuint i = 0; i < numVertices; ++i){
			tempC[(i*2)]  = Vector3(1,1,1);
			tempC[(i*2)+1] = Vector3(0,1,0);

			tempV[(i*2)]   = vertices[i];
			tempV[(i*2)+1] = vertices[i] + (tangents[i] * length);
		}

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3)*2, tempV, GL_STREAM_DRAW);
		glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(VERTEX_BUFFER);

		glGenBuffers(1, &cbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3)*2, tempC, GL_STREAM_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(COLOUR_BUFFER);

		glPointSize(4.0f);
		glLineWidth(2.0f);
		glDrawArrays(GL_POINTS,  0, numVertices*2);	// draw ordered list of vertices
		glDrawArrays(GL_LINES,  0, numVertices*2);	// draw ordered list of vertices
		glLineWidth(1.0f);
		glPointSize(1.0f);

		glBindVertexArray(0);

		glDeleteVertexArrays(1, &array);
		glDeleteBuffers(1, &buffer);
		glDeleteBuffers(1, &cbuffer);

		delete[] tempV;
		delete[] tempC;
	}

	//for(unsigned int i = 0; i < children.size(); ++i) {
	//	children.at(i)->DrawDebugTangents();
	//}
}