#include "Mesh.h"
#include <cassert>
#include "includes.h"


Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
}

Mesh::Mesh(const std::string& fileName, float scale , bool isOBJ) {

	if (isOBJ) loadOBJ(fileName, scale);
	else loadASE(fileName, scale);
	
	computeTangent();
	allocateARBBuffers();
}
Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
}

Mesh::~Mesh()
{
	if( vertices_vbo_id ) glDeleteBuffersARB( 1, &vertices_vbo_id );
	if( normals_vbo_id ) glDeleteBuffersARB( 1, &normals_vbo_id );
	if( uvs_vbo_id ) glDeleteBuffersARB( 1, &uvs_vbo_id );
	if( colors_vbo_id ) glDeleteBuffersARB( 1, &colors_vbo_id );
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
}

void Mesh::loadASE(const std::string& fileName, float scale) {

	TextParser t(addCurrentPath(fileName).c_str());
	
	clear();
	
	int numVertex, numFaces, numTVertex, numTFaces, numNormals;

	t.seek("*MESH_NUMVERTEX");
	numVertex = t.getint();
	t.seek("*MESH_NUMFACES");
	numFaces = t.getint();

	std::vector< Vector3 > verticesASE(numVertex);

	//Vertices
	for (int i = 0; i < numVertex; i++) {
		t.seek("*MESH_VERTEX");
		t.getint(); // salta la id

		verticesASE[i].x = t.getfloat(); // coord x
		verticesASE[i].y = t.getfloat(); // coord y
		verticesASE[i].z = t.getfloat(); // coord z

		centroid = centroid + verticesASE[i];
	}

	centroid = centroid / float(numVertex);

	//std::vector< Vector3 > faces(numFaces);

	//Faces
	for (int i = 0; i<numFaces; i++) {
		t.seek("*MESH_FACE");
		t.getword();

		//face A
		t.getword();
		vertices.push_back(verticesASE[t.getint()]);
		//faces[i].x = t.getint();

		//face B
		t.getword();
		vertices.push_back(verticesASE[t.getint()]);
		//faces[i].y = t.getint();

		//face C
		t.getword();
		vertices.push_back(verticesASE[t.getint()]);
		//faces[i].z = t.getint();
	}
	
	//Textures Vertex, we first check if the file has Textures or not, since knot.ASE does not have textures
	if (t.seek("*MESH_NUMTVERTEX")) {

		numTVertex = t.getint();


		std::vector<Vector2> textureVertices(numTVertex);

		for (int i = 0; i < numTVertex; i++) {
			t.seek("*MESH_TVERT");
			t.getint();

			textureVertices[i].x = t.getfloat();
			textureVertices[i].y = t.getfloat();
		}

		//Texture faces
		t.seek("*MESH_NUMTVFACES");
		numTFaces = t.getint();

		std::vector<Vector3> textureFaces(numTFaces);

		for (int i = 0; i < numTFaces; i++) {
			t.seek("*MESH_TFACE");
			t.getint();

			textureFaces[i].x = t.getint();
			textureFaces[i].y = t.getint();
			textureFaces[i].z = t.getint();

		}

		//We store the values for the texture int uv
		for (int i = 0; i < numTFaces; i++) {
			uvs.push_back(textureVertices[textureFaces[i].x]);
			uvs.push_back(textureVertices[textureFaces[i].y]);
			uvs.push_back(textureVertices[textureFaces[i].z]);
		}
	}

	//We store the normals ( if has )
	Vector3 normal;
	while (t.seek("*MESH_VERTEXNORMAL")) {
		t.getint();
		normal.x = t.getfloat();
		normal.y = t.getfloat();
		normal.z = t.getfloat();

		normals.push_back(normal);
	}

	//for (int i = 0; i < numFaces; i++) {
	//	vertices.push_back(verticesASE[faces[i].x]);
	//	vertices.push_back(verticesASE[faces[i].y]);
	//	vertices.push_back(verticesASE[faces[i].z]);
	//}

	//Then, we scale vertices
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].x *= scale;
		vertices[i].y*= scale;
		vertices[i].z *= scale;
	}

}
// since finding a .ASE file is exhausting we prefer do a Parser for .obj models
// code finded and rearrenged from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
void Mesh::loadOBJ(const std::string& fileName, float scale) {

	std::vector<Vector3> verticesOBJ, normalsOBJ;
	std::vector<Vector2> texturescoordOBJ;
	std::vector<int> v_verticesindex, v_uvindex, v_normalindex;
	Vector3 v3_auxiliar, v3_vertexindex,v3_uvindex,v3_normalindex;
	Vector2 v2_auxiliar;
	int linecount = 0;

	FILE * file = fopen(addCurrentPath(fileName).c_str(), "r");

	if (file == NULL) {
		printf("Could not open the file");
		return;
	}

	while (1) {
		char lineheader[256];
		int res = fscanf(file, "%s", lineheader);
		linecount++;
		if (res == EOF) break;
		if (strcmp(lineheader, "v") == 0) {
			fscanf(file, "%f %f %f\n", &v3_auxiliar.x, &v3_auxiliar.y, &v3_auxiliar.z);
			verticesOBJ.push_back(v3_auxiliar);
		}
		else if (strcmp(lineheader, "vt") == 0) {
			fscanf(file, "%f %f\n", &v2_auxiliar.x, &v2_auxiliar.y);
			texturescoordOBJ.push_back(v2_auxiliar);
		}
		else if (strcmp(lineheader, "vn") == 0) {
			fscanf(file, "%f %f %f\n", &v3_auxiliar.x, &v3_auxiliar.y, &v3_auxiliar.z);
			normalsOBJ.push_back(v3_auxiliar);
		}
		else if (strcmp(lineheader, "f") == 0) {
			int v1, v2, v3, v4, uv1, uv2, uv3, uv4, n1, n2, n3, n4;
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &uv1, &n1, &v2, &uv2, &n2
				, &v3, &uv3, &n3, &v4,&uv4,&n4);

			if (matches < 9) {
				printf("Error"+ linecount);
				return;
			}
			else if (matches >= 9) {
				v_verticesindex.push_back(v1);
				v_verticesindex.push_back(v2);
				v_verticesindex.push_back(v3);

				v_normalindex.push_back(n1);
				v_normalindex.push_back(n2);
				v_normalindex.push_back(n3);

				v_uvindex.push_back(uv1);
				v_uvindex.push_back(uv2);
				v_uvindex.push_back(uv3);

				if (matches == 12) {
					// A veces hay modelos de obj que los indcies del triangulo son mas de 3
					// Informacion extraida: https://stackoverflow.com/questions/23723993/converting-quadriladerals-in-an-obj-file-into-triangles
					// En este caso hemos hecho para 4 indices, si son para mas, mejor pasarlo pro el Blender y triangularlo
					v_verticesindex.push_back(v1);
					v_verticesindex.push_back(v3);
					v_verticesindex.push_back(v4);

					v_normalindex.push_back(n1);
					v_normalindex.push_back(n3);
					v_normalindex.push_back(n4);

					v_uvindex.push_back(uv1);
					v_uvindex.push_back(uv2);
					v_uvindex.push_back(uv4);
				}

			}
			

		}
	}
	int a = 0;
	for (int i = 0; i < v_verticesindex.size(); i++) {
		vertices.push_back(verticesOBJ[v_verticesindex[i] - 1] * scale);
		normals.push_back(normalsOBJ[v_normalindex[i] - 1]);
		uvs.push_back(texturescoordOBJ[v_uvindex[i] - 1]);
	}

	

	//allocateARBBuffers();
}

void Mesh::allocateARBBuffers(void)
{
	glGenBuffersARB(1, &vertices_vbo_id);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices_vbo_id);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector3) * vertices.size(), &vertices[0], GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glGenBuffersARB(1, &normals_vbo_id);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normals_vbo_id);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector3) * normals.size(), &normals[0], GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glGenBuffersARB(1, &uvs_vbo_id);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, uvs_vbo_id);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector2) * uvs.size(), &uvs[0], GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	if (colors.size() > 0)
	{
		glGenBuffersARB(1, &colors_vbo_id);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, colors_vbo_id);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector4) * colors.size(), &colors[0], GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
	if (tangents.size() >= 0) {
		glGenBuffersARB(1, &tangent_vbo_id);
		glBindBufferARB(GL_ARRAY_BUFFER, tangent_vbo_id);
		glBufferDataARB(GL_ARRAY_BUFFER, sizeof(Vector3)*tangents.size(), &tangents[0], GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
	if (bitangents.size() >= 0) {
		glGenBuffersARB(1, &bitangent_vbo_id);
		glBindBufferARB(GL_ARRAY_BUFFER, bitangent_vbo_id);
		glBufferDataARB(GL_ARRAY_BUFFER, sizeof(Vector3)*bitangents.size(), &bitangents[0], GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
}

void Mesh::render( int primitive )
{
	assert( vertices.size() && "No vertices in this mesh" );

	glEnableClientState( GL_VERTEX_ARRAY );

	if( vertices_vbo_id )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertices_vbo_id );
		glVertexPointer( 3, GL_FLOAT, 0, NULL );
	}
	else
		glVertexPointer( 3, GL_FLOAT, 0, &vertices[ 0 ] );

	if( normals.size() )
	{
		glEnableClientState( GL_NORMAL_ARRAY );
		if( normals_vbo_id )
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, normals_vbo_id );
			glNormalPointer( GL_FLOAT, 0, NULL );
		}
		else
			glNormalPointer( GL_FLOAT, 0, &normals[ 0 ] );
	}

	if( uvs.size() )
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		if( uvs_vbo_id )
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, uvs_vbo_id );
			glTexCoordPointer( 2, GL_FLOAT, 0, NULL );
		}
		else
			glTexCoordPointer( 2, GL_FLOAT, 0, &uvs[ 0 ] );
	}

	if( colors.size() )
	{
		glEnableClientState( GL_COLOR_ARRAY );
		if( colors_vbo_id )
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, colors_vbo_id );
			glColorPointer( 4, GL_FLOAT, 0, NULL );
		}
		else
			glColorPointer( 4, GL_FLOAT, 0, &colors[ 0 ] );
	}

	glDrawArrays( primitive, 0, (GLsizei)vertices.size() );
	glDisableClientState( GL_VERTEX_ARRAY );

	if( normals.size() )
		glDisableClientState( GL_NORMAL_ARRAY );
	if( uvs.size() )
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	if( colors.size() )
		glDisableClientState( GL_COLOR_ARRAY );
}

void Mesh::createQuad( float center_x, float center_y, float w, float h, bool flip_uvs )
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back( Vector3( center_x + w*0.5f, center_y + h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x + w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y + h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x + w*0.5f, center_y + h*0.5f, 0.0f ) );

	//texture coordinates
	uvs.push_back( Vector2( 1.0f, flip_uvs ? 0.0f : 1.0f ) );
	uvs.push_back( Vector2( 0.0f, flip_uvs ? 1.0f : 0.0f ) );
	uvs.push_back( Vector2( 1.0f, flip_uvs ? 1.0f : 0.0f ) );
	uvs.push_back( Vector2( 0.0f, flip_uvs ? 0.0f : 1.0f ) );
	uvs.push_back( Vector2( 0.0f, flip_uvs ? 1.0f : 0.0f ) );
	uvs.push_back( Vector2( 1.0f, flip_uvs ? 0.0f : 1.0f ) );

	//all of them have the same normal
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
}

void Mesh::createPlane( float size )
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back( Vector3( size, 0, size ) );
	vertices.push_back( Vector3( size, 0, -size ) );
	vertices.push_back( Vector3( -size, 0, -size ) );
	vertices.push_back( Vector3( -size, 0, size ) );
	vertices.push_back( Vector3( size, 0, size ) );
	vertices.push_back( Vector3( -size, 0, -size ) );

	//all of them have the same normal
	normals.push_back( Vector3( 0, 1, 0 ) );
	normals.push_back( Vector3( 0, 1, 0 ) );
	normals.push_back( Vector3( 0, 1, 0 ) );
	normals.push_back( Vector3( 0, 1, 0 ) );
	normals.push_back( Vector3( 0, 1, 0 ) );
	normals.push_back( Vector3( 0, 1, 0 ) );

	//texture coordinates
	uvs.push_back( Vector2( 1, 1 ) );
	uvs.push_back( Vector2( 1, 0 ) );
	uvs.push_back( Vector2( 0, 0 ) );
	uvs.push_back( Vector2( 0, 1 ) );
	uvs.push_back( Vector2( 1, 1 ) );
	uvs.push_back( Vector2( 0, 0 ) );

	allocateARBBuffers();
}

void Mesh:: invertMe(){
	//Reverse vertices
	std::vector<Vector3> v_reversed;
	v_reversed.reserve(vertices.size());

	auto iter = vertices.rbegin();

	for(;iter!=vertices.rend();++iter){
		v_reversed.push_back(*iter);
	}

	vertices.swap(v_reversed);

	//Reverse normals
	std::vector<Vector3> v_reversednormals;
	v_reversednormals.reserve(normals.size());

	auto itern = normals.rbegin();

	for (; itern != normals.rend(); ++itern) {
		v_reversednormals.push_back(*itern);
	}

	normals.swap(v_reversednormals);

	//Reverse uvs
	std::vector<Vector2> v_reverseduv;
	v_reverseduv.reserve(uvs.size());

	auto iteruv = uvs.rbegin();

	for (; iteruv != uvs.rend(); ++iteruv) {
		v_reverseduv.push_back(*iteruv);
	}

	v_reverseduv.swap(uvs);
}
//Computing Tangents and bitangents
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
void Mesh::computeTangent() {
	for (int i = 0; i < vertices.size(); i += 3) {
		Vector3 v0 = vertices[i];
		Vector3 v1 = vertices[i + 1];
		Vector3 v2 = vertices[i + 2];

		Vector2 uv0 = uvs[i];
		Vector2 uv1 = uvs[i+1];
		Vector2 uv2 = uvs[i+2];

		Vector3 deltapos1 = v1 - v0;
		Vector3 deltapos2 = v2 - v0;

		Vector2 deltauv1 = uv1 - uv0;
		Vector2 deltauv2 = uv2 - uv0;

		float r = 1.0f / (deltauv1.x*deltauv2.y - deltauv1.y*deltauv2.x);
		Vector3 tangent = (deltapos1*deltauv2.y - deltapos2 * deltauv1.y)*r;
		Vector3 bitangent = (deltapos2*deltauv1.x - deltapos1 * deltauv2.x)*r;

		//Add the tangents and bitangent to the buffer
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}
}

