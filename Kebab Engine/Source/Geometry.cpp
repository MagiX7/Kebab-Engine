#include "Geometry.h"

#include "Globals.h"

#include "mmgr/mmgr.h"

KbGeometry::~KbGeometry()
{
	glDeleteBuffers(1, &texBuffer);
	//normals.clear();
	//RELEASE_ARRAY(vertices);
	//RELEASE_ARRAY(indices);
	delete[](normals);
	normals = nullptr;
	//RELEASE_ARRAY(texCoords);

	delete(vertexBuffer);
	vertexBuffer = nullptr;

	delete(indexBuffer);
	indexBuffer = nullptr;
	
	//delete(texture);
	//texture = nullptr;
}

void KbGeometry::SetUpBuffers()
{
	vertexBuffer = new VertexBuffer();
	vertexBuffer->SetData(vertices);
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
	
	/*SetCheckersTexture();
	currentTexture = checkersTexture;*/
}