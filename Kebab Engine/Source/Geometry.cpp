#include "Geometry.h"

#include "Globals.h"

KbGeometry::~KbGeometry()
{
	glDeleteBuffers(1, &texBuffer);
	//normals.clear();
	//RELEASE_ARRAY(vertices);
	//RELEASE_ARRAY(indices);
	RELEASE_ARRAY(normals);
	//RELEASE_ARRAY(texCoords);

	RELEASE(vertexBuffer);
	RELEASE(indexBuffer);
	RELEASE(texture);
}

void KbGeometry::SetUpBuffers()
{
	vertexBuffer = new VertexBuffer();
	vertexBuffer->SetData(vertices);
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	checkersTexture = SetCheckersTexture();
	currentTexture = checkersTexture;

	//vertexArray = new VertexArray();
	/*vertexArray->AddVertexBuffer(*vertexBuffer);
	vertexArray->SetIndexBuffer(*indexBuffer);*/
}

//void KbGeometry::SetCheckerTexture()
//{
//	/*GLubyte checkerImage[40][40][4];
//	for (int i = 0; i < 40; i++) {
//		for (int j = 0; j < 40; j++) {
//			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
//			checkerImage[i][j][0] = (GLubyte)c;
//			checkerImage[i][j][1] = (GLubyte)c;
//			checkerImage[i][j][2] = (GLubyte)c;
//			checkerImage[i][j][3] = (GLubyte)255;
//		}
//	}
//
//	texture = new Texture(checkerImage, 40, 40);*/
//
//}