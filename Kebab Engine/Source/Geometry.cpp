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
	//RELEASE(vertexArray);
	RELEASE(texture);
}

void KbGeometry::SetUpBuffers()
{
	vertexBuffer = new VertexBuffer();
	vertexBuffer->SetData(vertices);
	//vertexBuffer = new VertexBuffer(vertices, sizeof(vertices) * verticesCount);
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	/*glGenBuffers(1, &texBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
	glBufferData(GL_ARRAY_BUFFER, texCoordsCount * sizeof(float2), &texCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	SetCheckerTexture();

	//vertexArray = new VertexArray();
	/*vertexArray->AddVertexBuffer(*vertexBuffer);
	vertexArray->SetIndexBuffer(*indexBuffer);*/
}

void KbGeometry::Draw(bool showNormals)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//vertexArray->Bind();
	
	vertexBuffer->Bind();
	indexBuffer->Bind();
	glVertexPointer(3, GL_FLOAT, 0, 0);

	texture->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	texture->Unbind();

	vertexBuffer->Unbind();
	indexBuffer->Unbind();

	//vertexArray->Unbind();
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void KbGeometry::SetCheckerTexture()
{
	GLubyte checkerImage[40][40][4];
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	texture = new Texture(checkerImage, 40, 40);

}